#include <inttypes.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include "Whatsapp/whatsapp_bot.h"
#include "Networking/Server.h"

char FaceBook_URL[] = "https://graph.facebook.com/v17.0/149485694924975/messages";
char Header_Autorization[] = "Authorization: Bearer EAAzl9nSKvWYBO0LY3NfQYtZC6s4UcofwzpZAnhPt0MHKoDIdF6TQGaLbe6gZBvkuFkHd42yRTtHWZAaJ870AJTOoZB9MfJHhSQJit13eLDUj19vReoaSAv4HBRPVs2nlqAbWW8QcetaodPDsYMcHMFafFNE10Nuxi1HtgtQrTE5JPMk3ksmk08y2hu76C6GkyHp7JSymZC6hObalsxdaCjBN5y6Rmg";
char Header_ContentType[] = "Content-Type: application/json";

char data_default_template[] = "{ \"messaging_product\": \"whatsapp\", \"to\": \"27769827148\", \"type\": \"template\", \"template\": { \"name\": \"bcs_first\", \"language\": { \"code\": \"en_US\" } } }";
char data_sample_text[] = "{ \"messaging_product\": \"whatsapp\",\"recipient_type\": \"individual\",\"to\": \"27769827148\",\"type\": \"text\",\"text\": { \"preview_url\": false,\"body\": \"Hello Jan-Hendrik\"}}";

int main(int argc ,char* argv[]) {
    for(int i = 0; i < argc; i++) 
        printf("argv[%d] = %s\n",i,argv[i]);
    printf("\nStarting BCS_bot...\n\n");
    if(argc == 2) {
        if(strcmp(argv[1],"test") == 0) {
            printf("launching in test mode \n\n\n");
            struct WhatsappBot BCS_bot = whatsappbot_constructor("Boshoff 1",0);
            char* response_sample;
            BCS_bot.read_text_file("json.txt",&response_sample);

            BCS_bot.set_bot_curl_properties(&BCS_bot,"127.0.0.1",
                                            Header_Autorization,
                                            Header_ContentType,
                                            response_sample,
                                            sizeof(char[strlen(response_sample)]));
            BCS_bot.send_whatsapp_msg(&BCS_bot);
           
            whatsappbot_destructor(&BCS_bot); 
            return 0;
        } 
        else if(strcmp(argv[1],"server")==0) {
            struct WhatsappBot BCS_bot = whatsappbot_constructor("Serve BOT",1);
            while(1) {
                char* response = BCS_bot.webhook.launch(&BCS_bot.webhook);
                BCS_bot.parse_received_msg(&BCS_bot,response);

                printf("name = %s\n",BCS_bot.received_message.name);
                printf("wa_id = %s\n",BCS_bot.received_message.wa_id);
                printf("from = %s\n",BCS_bot.received_message.from);
                printf("id = %s\n",BCS_bot.received_message.id);
                printf("timestamp = %s\n",BCS_bot.received_message.timestamp);
                printf("type = %s\n",BCS_bot.received_message.type);
                printf("text_body = %s\n",BCS_bot.received_message.text_body);
                if(strcmp(BCS_bot.received_message.name,"Jan")==0) {
                    printf("\n\nJan sent a message\n\n");
                    break;
                }
            }
            whatsappbot_destructor(&BCS_bot);
        }
    }
    
}

