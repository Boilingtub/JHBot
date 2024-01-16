#ifndef BOT_FLI_PYTHON_INCLUDE
#define BOT_FLI_PYTHON_INCLUDE

void python_initialize_bot();
int python_create_new_listner_server(int domain,int service,int protocol,
                               unsigned long interface,int port,int backlog);
int python_launch_listner_server(int select);
int python_parse_httprequest(char* data); 
char* python_httprequest_search(int select, char* part ,char* search);
void python_clear_httprequests();
void python_clear_server();

#endif
