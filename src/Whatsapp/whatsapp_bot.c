#include <curl/curl.h>
#include <curl/easy.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "whatsapp_bot.h"

void parse_received_msg(char* msg);
void interpret_msg(struct WhatsappBot *bot, char* msg);
int send_whatsapp_msg(struct WhatsappBot *bot);
void set_bot_curl_properties(struct WhatsappBot *bot,
                             char URL[], char Autorization[],
                             char ContentType[], char Data[],
                             unsigned long data_size);

struct WhatsappBot whatsappbot_constructor(char* name) {
    struct WhatsappBot new_bot;
    new_bot.name = name;
    new_bot.parse_received_msg = parse_received_msg;
    new_bot.interpret_msg = interpret_msg;
    new_bot.send_whatsapp_msg = send_whatsapp_msg;
    new_bot.set_bot_curl_properties = set_bot_curl_properties;
    return new_bot;
    
}
void set_bot_curl_properties(struct WhatsappBot *bot,
                             char URL[] , char Autorization[],
                             char ContentType[], char Data[],
                             unsigned long data_size) {
    strcpy(bot->whatsappcurl.URL , URL);
    strcpy(bot->whatsappcurl.Header_Autorization , Autorization);
    strcpy(bot->whatsappcurl.Header_ContentType , ContentType);
   
    free(bot->whatsappcurl.Data);
    bot->whatsappcurl.Data = malloc(data_size);
    strncpy(bot->whatsappcurl.Data ,Data, data_size);
}
void parse_received_msg(char* msg) {
    
}
void interpret_msg(struct WhatsappBot *bot, char* msg) {

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

