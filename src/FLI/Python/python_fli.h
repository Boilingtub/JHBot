#ifndef BOT_FLI_PYTHON_INCLUDE
#define BOT_FLI_PYTHON_INCLUDE

void python_initialize_bot();
int python_create_new_listner_server(int domain,int service,int protocol,
                               unsigned long face,int port,int backlog);
int python_launch_listner_server(int select);
int python_parse_httprequest(char* data); 
char* python_httprequest_search(int select, char* part ,char* search);

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

#endif
