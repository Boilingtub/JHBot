#ifndef WHATSAPP_DATA_GEN_INCLUDE
#define WHATSAPP_DATA_GEN_INCLUDE

char * generate_whatsapp_template_message(char * recipient_type, char* to, char* name, char* language_code);
char * generate_whatsapp_text_message(char* recipient_type,char* to,char* body) ;
char*  generate_whatsapp_reply_message(char* recipient_type,char* to, char* message_id, char* body);

#endif
