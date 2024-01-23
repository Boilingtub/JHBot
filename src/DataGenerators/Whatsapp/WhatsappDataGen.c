#include "WhatsappDataGen.h"
#include <string.h>
#include <stdio.h>
char* haspart(cJSON* message, char* part) {    
    cJSON* item;
    cJSON_ArrayForEach(item , message) {
        if(strcmp(item->string,part) == 0) {
            return item->valuestring;
        }
    }
    return NULL;

}

char* whatsapp_message_to_string(cJSON* message) {
    return cJSON_PrintUnformatted(message);
}

cJSON* create_whatsapp_message(char* recipient_type, char* to) {
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "messaging_product",cJSON_CreateString("whatsapp"));
    cJSON_AddItemToObject(root, "recipient_type",cJSON_CreateString(recipient_type));
    cJSON_AddItemToObject(root, "to", cJSON_CreateString(to));
    return root;
}

int Make_Template(cJSON* message ,char* name , char* language_code) { 
    char* type = haspart(message,"type");
    if(type) {
        printf("\nmessage is of type %s , Create a new message to make template type\n",
               type);
        return 1;
    }
    cJSON_AddItemToObject(message,"type",cJSON_CreateString("template"));
    cJSON* template =  cJSON_CreateObject();
    cJSON_AddItemToObject(template,"name",cJSON_CreateString(name));
    
    cJSON* language = cJSON_CreateObject();
    cJSON_AddItemToObject(language,"code",cJSON_CreateString(language_code));
    cJSON_AddItemToObject(template,"language",language);
    cJSON_AddItemToObject(message,"template",template);
    return 0;

}

int Make_Text(cJSON* message , char* body) { 
    char* type = haspart(message,"type");
    if(type) {
        printf("\nmessage is of type %s , Create a new message to make text type\n",
               type);
        return 1;
    }

    cJSON_AddItemToObject(message,"type",cJSON_CreateString("text"));
    cJSON* text =  cJSON_CreateObject();
    cJSON_AddItemToObject(text,"body",cJSON_CreateString(body));
    cJSON_AddItemToObject(message,"text",text);
    return 0;
}


int Make_reply(cJSON* message , char* msg_id) {
    if(haspart(message,"context")) {
        printf("already a reply message\n");
        return 1;
    }
    cJSON* context =  cJSON_CreateObject();
    cJSON_AddItemToObject(context,"message_id",cJSON_CreateString(msg_id));
    cJSON_AddItemToObject(message,"context",context);
    return 0;
}

//============================================================================
int Add_Header(cJSON* message, char* header_text) {
    if(haspart(message,"header")) {
        printf("already has header\n");
        return 1;
    }
    cJSON * header = cJSON_CreateObject();
    cJSON_AddItemToObject(header,"type",cJSON_CreateString("text"));
    cJSON_AddItemToObject(header,"text",cJSON_CreateString(header_text));
    cJSON_AddItemToObject(message,"header",header);
    return 0;
}

int Add_Body(cJSON* message, char* body_text) {
    if(haspart(message,"body")) {
        printf("already has body\n");
        return 1;
    }
    cJSON * body = cJSON_CreateObject();
    cJSON_AddItemToObject(body,"text",cJSON_CreateString(body_text));
    cJSON_AddItemToObject(message,"body",body);
    return 0;

}

int Add_Footer(cJSON* message, char* footer_text) {
    if(haspart(message,"footer")) {
        printf("already a reply message\n");
        return 1;
    }
    cJSON * footer = cJSON_CreateObject();
    cJSON_AddItemToObject(footer,"text",cJSON_CreateString(footer_text));
    cJSON_AddItemToObject(message,"footer",footer);
    return 0;
}

int Make_interactive(cJSON* message,char* header,char*body, char* footer,
                     cJSON* action) {
    char* type = haspart(message,"type");
    if(type) {
        printf("\nmessage is of type %s , Create a new message to make interactive type\n",
               type);
        return 1;
    }

    
    cJSON_AddItemToObject(message,"type",cJSON_CreateString("interactive"));
    cJSON* interactive_part = cJSON_CreateObject();
    cJSON_AddItemToObject(interactive_part,"type",cJSON_CreateString("list"));
    
    if(strlen(header) > 0)
        Add_Header(interactive_part,header);
    if(strlen(body) > 0)
        Add_Body(interactive_part,body);
    else
        printf("\n\ncannot post message without a Body\n\n");
    if(strlen(footer) > 0)
        Add_Footer(interactive_part,footer);


    cJSON_AddItemToObject(interactive_part,"action",action);
    cJSON_AddItemToObject(message,"interactive",interactive_part);
    return 0;
}

cJSON* create_action_list(char* action_name, cJSON* sections[], int section_count) {
    cJSON* action_part = cJSON_CreateObject(); 
    cJSON_AddItemToObject(action_part,"button",cJSON_CreateString(action_name));
    cJSON* sections_arr = cJSON_CreateArray();
    for(int i = 0; i < section_count;i++) {
        cJSON_AddItemToArray(sections_arr,sections[i]);
    }
    cJSON_AddItemToObject(action_part,"sections",sections_arr);
    return action_part;
}

cJSON* create_section(char* section_title, char* options[], int option_count) {
    cJSON* section = cJSON_CreateObject();
    cJSON_AddItemToObject(section,"title",cJSON_CreateString(section_title));
    cJSON * rows = cJSON_CreateArray();
    for(int i = 0;i < option_count; i++) {
        cJSON* row_item = cJSON_CreateObject(); 
        char id[32];
        sprintf(id,"BUTTON_ID_%d",i);
        cJSON_AddItemToObject(row_item,"id",cJSON_CreateString(id));
        cJSON_AddItemToObject(row_item,"title",cJSON_CreateString(options[i]));
        cJSON_AddItemToArray(rows,row_item);
    }
    cJSON_AddItemToObject(section,"rows",rows);
    return section;
}
