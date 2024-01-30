#ifndef BOT_FLI_PYTHON_INCLUDE
#define BOT_FLI_PYTHON_INCLUDE

#ifdef _WIN32
    #define ifwinExportdll __declspec(dllexport)
#else
    #define ifwinExportdll 
#endif

ifwinExportdll char * python_read_text_file(char* file_path);
ifwinExportdll void python_write_text_file(char* file_path, char* buffer); 
ifwinExportdll int python_post_data(char *URL, char*Headers[], unsigned long Header_count,
                     char* Data); 

ifwinExportdll int python_create_new_listner_server(int domain,int service,int protocol,
                               unsigned long face,int port,int backlog);

ifwinExportdll int python_launch_listner_server(int select,
                                                char* server_response_message);

ifwinExportdll int python_parse_httprequest(char* data); 

ifwinExportdll char* python_httprequest_search(int select, char* part ,char* field);

ifwinExportdll char* python_whatsapp_message_to_string(int message_index); 
ifwinExportdll int python_create_whatsapp_message(char* recipient_type, char* to); 
ifwinExportdll void python_Make_Template(int message_index, char* name,
                                         char* language_code);
ifwinExportdll void python_Make_Text(int message_index, char* body);
ifwinExportdll void python_Make_reply(int message_index, char* message_id);
ifwinExportdll void python_Make_interactive(int message_index, char* header, char* body,
                             char* footer, int action_index);
ifwinExportdll int python_create_action_list(char* action_name,
                                             int sections_index[],int section_count);
ifwinExportdll int python_create_section(char* section_title, char* options[], int option_count);

ifwinExportdll void python_clear_httprequests();
ifwinExportdll void python_clear_servers();
ifwinExportdll void python_clear_messages();


#endif
