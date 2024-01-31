#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../Bot/bot.h"
#include "../../Networking//HttpRequest.h"
#include "../../DataGenerators/Whatsapp/WhatsappDataGen.h"

#ifdef _WIN32
    #define ifwinExportdll __declspec(dllexport)
#else
    #define ifwinExportdll 
#endif

#define UNUSED_INDEX -2000000000

struct ServerList {
    struct Server* servers;
    int length;
};
void ServerList_append(struct ServerList *list, struct Server item_to_append) {
    if(list->length == 0) {
        list->servers = (struct Server*) malloc(list->length*sizeof(struct Server));
    }
    list->length += 1;
    list->servers = (struct Server*) realloc(list->servers, list->length*sizeof(struct Server));
    list->servers[list->length-1] = item_to_append;
}
struct ServerList ServerList_constructor() {
    struct ServerList new_server_list;
    new_server_list.length = 0;
    return new_server_list;
}

struct ServerList server_list;
//====
struct SSL_ServerList {
    struct SSL_Server* servers;
    int length;
};
void SSL_ServerList_append(struct SSL_ServerList *list, struct SSL_Server item_to_append) {
    if(list->length == 0) {
        list->servers = (struct SSL_Server*) malloc(list->length*sizeof(struct Server));
    }
    list->length += 1;
    list->servers = (struct SSL_Server*) realloc(list->servers,
                                                 list->length*sizeof(struct SSL_Server));
    list->servers[list->length-1] = item_to_append;
}
struct SSL_ServerList SSL_ServerList_constructor() {
    struct SSL_ServerList new_server_list;
    new_server_list.length = 0;
    return new_server_list;
}

struct SSL_ServerList ssl_server_list;
//====

struct HttpRequestList {
    struct HTTPRequest* requests;
    int length;
};
void HttpRequestList_append(struct HttpRequestList *list, struct HTTPRequest item_to_append) {
    if(list->length == 0) {
        list->requests = (struct HTTPRequest*) malloc(list->length*sizeof(struct HTTPRequest));
    }
    list->length += 1;
    list->requests = (struct HTTPRequest*) realloc(list->requests, list->length*sizeof(struct HTTPRequest));
    list->requests[list->length-1] = item_to_append;
}
struct HttpRequestList HTTPRequestList_constructor() {
    struct HttpRequestList new_httprequest_list;
    new_httprequest_list.length = 0;
    return new_httprequest_list;
}

struct HttpRequestList HttpRequest_list;
//====
struct cJsonObjList {
    cJSON** objs;
    int length;
};

struct cJsonObjList cJsonObjList_constructor() {
    struct cJsonObjList new_message_list;
    new_message_list.length = 0;
    return new_message_list;
}
void cJsonObjList_append(struct cJsonObjList *list, cJSON* item_to_append) {
    if(list->length == 0) {
        list->objs = (cJSON**) malloc(list->length*sizeof(cJSON*));
    }
    list->length += 1;
    list->objs = (cJSON**) realloc(list->objs, list->length*sizeof(cJSON*));
    list->objs[list->length-1] = item_to_append;
}

struct cJsonObjList message_list;


int* JsonObjects_to_free; 
int JsonObjects_to_free_length = 0;

void addJsonObj_to_freeList(int index_to_add) {
    if(JsonObjects_to_free_length == 0) {
         JsonObjects_to_free = malloc(0);
    }
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

ifwinExportdll void python_write_text_file(char* file_path, char* buffer) {
    write_text_file(file_path, buffer);
}

ifwinExportdll int python_post_data(char *URL, char*Headers[], unsigned long Header_count,
                     char* Data) {
    return post_data(URL,Headers,Header_count,Data);
}

ifwinExportdll int python_create_new_listner_server(int domain,int service,int protocol,
                               unsigned long face,int port,int backlog) { 
    ServerList_append(&server_list,Server_constructor(domain,service,protocol,
                                         face,port,backlog));
    return server_list.length-1;
}

ifwinExportdll int python_create_new_ssl_listner_server(int domain,int service,int protocol,
                               unsigned long face,int port,int backlog,
                               char* cert_pem, char* key_pem) { 
    SSL_ServerList_append(&ssl_server_list,SSL_Server_constructor(domain,service,protocol,
                                         face,port,backlog,cert_pem,key_pem));
    return ssl_server_list.length-1;
}

ifwinExportdll int python_launch_listner_server(int select, char* server_response_message) {
    struct Server selected_server = server_list.servers[select]; 
    HttpRequestList_append(&HttpRequest_list,
                           launch_listner_server(&selected_server,
                                                 server_response_message));
    return HttpRequest_list.length-1; 
}

ifwinExportdll int python_launch_ssl_listner_server(int select, char* server_response_message) {
    struct SSL_Server selected_server = ssl_server_list.servers[select]; 
    HttpRequestList_append(&HttpRequest_list,
                           launch_ssl_listner_server(&selected_server,
                                                 server_response_message));
    return HttpRequest_list.length-1; 
}

ifwinExportdll int python_parse_httprequest(char* data) {
    HttpRequestList_append(&HttpRequest_list,parse_to_httpresponse(data));
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
    struct HTTPRequest request = HttpRequest_list.requests[select];
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
ifwinExportdll void python_clear_httprequests() {   
    for(int i = 0;i < HttpRequest_list.length;i++) {
        HttpRequest_destructor(&HttpRequest_list.requests[i]);
    }
    HttpRequest_list.length = 0;
}

ifwinExportdll void python_clear_servers() {
    for(int i = 0;i < server_list.length;i++) {
        free(&server_list.servers[i]);
    }
    for(int i = 0;i < ssl_server_list.length;i++) {
        DestroySSL(&ssl_server_list.servers[i]);
    }

    ssl_server_list.length = 0; 
    server_list.length = 0;
}

ifwinExportdll void python_clear_messages() {
    for(int i = 0;i < JsonObjects_to_free_length;i++) {
       if(!(UNUSED_INDEX == JsonObjects_to_free[i]))
            cJSON_Delete(message_list.objs[JsonObjects_to_free[i]]);
    }
    free(message_list.objs);
    free(JsonObjects_to_free);
    JsonObjects_to_free_length = 0;
    message_list.length = 0;
}


