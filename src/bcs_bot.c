#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "Whatsapp/whatsapp_bot.h"
#include "Networking/Server.h"
#include "Networking/HttpRequest.h"

char FaceBook_URL[] = "https://graph.facebook.com/v17.0/149485694924975/messages";
char Header_Autorization[] = "Authorization: Bearer EAAzl9nSKvWYBO0LY3NfQYtZC6s4UcofwzpZAnhPt0MHKoDIdF6TQGaLbe6gZBvkuFkHd42yRTtHWZAaJ870AJTOoZB9MfJHhSQJit13eLDUj19vReoaSAv4HBRPVs2nlqAbWW8QcetaodPDsYMcHMFafFNE10Nuxi1HtgtQrTE5JPMk3ksmk08y2hu76C6GkyHp7JSymZC6hObalsxdaCjBN5y6Rmg";
char Header_ContentType[] = "Content-Type: application/json";

char data_default_template[] = "{ \"messaging_product\": \"whatsapp\", \"to\": \"27769827148\", \"type\": \"template\", \"template\": { \"name\": \"bcs_first\", \"language\": { \"code\": \"en_US\" } } }";
char data_sample_text[] = "{ \"messaging_product\": \"whatsapp\",\"recipient_type\": \"individual\",\"to\": \"27769827148\",\"type\": \"text\",\"text\": { \"preview_url\": false,\"body\": \"Hello Jan-Hendrik\"}}";

void send_test_messages(struct WhatsappBot *bot) { 
    bot->send_whatsapp_msg(bot);
}

void launch_web_hook(struct Server *server) {
    #define buffer_size 30000
    char buffer[buffer_size];
    int address_length = sizeof(server->address);
    int new_socket;

    while(1) {
        printf("\n===== WAITING FOR CONNECTION =====\n");

        new_socket = accept(server->socket, (struct sockaddr*)&server->address,
                            (socklen_t*)&address_length);
        if(new_socket > 0)
            printf("\n===== CONNECTION SUCCESS =====\n");
        read(new_socket , buffer , buffer_size);

        printf("\n\n\n%s\n\n\n",buffer);

        //struct HTTPRequest sent_request = http_request_constructor(buffer);
        
        

        //parse message out
        //interprete message
        //send_message
        //reset whatsapp_bot

        close(new_socket);
    }
}

int main(int argc ,char* argv[]) {
    for(int i = 0; i < argc; i++) 
        printf("argv[%d] = %s\n",i,argv[i]);

    printf("\nStarting BCS_bot...\n\n");

    if(argc == 2)
        if(strcmp(argv[1],"test") == 0) {
            printf("launching in test mode \n\n\n");
            struct WhatsappBot BCS_bot = whatsappbot_constructor("Boshoff 1");
            BCS_bot.set_bot_curl_properties(&BCS_bot,FaceBook_URL,
                                            Header_Autorization,
                                            Header_ContentType,
                                            data_sample_text,
                                            sizeof(data_sample_text));
            send_test_messages(&BCS_bot);
            return 0;
        } 
    
    struct Server server = server_constructor(AF_INET, SOCK_STREAM, 0,
                                              INADDR_ANY, 80, 10,
                                              launch_web_hook);
    server.launch(&server);
    return 0;
    
}

