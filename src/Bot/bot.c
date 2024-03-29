#include <openssl/err.h>
#include <openssl/ssl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
#include <io.h>
#elif __linux__
#include <unistd.h>
#endif
#include <curl/curl.h>
#include <curl/easy.h>
#include "bot.h"
#include "../Networking/HttpRequest.h"
#include "../Networking/Server.h"

struct HTTPRequest parse_to_httpresponse(char* msg);
void assign_string(char** dest,char* source);

char* launch_ssl_listner_server(struct SSL_Server *server,
                                             char* server_response_message) {
   
    #define buffer_size 300000
    char buffer[buffer_size] = {0};
 
    char server_response[strlen(server_response_message)+41]; 
    strcpy(server_response,"HTTP/1.1 200 OK\nConnection: Closed\n\n");
    strcat(server_response,server_response_message);
    
    if (listen(server->socket , server->backlog) < 0) {
        perror("Failed to start listening...\n");
        exit(1);
    }

    

    int address_length = sizeof(server->address);
    int new_socket;
 
    printf("\n===== WAITING FOR CONNECTION =====\n");

    new_socket = accept(server->socket, (struct sockaddr*)&server->address,
                        (socklen_t*)&address_length);
    if(new_socket < 0) {
        perror("unable to accept...\n");
        exit(1);
    }

    SSL_set_fd(server->ssl, new_socket);
    int ssl_err = SSL_accept(server->ssl);

    if(ssl_err <= 0) {
        printf("ssl error : %s\n",ERR_error_string(ERR_get_error() , NULL));
        SSL_shutdown(server->ssl);
        exit(1);
    }
    else {
        printf("\n===== CONNECTION SUCCESS =====\n");
       
        SSL_read(server->ssl, (char*)buffer, buffer_size);
        SSL_write(server->ssl, server_response, strlen(server_response));
        
        if(strlen(buffer) < 2) {
            printf("No data transfered\n%s\n",buffer);
            exit(1);
        }

        printf("\n==== CONNECTION DATA START ====\n%s\n===== CONNECTION DATA END=====\n",
           buffer);
        int option = 1;
        setsockopt(server->socket,SOL_SOCKET,SO_REUSEADDR,(char *)&option,sizeof(option));
        #ifdef _WIN32
        closesocket(new_socket);
        #elif __linux__
        close(new_socket);
        #endif
        SSL_shutdown(server->ssl);

        char* ret_buffer = malloc(buffer_size);
        strncpy(ret_buffer, buffer,buffer_size);
        return ret_buffer;
    }
}

char* launch_listner_server(struct Server *server,
                                         char* server_response_message) {
    #define buffer_size 300000
    char buffer[buffer_size] = {0};

    char server_response[strlen(server_response_message)+40]; 
    strcpy(server_response,"HTTP/1.1 200 OK\nConnection: Closed\n\n");
    strcat(server_response,server_response_message);
    
    if (listen(server->socket , server->backlog) < 0) {
        perror("Failed to start listening...\n");
        exit(1);
    }

    int address_length = sizeof(server->address);
    int new_socket;

    printf("\n===== WAITING FOR CONNECTION =====\n");
    
    new_socket = accept(server->socket, (struct sockaddr*)&server->address,
                        (socklen_t*)&address_length); //while loop in accept();
    if(new_socket < 0) {
        perror("unable to accept...\n");
        exit(1);
    }    
    else {
        printf("\n===== CONNECTION SUCCESS =====\n");
        #ifdef _WIN32 
        recv(new_socket, buffer, buffer_size,0);
        send(new_socket, server_response, (int)strlen(server_response), 0);
        #elif __linux__
        read(new_socket , buffer , buffer_size);
        write(new_socket, server_response, strlen(server_response));
        #endif
        if(strlen(buffer) < 2) {
            printf("No data transfered\n%s\n",buffer);
            exit(1);
        }

        printf("\n==== CONNECTION DATA START ====\n%s\n===== CONNECTION DATA END=====\n",
           buffer);
        int option = 1;
        setsockopt(server->socket,SOL_SOCKET,SO_REUSEADDR,(char *)&option,sizeof(option));
        #ifdef _WIN32
        closesocket(new_socket);
        #elif __linux__
        close(new_socket);
        #endif

        char* ret_buffer = malloc(buffer_size);
        strncpy(ret_buffer, buffer,buffer_size);
        return ret_buffer;
    }
}

