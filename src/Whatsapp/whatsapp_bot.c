#include <curl/curl.h>
#include <curl/easy.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "whatsapp_bot.h"
#include "../Networking/HttpRequest.h"
#include "../Networking/Server.h"

char* launch_web_hook(struct Server *server);
void set_webhook_properties(struct WhatsappBot *bot,
                             int domain,int service,int protocol,
                             u_long iterface,int port,int backlog);
void parse_received_msg(struct WhatsappBot *bot, char* msg);
void parse_whatsapp_body_text(struct Dictionary *body_fields,char* body_text);
int send_whatsapp_msg(struct WhatsappBot *bot);
void set_sending_properties(struct WhatsappBot *bot,
                             char URL[], char Autorization[],
                             char ContentType[], char Data[],
                             unsigned long data_size);
void read_text_file(char* file_path, char** buffer);
void write_text_file(char* file_path, char* buffer);
void assign_string(char** dest,char* source);
void HTTPRequest_Body_To_WhatsappMessage(struct WhatsappBot *bot,struct Dictionary body); 

struct WhatsappBot whatsappbot_constructor(char* name) {
    struct WhatsappBot new_bot;
    new_bot.name = name;
    new_bot.read_text_file = read_text_file;
    new_bot.write_text_file = write_text_file;
    new_bot.set_webhook_properties = set_webhook_properties ;
    new_bot.parse_received_msg = parse_received_msg;
    new_bot.send_whatsapp_msg = send_whatsapp_msg;
    new_bot.set_sending_properties = set_sending_properties;
    return new_bot;
    
}


void whatsappbot_destructor(struct WhatsappBot *bot) {
    printf("destroying bot\n");
    /*if(bot->whatsappcurl.Data)
        free(bot->whatsappcurl.Data);
    if(bot->received_message.name)
        free(bot->received_message.name);
    if(bot->received_message.wa_id)
        free(bot->received_message.wa_id);
    if(bot->received_message.from)
        free(bot->received_message.from);
    if(bot->received_message.id)
        free(bot->received_message.id);
    if(bot->received_message.timestamp)
        free(bot->received_message.timestamp);
    if(bot->received_message.type)
        free(bot->received_message.type);
    if(bot->received_message.text_body)
        free(bot->received_message.text_body);*/

}

char* launch_web_hook(struct Server *server) {
    #define buffer_size 300000
    int address_length = sizeof(server->address);
    int new_socket;
    char *buffer = calloc(1,buffer_size);

    while(1) {
        printf("\n===== WAITING FOR CONNECTION =====\n");

        new_socket = accept(server->socket, (struct sockaddr*)&server->address,
                            (socklen_t*)&address_length);
        if(new_socket > 0)
            printf("\n===== CONNECTION SUCCESS =====\n");
        read(new_socket , buffer , buffer_size);
        close(new_socket);
        return buffer;
    }
}

void set_webhook_properties(struct WhatsappBot *bot,
                             int domain,int service,int protocol,
                             u_long iterface,int port,int backlog) {
    printf("setting web-properties\n");
    bot->webhook = server_constructor(domain , service , protocol,
                                      iterface,port,backlog,
                                      launch_web_hook);
    
}

void set_sending_properties(struct WhatsappBot *bot,
                             char URL[] , char Autorization[],
                             char ContentType[], char Data[],
                             unsigned long data_size) {
    strcpy(bot->whatsappcurl.URL , URL);
    strcpy(bot->whatsappcurl.Header_Autorization , Autorization);
    strcpy(bot->whatsappcurl.Header_ContentType , ContentType);
    bot->whatsappcurl.Data = malloc(data_size+1);
    strncpy(bot->whatsappcurl.Data ,Data, data_size);
    
}

void assign_string(char** dest , char* source) {
    *dest = malloc(sizeof(char[strlen(source)+1]));
    strncpy(*dest,source,sizeof(char[strlen(source)])); 
};

