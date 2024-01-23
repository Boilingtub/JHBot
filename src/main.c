#ifdef _WIN32
#include <winsock2.h>
#elif __linux__
#include <netinet/in.h>
#include <sys/socket.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DataGenerators/Whatsapp/WhatsappDataGen.h"
#include "DataStructures/Dictionary/Dictionary.h"
#include "Networking/HttpRequest.h"
#include "Networking/Server.h"
#include "Bot/bot.h"
#include "Parsers/cjson/cJSON.h"

char FaceBook_URL[] = "https://graph.facebook.com/v17.0/149485694924975/messages";
char Header_Autorization[] = "Authorization: Bearer EAAzl9nSKvWYBO6LowDSFq9PxrdznQVytZAAKGZBG0ZAJ4ZCKcdgnKubQH4ibxDrsAuVgLYnaO2gsrjpRUWYmxv65k40WZCOZBughPPoY7CYSzTWlNoZCdeiNWwsny0o5zxHDqG1B4e9uyBTKo7meZC7mvqzANOpRZAyLDJpMuWrhoHghZAyWWO2FXhM1Xqig2hPNQec20U6rk26DeZA7OFAOOfxMLZA9Vy4ZD";
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
            char* data = read_text_file("../samples/json.txt");
            //printf("data == %s\n",data);
            post_data("127.0.0.1",Headers,2,data);
            free(data);
        } 
        else if(strcmp(argv[1],"server")==0) {
            struct Server server = Server_constructor(AF_INET,SOCK_STREAM,
                                                      0,INADDR_ANY,80,10);
            while(1) {
                struct HTTPRequest response = launch_listner_server(&server); 
                
                printf("(Search_test) name = %s\n\n",(char*)Dictionary_search(&response.body,"contacts.profile.name",sizeof("contacts.profile.name")));
                char* body = Dictionary_print(&response.body);
                printf("\n\n%s\n\n",body);
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
            cJSON* message = create_whatsapp_message("individual","27769827148");
            //Add_Body(message,"This is a valued message");
            char* First_Section_options[] = {"Sec1_opt1","Sec1_opt2"}; 
            char* Second_Section_options[] = {"Sec2_opt1","Sec2_opt2"};
            cJSON* sections[] = {create_section("First_Section",First_Section_options,2),create_section("Second_Section",Second_Section_options,2)};
            cJSON* action = create_action_list("open options",sections,2);
            Make_interactive(message,"","blah blah","a",action);       
            char* Headers[] = {Header_Autorization,Header_ContentType}; 
            printf("\n\n%s\n\n",cJSON_Print(message) );
            post_data(FaceBook_URL,Headers,2,cJSON_PrintUnformatted(message));
        }
    }
}

