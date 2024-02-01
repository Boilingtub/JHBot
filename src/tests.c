#include "DataStructures/Lists/LinkedList.h"
#include "FLI/Python/python_fli.h"
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

char certificate_path[] = "/home/hendrik/ca-certificates/cert.pem";
char key_path[] = "/home/hendrik/ca-certificates/key.pem";

int main(int argc ,char* argv[]) {
    for(int i = 0; i < argc; i++) 
        printf("argv[%d] = %s\n",i,argv[i]);
    printf("\nStarting BCS_bot..(2).\n\n");
   
    
    if(argc == 2) {
        if(strcmp(argv[1],"send") == 0) {
            char* Headers[] = {Header_Autorization,Header_ContentType};
            char* data = read_text_file("../samples/json.txt");
            //printf("data == %s\n",data);
            post_data("127.0.0.1",Headers,2,data);
            free(data);
        } 
        else if(strcmp(argv[1],"server")==0) {
            printf("cert path : %s\n",certificate_path);
            struct SSL_Server server = SSL_Server_constructor(AF_INET,SOCK_STREAM,
                                                      0,INADDR_ANY,80,10,certificate_path,key_path);
            while(1) {
                char* buff = launch_ssl_listner_server(&server,"server_test"); 
                struct HTTPRequest response = parse_to_httpresponse(buff);
                free(buff);

                printf("(Search_test) name = %s\n\n",(char*)Dictionary_search(
                    &response.body,"contacts.profile.name",
                    sizeof("contacts.profile.name")));
                char* body = Dictionary_print(&response.body);
                printf("\n\n%s\n\n",body);
                free(body);
                HttpRequest_destructor(&response);
            }
        }
        else if(strcmp(argv[1],"parse")==0) {
            char* data = read_text_file("../samples/sample.txt");
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
            cJSON* sections[] = {create_section("First_Section",
                                                First_Section_options,2),
                create_section("Second_Section",Second_Section_options,2)};
            
            cJSON* action = create_action_list("open options",sections,2);
            Make_interactive(message,"","blah blah","a",action);       
            char* Headers[] = {Header_Autorization,Header_ContentType}; 
            //printf("\n\n%s\n\n",cJSON_Print(message) );
            char* data = cJSON_PrintUnformatted(message);
            post_data("127.0.0.1",Headers,2,data);
            free(data);
            //cJSON_free(action);
            //cJSON_free(sections[0]);
            //cJSON_free(sections[1]);
            cJSON_Delete(message);

        }
        else if(strcmp(argv[1],"memtest")==0) {
            struct LinkedList ll = LinkedList_constructor(); 
            LinkedList_insert(&ll,ll.length,"value1",7);
            LinkedList_insert(&ll,ll.length,"value2",7);
            LinkedList_insert(&ll,ll.length,"value3",7); 
            LinkedList_insert(&ll,ll.length,"value4",7);
            LinkedList_insert(&ll,ll.length,"value5",7);
            LinkedList_insert(&ll,ll.length,"value6",7);
            for(int i = 0; i < ll.length;i++) {
                char* ll_value = (char*)LinkedList_retreive(&ll,i);
                printf("%s\n",ll_value);
            }
            LinkedList_destructor(&ll,NULL);
            struct Dictionary dict = Dictionary_constructor(NULL);
            Dictionary_insert(&dict,"key1",5,"value1",7);
            Dictionary_insert(&dict,"key2",5,"value2",7);
            Dictionary_insert(&dict,"key3",5,"value3",7);
            Dictionary_insert(&dict,"key4",7,"value4",9);
            Dictionary_insert(&dict,"key5",7,"value5",9);
            Dictionary_insert(&dict,"key6",7,"value6",9);
            Dictionary_insert(&dict,"key7",7,"value7",9);
            Dictionary_insert(&dict,"key8",7,"value8",9);
            Dictionary_insert(&dict,"key9",7,"value9",9);
            Dictionary_insert(&dict,"key10",7,"value10",9);
            char* dict_print = Dictionary_print(&dict);
            printf("\n%s\n",dict_print);
            Dictionary_destructor(&dict);
            free(dict_print);

            
        }
        else if(strcmp(argv[1],"invalid") == 0) {
            char* data = read_text_file("./../samples/invalid_sample");
            printf("data : \n%s\n",data);
            struct HTTPRequest request = parse_to_httpresponse(data);
            
            char* request_line = Dictionary_print(&request.request_line);
            char* header_fields = Dictionary_print(&request.header_fields);
            char* body = Dictionary_print(&request.body);

            printf("request_line : \n%s\n",request_line);
            printf("header_fields : \n%s\n",header_fields);
            printf("body : \n%s\n",body);
            
            free(request_line);
            free(header_fields);
            free(body);
            free(data);
            HttpRequest_destructor(&request);
        }
        else if (strcmp(argv[1],"FLI-test")==0) {
            int listner_server = python_create_new_listner_server(
                AF_INET,SOCK_STREAM,0,INADDR_ANY,80,10);
            python_launch_listner_server(listner_server,"FLI-test");
            python_clear_servers();
  
        }
        else if(strcmp(argv[1],"FLI-server") == 0) {

            int listner_server = python_create_new_ssl_listner_server(
                AF_INET,SOCK_STREAM,0,INADDR_ANY,80,10,certificate_path,key_path);
            int i = 0;
            while(i < 1) {
                char* http_response = python_launch_ssl_listner_server(listner_server,"FLI-server"); 
                
                //int response = python_parse_httprequest(http_response);
                //char* request_data = python_read_text_file("../samples/sample.txt");
                //char* request_data = python_read_text_file("accepted_message.txt");
                //int request = python_parse_httprequest(request_data);
 
                //char *response = python_httprequest_search(http_response,"dfsc","");
                //printf("\n\n\n%s\n\n\n",response);
                //free(response);

                //free(request_data);
                free(http_response);
                python_clear_httprequests();
                i++;
                
            }
            python_clear_servers();
            
        }
        else if(strcmp(argv[1],"FLI-send") == 0) {
            char* Headers[] = {Header_Autorization,Header_ContentType};
            int message = python_create_whatsapp_message("individual","27769827148");
            char* Section1_Options[] = {"Sec1Opt1","Sec1Opt2"};
            char* Section2_Options[] = {"Sec2Opt1","Sec2Opt2"};
            int Section1 = python_create_section("Section1",Section1_Options,2);
            int Section2 = python_create_section("Section2",Section2_Options,2);
            int Sections[] = {Section1,Section2};
            int actions = python_create_action_list("quick reply",Sections,2);
            python_Make_interactive(message,"head","body","foot",actions);
            //printf("MADE INTERACTIVE\n");
            char* data = python_whatsapp_message_to_string(message);
            //printf("\ndata = %s\n",data);
            post_data("127.0.0.1",Headers,2,data);
            free(data);
            python_clear_messages();
        
        }
        else if(strcmp(argv[1],"memtest-server") == 0) {
            struct Server server = Server_constructor(AF_INET,SOCK_STREAM,
                                                      0,INADDR_ANY,80,10);
            while(1) {
                
                char* buff = launch_listner_server(&server,"memtest-server");  
                
                struct HTTPRequest response = parse_to_httpresponse(buff);
                free(buff);

                char* dictvalues = Dictionary_print(&response.body);
                printf("\n%s\n",dictvalues);
                free(dictvalues);
                HttpRequest_destructor(&response);
                break;
            }
        }
        else if(strcmp(argv[1],"memtest-send") == 0) {
            char* Headers[] = {Header_Autorization,Header_ContentType};
            char* data = read_text_file("../samples/json.txt");
            for(int i = 0;i < 1; i++) {
                post_data("127.0.0.1",Headers,2,data);
            }
            free(data);

        }
    }
}

