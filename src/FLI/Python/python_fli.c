#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../Bot/bot.h"
#include "../../Networking//HttpRequest.h"
#include "../../DataStructures/Lists/LinkedList.h"
#include "../../DataGenerators/Whatsapp/WhatsappDataGen.h"

#ifdef _WIN32
    #define ifwinExportdll __declspec(dllexport)
#else
    #define ifwinExportdll 
#endif

#define UNUSED_INDEX -2000000000
/*
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
void python_clear_servers();
void python_clear_Json();
*/
struct LinkedList server_list;
struct LinkedList HttpRequest_list;

struct cJsonObjList {
    cJSON** objs;
    int length;
};

struct cJsonObjList cJsonObjList_constructor() {
    struct cJsonObjList new_message_list;
    new_message_list.length = 0;
    new_message_list.objs = (cJSON**) malloc(new_message_list.length*sizeof(cJSON*));
    return new_message_list;
}
void cJsonObjList_append(struct cJsonObjList *list, cJSON* item_to_append) {
    list->length += 1;
    list->objs = (cJSON**) realloc(list->objs, list->length*sizeof(cJSON*));
    list->objs[list->length-1] = item_to_append;
}

struct cJsonObjList message_list;



int* JsonObjects_to_free; 
int JsonObjects_to_free_length;

void addJsonObj_to_freeList(int index_to_add) {
    JsonObjects_to_free_length += 1;
    JsonObjects_to_free = realloc(JsonObjects_to_free,
                                 JsonObjects_to_free_length*sizeof(int));
    JsonObjects_to_free[JsonObjects_to_free_length-1] = index_to_add;
}

void removeJsonObj_form_freeList(int index_to_remove) {
    JsonObjects_to_free[index_to_remove] = UNUSED_INDEX; 
}

//===========Start Of exposed Functions==========

ifwinExportdll char* python_read_text_file(char* file_path) {
    return read_text_file(file_path);
}

ifwinExportdll int python_post_data(char *URL, char*Headers[], unsigned long Header_count,
                     char* Data) {
    return post_data(URL,Headers,Header_count,Data);
}

ifwinExportdll void python_initialize_bot() {
    server_list = LinkedList_constructor();
    HttpRequest_list = LinkedList_constructor();
    message_list = cJsonObjList_constructor();
    JsonObjects_to_free = malloc(0);
}

ifwinExportdll int python_create_new_listner_server(int domain,int service,int protocol,
                               unsigned long face,int port,int backlog) {
     
    struct Server new_server = Server_constructor(domain,service,protocol,
                                       face,port,backlog);
    LinkedList_insert(&server_list,server_list.length,&new_server,sizeof(new_server));
    return server_list.length-1;
}

ifwinExportdll int python_launch_listner_server(int select) {
    struct Server selected_server = *(struct Server*)(LinkedList_retreive(&server_list,select));
    struct HTTPRequest response = launch_listner_server(&selected_server);
    LinkedList_insert(&HttpRequest_list,HttpRequest_list.length,&response,sizeof(response));
    return HttpRequest_list.length-1; 
}

ifwinExportdll int python_parse_httprequest(char* data) {
    struct HTTPRequest response = parse_to_httpresponse(data);
    LinkedList_insert(&HttpRequest_list,HttpRequest_list.length,&response,sizeof(response));
    return HttpRequest_list.length-1; 
}

//NOT EXPORTED
char* python_return_dict_or_key(struct Dictionary dictionary, char* field) {
    char* buf = Dictionary_search(&dictionary,field,sizeof(char[strlen(field)])); 
    if(buf == NULL) {
        printf("Could not find selected field !\nThese are all available fields for this part\n");
        char* dict_print_string = Dictionary_print(&dictionary);
        printf("\n%s\n", dict_print_string);
        free(dict_print_string);
        return NULL;
    }
    return buf;
}

ifwinExportdll char* python_httprequest_search(int select, char* part ,char* field) {
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

ifwinExportdll char* python_whatsapp_message_to_string(int message_index) {
    return whatsapp_message_to_string(message_list.objs[message_index]);
}

ifwinExportdll int python_create_whatsapp_message(char* recipient_type, char* to) {
    cJSON* new_whatsapp_message = create_whatsapp_message(recipient_type,to);
    cJsonObjList_append(&message_list,new_whatsapp_message);
    addJsonObj_to_freeList(message_list.length-1);
    return message_list.length-1;
}

ifwinExportdll void python_Make_Template(int message_index, char* name, char* language_code) {
    cJSON* message = message_list.objs[message_index];
    Make_Template(message,name,language_code); 
}
ifwinExportdll void python_Make_Text(int message_index, char* body) {
    cJSON* message = message_list.objs[message_index];
    Make_Text(message,body);
}

ifwinExportdll void python_Make_reply(int message_index, char* message_id) {
    cJSON* message = message_list.objs[message_index];
    Make_reply(message, message_id);
}

ifwinExportdll void python_Make_interactive(int message_index, char* header, char* body,
                             char* footer, int action_index) {
    cJSON* message = message_list.objs[message_index];
    cJSON* action = message_list.objs[action_index];
    removeJsonObj_form_freeList(action_index);
    Make_interactive(message,header,body,footer,action);
}

ifwinExportdll int python_create_action_list(char* action_name, int sections_index[],int section_count) {
    cJSON* sections[section_count];
    for(int i = 0;i<section_count;i++){
        sections[i] = message_list.objs[sections_index[i]];
        removeJsonObj_form_freeList(sections_index[i]);
    }
    cJSON* action_list = create_action_list(action_name,sections,section_count);
    cJsonObjList_append(&message_list,action_list);

    addJsonObj_to_freeList(message_list.length-1);
    return message_list.length-1;
}

ifwinExportdll int python_create_section(char* section_title, char* options[], int option_count) {
    cJSON* section = create_section(section_title,options,option_count);
    cJsonObjList_append(&message_list,section);

    addJsonObj_to_freeList(message_list.length-1);
    return message_list.length-1;
}

//=====

void LinkedList_HttpRequest_destructor(struct Node *node) {
    HttpRequest_destructor((struct HTTPRequest*)node->data);
    free(node);
}

void LinkedList_cJSON_destructor(struct Node *node) {
    cJSON_Delete((cJSON*)node->data);
    free(node);
}

ifwinExportdll void python_clear_httprequests() {    
    LinkedList_destructor(&HttpRequest_list,LinkedList_HttpRequest_destructor); 
    HttpRequest_list = LinkedList_constructor();
}

ifwinExportdll void python_clear_servers() {
    LinkedList_destructor(&server_list,NULL);
    server_list = LinkedList_constructor();
}

ifwinExportdll void python_clear_Json() {
    for(int i = 0;i < JsonObjects_to_free_length;i++) {
       if(!(UNUSED_INDEX == JsonObjects_to_free[i]))
            cJSON_Delete(message_list.objs[JsonObjects_to_free[i]]);
    }
    free(message_list.objs);
    free(JsonObjects_to_free);
    JsonObjects_to_free_length = 0;
    JsonObjects_to_free = malloc(0);
    message_list = cJsonObjList_constructor();   
}