void HTTPRequest_Body_To_WhatsappMessage(struct WhatsappBot *bot,
                                         struct Dictionary body) {
    
    assign_string(&bot->received_message.name,
                  body.search(&body,"name",sizeof("name")));  
    assign_string(&bot->received_message.from,
                  body.search(&body,"from",sizeof("from")));
    assign_string(&bot->received_message.id,
                  body.search(&body,"id",sizeof("id")));
    assign_string(&bot->received_message.type,
                  body.search(&body,"type",sizeof("type")));
    assign_string(&bot->received_message.wa_id,
                  body.search(&body,"wa_id",sizeof("wa_id"))); 
    assign_string(&bot->received_message.text_body,
                  body.search(&body,"body",sizeof("body")));
    assign_string(&bot->received_message.timestamp,
                  body.search(&body,"timestamp",sizeof("timestamp")));
}

void remove_all_chars(char* str, char c) {
    char *pr = str, *pw = str;
    while (*pr) {
        *pw = *pr++;
        pw += (*pw != c);
    }
    *pw = '\0';
}

void check_for_carage_return_char(char* input) {
    printf("checking for carage return\n");
    for(int i;i < strlen(input);i++) {
        if((int)input[i] == 0xD) {
            printf("carage return character detected, attempting remove...\n");
        }
    }

}

void parse_received_msg(struct WhatsappBot *bot, char* msg) {
    remove_all_chars(msg , 0xD);
    struct HTTPRequest sent_request = http_request_constructor(msg , NULL);
    
    /*for(int i = 0; i < sent_request.body.keys.length; i++) {
                char* key_name = (char*)sent_request.body.keys
                                 .retreive(&sent_request.body.keys,i);
                char* value = (char*)sent_request
                              .body.search(&sent_request.body,
                                           key_name,sizeof(strlen(key_name)));
                printf("key: %s\nvalue: %s\n",key_name,value); 
    }*/

    HTTPRequest_Body_To_WhatsappMessage(bot,sent_request.body);
    //http_request_destructor(&sent_request);
}

int send_whatsapp_msg(struct WhatsappBot *bot) {
    //#define USE_CHUNKED
    //#define DISABLE_EXPECT
    printf("Sending whatsapp message From %s \n\n%s\n\n",
           bot->name,bot->whatsappcurl.Data);
    CURLcode result; 
    result = curl_global_init(CURL_GLOBAL_DEFAULT);
    if(result != CURLE_OK) {
        fprintf(stderr, "curl_global_init() failed: %s\n",
                curl_easy_strerror(result));
        return 1;
    }
 
    CURL *curl = curl_easy_init();
    if(curl) {

        struct curl_slist *header_list = NULL;
        header_list = curl_slist_append(header_list, 
                                        bot->whatsappcurl.Header_Autorization
                                        );
        header_list = curl_slist_append(header_list, 
                                        bot->whatsappcurl.Header_ContentType
                                        );

        curl_easy_setopt(curl, CURLOPT_URL, bot->whatsappcurl.URL);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, bot->whatsappcurl.Data);
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        

        #ifdef USE_CHUNKED 
        {
            header_list = curl_slist_append(header_list,
                                      "Transfer-Encoding: chunked");
            result = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);
        }
        #endif

        #ifdef DISABLE_EXPECT 
        {
            header_list = curl_slist_append(header_list, "Expect:");
            result = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);
        }
        #endif
        result = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);
        result = curl_easy_perform(curl);
        
        if(result != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(result));
        }
        curl_easy_cleanup(curl);
        curl_slist_free_all(header_list);
    }
    curl_global_cleanup();
    return 0;
}


void read_text_file(char* file_path, char** buffer) {
    FILE *file_pointer;
    long File_SIZE = 0;

    file_pointer = fopen(file_path, "r");
    if(!file_pointer) {
        perror(file_path);
        exit(1);
    }
    fseek(file_pointer,0L,SEEK_END);
    File_SIZE = ftell(file_pointer);
    rewind(file_pointer);
    *buffer = calloc(1,File_SIZE+1);
    if(!*buffer) {
        fclose(file_pointer);
        fputs("memory alloc failed",stderr);
        exit(1);
    }

    if(1!=fread(*buffer, File_SIZE, 1 , file_pointer)) {
        fclose(file_pointer);
        free(*buffer);
        fputs("entire read failed",stderr);
        exit(1);
    }
}

void write_text_file(char* file_path, char* buffer) {
    FILE *file_pointer = fopen(file_path,"w");
    fprintf(file_pointer,"%s\n",buffer);
    fclose(file_pointer);
}
