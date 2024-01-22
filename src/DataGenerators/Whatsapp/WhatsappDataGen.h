#ifndef WHATSAPP_DATA_GEN_INCLUDE
#define WHATSAPP_DATA_GEN_INCLUDE
#include "../../Parsers/cjson/cJSON.h"
char* whatsapp_message_to_string(cJSON* message);
cJSON* create_whatsapp_message(char* recipient_type, char* to);
int Make_Template(cJSON* message ,char* name , char* language_code);
int Make_Text(cJSON* message , char* body); 
int Make_reply(cJSON* message , char* msg_id);
int Make_interactive(cJSON* message,char* header,char*body, char* footer,
                     cJSON* action);
    cJSON* create_action_list(char* action_name, cJSON* sections[], int section_count);
    cJSON* create_section(char* section_title, char* options[], int option_count);
#endif
