#include <netinet/in.h>
#include <stdio.h>
#include <lua5.3/lua.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "DataStructures/Dictionary/Dictionary.h"
#include "Networking/HttpRequest.h"
#include "DataGenerators/Whatsapp/WhatsappDataGen.h"
#include "Networking/Server.h"
#include "Bot/bot.h"

char FaceBook_URL[] = "https://graph.facebook.com/v17.0/149485694924975/messages";
char Header_Autorization[] = "Authorization: Bearer EAAzl9nSKvWYBO0LY3NfQYtZC6s4UcofwzpZAnhPt0MHKoDIdF6TQGaLbe6gZBvkuFkHd42yRTtHWZAaJ870AJTOoZB9MfJHhSQJit13eLDUj19vReoaSAv4HBRPVs2nlqAbWW8QcetaodPDsYMcHMFafFNE10Nuxi1HtgtQrTE5JPMk3ksmk08y2hu76C6GkyHp7JSymZC6hObalsxdaCjBN5y6Rmg";
char Header_ContentType[] = "Content-Type: application/json";

char data_default_template[] = "{ \"messaging_product\": \"whatsapp\", \"to\": \"27769827148\", \"type\": \"template\", \"template\": { \"name\": \"bcs_first\", \"language\": { \"code\": \"en_US\" } } }";
char data_sample_text[] = "{ \"messaging_product\": \"whatsapp\",\"recipient_type\": \"individual\",\"to\": \"27769827148\",\"type\": \"text\",\"text\": { \"preview_url\": false,\"body\": \"Hello Jan-Hendrik\"}}";

int main(int argc ,char* argv[]) {
    for(int i = 0; i < argc; i++) 
        printf("argv[%d] = %s\n",i,argv[i]);
    printf("\nStarting BCS_bot..(2).\n\n");
   
    
    if(argc == 2) {
        if(strcmp(argv[1],"test") == 0) {
            char* Headers[] = {Header_Autorization,Header_ContentType};
            char* data = read_text_file("samples/json.txt");
            //printf("data == %s\n",data);
            post_data("127.0.0.1",Headers,2,data);
            free(data);
        } 
        else if(strcmp(argv[1],"server")==0) {
            struct Server server = Server_constructor(AF_INET,SOCK_STREAM,
                                                      0,INADDR_ANY,80,10);
            while(1) {
                struct HTTPRequest response = launch_listner_server(&server); 
                //printf("(Search_test) name = %s\n\n",(char*)Dictionary_search(&response.body,"contacts.profile.name",sizeof("contacts.profile.name")));
                //char* body = Dictionary_print(&response.body);
                //printf("\n\n%s\n\n",body);
                //free(body);
                HttpRequest_destructor(&response);
            }
        }
        else if(strcmp(argv[1],"parse")==0) {
            char* data = read_text_file("samples/sample.txt");
            //printf("data == %s\n",data);
            struct HTTPRequest request = parse_to_httpresponse(data);
            char *response = Dictionary_print(&request.body);
            printf("\n\n\n%s\n\n\n",response);
            free(data);
            free(response);
            HttpRequest_destructor(&request);
        }
        else if(strcmp(argv[1],"gen")==0) {
            char* textdata = generate_whatsapp_text_message("individual","27769827148","Hello Hope you are having a good day");
            
            printf("\n==========\n%s\n==========\n",textdata);
            char* templatedata = generate_whatsapp_template_message("individual","27769827148","bcs_first","en_US");
        
            printf("\n==========\n%s\n==========\n",templatedata);
        }
    }
}

