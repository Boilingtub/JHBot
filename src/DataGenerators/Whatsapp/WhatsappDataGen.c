#include "WhatsappDataGen.h"
#include "../../Parsers/cjson/cJSON.h"

cJSON* generate_whatsapp_data(char* recipient_type, char* to) {
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "messaging_product",cJSON_CreateString("whatsapp"));
    cJSON_AddItemToObject(root, "recipient_type",cJSON_CreateString(recipient_type));
    cJSON_AddItemToObject(root, "to", cJSON_CreateString(to));
    return root;
}

char * generate_whatsapp_template_message(char * recipient_type, char* to, char* name, char* language_code) {
    cJSON* root = generate_whatsapp_data(recipient_type,to);
    cJSON* template =  cJSON_CreateObject();
    cJSON_AddItemToObject(template,"name",cJSON_CreateString(name));
    
    cJSON* language = cJSON_CreateObject();
    cJSON_AddItemToObject(language,"code",cJSON_CreateString(language_code));
    cJSON_AddItemToObject(template,"language",language);

    cJSON_AddItemToObject(root,"template",template);
    return cJSON_PrintUnformatted(root);

}

char * generate_whatsapp_text_message(char* recipient_type,char* to,char* body) {
    cJSON* root = generate_whatsapp_data(recipient_type,to);
    cJSON* text =  cJSON_CreateObject(); 
    cJSON_AddItemToObject(text,"body",cJSON_CreateString(body));
    cJSON_AddItemToObject(text,"preview_url",cJSON_CreateFalse());
    cJSON_AddItemToObject(root,"text",text);
    return cJSON_PrintUnformatted(root);
}

/*
recipient_type -> group / individual
to -> group-id / phone number

*/