void assign_string(char** dest , char* source) {
        *dest = malloc(sizeof(char[strlen(source)+1]));
        strncpy(*dest,source,sizeof(char[strlen(source)])+1); 
};

void remove_all_chars(char* str, char c) {
    char *pr = str, *pw = str;
    while (*pr) {
        *pw = *pr++;
        pw += (*pw != c);
    }
    *pw = '\0';
}

int check_for_carage_return_char(char* input) {
    printf("checking for carage return\n");
    for(int i = 0;i < strlen(input);i++) {
        if((int)input[i] == 0xD) {
            printf("carage return character detected, attempting remove...\n");
            remove_all_chars(input,0xD);
            return 0;
        }
    }
    return 0;

}

struct HTTPRequest parse_to_httpresponse(char* msg) {
    if(msg == NULL) { 
        printf("Message empty\n");
        exit(1);
    }
    remove_all_chars(msg,0xD);
    struct HTTPRequest sent_request = HttpRequest_constructor(msg);
    printf("2\n");
    /*for(int i = 0; i < sent_request.request_line.keys.length; i++) {
                char* key_name = (char*)LinkedList_retreive(
                                            &sent_request.request_line.keys,i);
                char* value = (char*)Dictionary_search(&sent_request.request_line,
                                           key_name,sizeof(strlen(key_name)));
                printf("%s: %s\n",key_name,value); 
    }
    for(int i = 0; i < sent_request.header_fields.keys.length; i++) {
                char* key_name = (char*)LinkedList_retreive(
                                            &sent_request.header_fields.keys,i);
                char* value = (char*)Dictionary_search(&sent_request.header_fields,
                                           key_name,sizeof(strlen(key_name)));
                printf("%s: %s\n",key_name,value); 
    }
    for(int i = 0; i < sent_request.body.keys.length; i++) {
                char* key_name = (char*)LinkedList_retreive(
                                            &sent_request.body.keys,i);
                char* value = (char*)Dictionary_search(&sent_request.body,
                                           key_name,sizeof(strlen(key_name)));
                printf("%s: %s\n",key_name,value); 
    }*/



    return sent_request;
}

/*int post_data(char* URL, char* Headers[],
              unsigned long Header_count, char* Data) {


    char request_line[] = "POST / HTTP/1.1\n";
    char Host[sizeof("Host: ")+strlen(URL)]; 
    char http_request_string; 

    return 0;
}*/

int post_data(char* URL , char* Headers[],
              unsigned long Header_count,char* Data, char* cert_pem) {
    //#define USE_CHUNKED
    //#define DISABLE_EXPECT
    //printf("\n\n%s\n\n",properties->Data);
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
        for(int i = 0;i<Header_count;i++) {
            header_list = curl_slist_append(header_list, 
                                            Headers[i]);
        }

        curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_SSLv3);
        curl_easy_setopt(curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_3);  
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
        curl_easy_setopt(curl, CURLOPT_SSLCERT, cert_pem);
        curl_easy_setopt(curl, CURLOPT_SSLKEY, cert_pem);
        curl_easy_setopt(curl, CURLOPT_URL, URL);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, Data);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        

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


char* read_text_file(char* file_path) {
    FILE *file_pointer;
    long File_SIZE = 0;
    char* buffer;

    file_pointer = fopen(file_path, "r");
    if(!file_pointer) {
        perror(file_path);
        exit(1);
    }
    fseek(file_pointer,0L,SEEK_END);
    File_SIZE = ftell(file_pointer)+1;
    rewind(file_pointer);

    buffer = malloc(File_SIZE);
    memset(buffer,0,File_SIZE);

    if(!buffer) {
        fclose(file_pointer);
        fputs("memory alloc failed",stderr);
        exit(1);
    }

    if(1!=fread(buffer, File_SIZE-1, 1 , file_pointer)) {
        fclose(file_pointer);
        free(buffer);
        fputs("entire read failed",stderr);
        exit(1);
    };
    fclose(file_pointer);
    return buffer;
}

void write_text_file(char* file_path, char* buffer) {
    FILE *file_pointer = fopen(file_path,"w");
    fprintf(file_pointer,"%s\n",buffer);
    fclose(file_pointer);
}
