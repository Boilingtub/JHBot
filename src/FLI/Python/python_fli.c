#include <stdio.h>
#include <string.h>
#include "../../Bot/bot.h"
#include "../../Networking//HttpRequest.h"
#include "../../DataStructures/Lists/LinkedList.h"
#include "../../DataGenerators/Whatsapp/WhatsappDataGen.h"

char * python_read_text_file(char* file_path);
int python_post_data(char *URL, char*Headers[], unsigned long Header_count,
                     char* Data); 

void python_initialize_bot();
int python_create_new_listner_server(int domain,int service,int protocol,
                               unsigned long face,int port,int backlog);
int python_launch_listner_server(int select);
int python_parse_httprequest(char* data); 
char* python_httprequest_search(int select, char* part ,char* field);

char* python_whatsapp_message_to_string(int message_index); 
int python_create_whatsapp_message(char* recipient_type, char* to); 
void python_Make_Template(int message_index, char* name, char* language_code);
void python_Make_Text(int message_index, char* body);
void python_Make_reply(int message_index, char* message_id);
void python_Make_interactive(int message_index, char* header, char* body,
                             char* footer, int action_index);
int python_create_action_list(char* action_name, int sections_index[],int section_count);
int python_create_section(char* section_title, char* options[], int option_count);

void python_clear_httprequests();
void python_clear_server();
void python_clear_Json();

struct LinkedList server_list;
struct LinkedList HttpRequest_list;
struct LinkedList JsonObj_list;

char* python_read_text_file(char* file_path) {
    return read_text_file(file_path);
}

int python_post_data(char *URL, char*Headers[], unsigned long Header_count,
                     char* Data) {
    return post_data(URL,Headers,Header_count,Data);
}

void python_initialize_bot() {
    server_list = LinkedList_constructor();
    HttpRequest_list = LinkedList_constructor();
    JsonObj_list = LinkedList_constructor();
}

int python_create_new_listner_server(int domain,int service,int protocol,
                               unsigned long face,int port,int backlog) {
     
    struct Server new_server = Server_constructor(domain,service,protocol,
                                       face,port,backlog);
    LinkedList_insert(&server_list,server_list.length,&new_server,sizeof(new_server)+2);
    return server_list.length-1;
}

int python_launch_listner_server(int select) {
    struct Server selected_server = *(struct Server*)(LinkedList_retreive(&server_list,select));
    struct HTTPRequest response = launch_listner_server(&selected_server);
    LinkedList_insert(&HttpRequest_list,HttpRequest_list.length,&response,sizeof(response)+2);
    return HttpRequest_list.length-1; 
}

int python_parse_httprequest(char* data) {
    struct HTTPRequest response = parse_to_httpresponse(data);
    LinkedList_insert(&HttpRequest_list,HttpRequest_list.length,&response,sizeof(response)+2);
    return HttpRequest_list.length-1; 
}

//NOT EXPORTED
char* python_return_dict_or_key(struct Dictionary dictionary, char* field) {
    char* buf = Dictionary_search(&dictionary,field,sizeof(char[strlen(field)])); 
    if(buf == NULL) {
        printf("Could not find selected field !\nThese are all available fields for this part\n");
        printf("\n%s\n", Dictionary_print(&dictionary));
        return NULL;
    }
    return buf;
}

char* python_httprequest_search(int select, char* part ,char* field) {
    struct HTTPRequest request = *(struct HTTPRequest*)LinkedList_retreive(&HttpRequest_list,select);
    if(strcmp(part,"request_line") == 0) {
        return python_return_dict_or_key(request.request_line, field);
    }
    else if(strcmp(part,"header_fields") == 0) {
        return python_return_dict_or_key(request.header_fields, field);
    }
    else if(strcmp(part,"body") == 0) {
        return python_return_dict_or_key(request.body, field);
    }
    else {
        printf("Invalid Part\nAn HTTPRequest only as 3 parts\n  -> request_line\n  -> header_fields\n  -> body\n");
        return NULL;
    }
}

//=====

char* python_whatsapp_message_to_string(int message_index) {
    return whatsapp_message_to_string(LinkedList_retreive(&JsonObj_list,
                                                          message_index));
}

int python_create_whatsapp_message(char* recipient_type, char* to) {
    cJSON* new_whatsapp_message = create_whatsapp_message(recipient_type,to);
    LinkedList_insert(&JsonObj_list,JsonObj_list.length,new_whatsapp_message,
                      sizeof(*new_whatsapp_message)+2);
    return JsonObj_list.length-1;
}

void python_Make_Template(int message_index, char* name, char* language_code) {
    cJSON* message = LinkedList_retreive(&JsonObj_list,message_index);
    Make_Template(message,name,language_code); 
}
void python_Make_Text(int message_index, char* body) {
    cJSON* message = LinkedList_retreive(&JsonObj_list,message_index);
    Make_Text(message,body);
}

void python_Make_reply(int message_index, char* message_id) {
    cJSON* message = LinkedList_retreive(&JsonObj_list,message_index);
    Make_reply(message, message_id);
}

void python_Make_interactive(int message_index, char* header, char* body,
                             char* footer, int action_index) {
    cJSON* message = LinkedList_retreive(&JsonObj_list,message_index);
    cJSON* action = LinkedList_retreive(&JsonObj_list,action_index);
    Make_interactive(message,header,body,footer,action);
}

int python_create_action_list(char* action_name, int sections_index[],int section_count) {
    cJSON* sections[section_count];
    for(int i = 0;i<section_count;i++){
       sections[i] = LinkedList_retreive(&JsonObj_list,sections_index[i]); 
    }
    cJSON* action_list = create_action_list(action_name,sections,section_count);
    LinkedList_insert(&JsonObj_list,JsonObj_list.length,action_list,
                      sizeof(*action_list));
    return JsonObj_list.length-1;
}

int python_create_section(char* section_title, char* options[], int option_count) {
    cJSON* section = create_section(section_title,options,option_count);
    LinkedList_insert(&JsonObj_list,JsonObj_list.length,section,
                      sizeof(*section)+2);
    return JsonObj_list.length-1;
}

//=====

void python_clear_httprequests() {
    LinkedList_destructor(&HttpRequest_list);
    HttpRequest_list = LinkedList_constructor();
}

void python_clear_servers() {
    LinkedList_destructor(&server_list);
    server_list = LinkedList_constructor();
}

void python_clear_Json() {
    LinkedList_destructor(&JsonObj_list);
    JsonObj_list = LinkedList_constructor();
    
}

