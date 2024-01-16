#include <stdio.h>
#include <string.h>
#include "../../Bot/bot.h"
#include "../../Networking//HttpRequest.h"
#include "../../DataStructures/Lists/LinkedList.h"

void python_initialize_bot();
int python_create_new_listner_server(int domain,int service,int protocol,
                               unsigned long interface,int port,int backlog);
int python_launch_listner_server(int select);
int python_parse_httprequest(char* data); 
char* python_httprequest_search(int select, char* part ,char* field);
void python_clear_httprequests();
void python_clear_server();

struct LinkedList server_list;
struct LinkedList HttpRequest_list;

void python_initialize_bot() {
    server_list = LinkedList_constructor();
    HttpRequest_list = LinkedList_constructor();
}

int python_create_new_listner_server(int domain,int service,int protocol,
                               unsigned long interface,int port,int backlog) {
     
    struct Server new_server = Server_constructor(domain,service,protocol,
                                       interface,port,backlog);
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

void python_clear_httprequests() {
    LinkedList_destructor(&HttpRequest_list);
    HttpRequest_list = LinkedList_constructor();
}

void python_clear_servers() {
    LinkedList_destructor(&server_list);
    server_list = LinkedList_constructor();
}

