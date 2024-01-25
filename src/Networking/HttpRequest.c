#include "HttpRequest.h"
#include "../DataStructures/Lists/Queue.h"
#include "../Parsers/cjson/cJSON.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void extract_request_line_fields(struct HTTPRequest *request,
                                 char *request_line);
void extract_header_fields(struct HTTPRequest *request,
                           char* header_fields);
void extract_body(struct HTTPRequest *request, char* body);
    void extract_body_content_type_x_www_form_urlencoded(struct Dictionary
                                                         *body_fields,
                                                         char* body); 
    void extract_body_content_type_json(struct Dictionary *body_fields,
                                        char * body); 

struct HTTPRequest HttpRequest_constructor(char *request_input_string) {
    struct HTTPRequest request;
    //printf("\n+++++++++++++\nInput Length = %lu\n++++++++++++++\n",strlen(request_input_string));
    unsigned long request_string_length = strlen(request_input_string)+1;
    char request_string[request_string_length];
    memset(request_string,0,request_string_length);

    strncpy(request_string, request_input_string,strlen(request_input_string));

    for(int i = 0; i <= strlen(request_string) -1; i++) {
        if (request_string[i] == '\n' && request_string[i+1] == '\n'){
            request_string[i+1] = '|';//separe header from other fields
        }       
    }   
    char *request_line = strtok(request_string , "\n");
    //printf("request_line = %s\n",request_line);
    char *header_fields = strtok(NULL, "|");
    //printf("header_fields = %s\n",header_fields);
    char *body = strtok(NULL , "|");
    //printf("body = %s\n",body);
    
    {
        int incorrect_format = 0;
        if(strlen(request_line) < 2) {
            printf("request line incorrectly formatted\n%s",request_line);
            incorrect_format = 1;
        }
        if(strlen(header_fields) <2) {
            printf("header fields incorrectly formatted\n%s",header_fields);
            incorrect_format = 1;
        }
        if(strlen(body) < 2) {
            printf("body line incorrectly formatted\n%s",body);
            incorrect_format = 1;
        }
        if(incorrect_format == 1) {
            exit(1);
        }
    
    }

    extract_request_line_fields(&request, request_line);
    extract_header_fields(&request, header_fields);
    extract_body(&request, body);
    return request;
}

void HttpRequest_destructor(struct HTTPRequest *request) {
    Dictionary_destructor(&request->request_line);
    Dictionary_destructor(&request->header_fields);
    Dictionary_destructor(&request->body);
}

void extract_request_line_fields(struct HTTPRequest *request,
                                 char *request_line) {

    unsigned long fields_string_length = strlen(request_line)+1;
    char fields[fields_string_length];
    memset(fields,0,fields_string_length);

    strncpy(fields, request_line,strlen(request_line));

    char *method = strtok(fields, " ");

    //printf("method = %s\n",method);
    char *uri = strtok(NULL, " ");
    //printf("uri = %s\n",uri);
    char *http_version = strtok(NULL, "\0");
    //printf("version = %s\n",http_version);

    request->request_line = Dictionary_constructor(
                                                dict_compare_entry_string_keys);
    
    Dictionary_insert(&request->request_line, 
                             "method", sizeof("method"),
                             method , sizeof(char[strlen(method)]));
    
    Dictionary_insert(&request->request_line, 
                             "uri", sizeof("uri"),
                             uri , sizeof(char[strlen(uri)]));

    Dictionary_insert(&request->request_line,
                             "http_version", sizeof("http_version"),
                             http_version, 
                             sizeof(char[strlen(http_version)]));


    if (strcmp(method,"GET") == 0) {
        printf("\nGET REQUEST!!!\n");
        extract_body(request, (char*)Dictionary_search(
                &request->request_line, "uri", sizeof("uri")));
    }

}

void extract_header_fields(struct HTTPRequest *request,
                           char* header_fields) {
    unsigned long header_fields_string_length = strlen(header_fields)+1;  
    char fields[header_fields_string_length];
    memset(fields,0,header_fields_string_length);

    strncpy(fields, header_fields,strlen(header_fields));
    struct Queue headers = Queue_constructor();
    char *field = strtok(fields, "\n");

    while (field) {
        Queue_push(&headers, field, sizeof(char[strlen(field)]));
        field = strtok(NULL, "\n");
    }
    
    request->header_fields = Dictionary_constructor(dict_compare_entry_string_keys);
    while(headers.list.length > 0) {
        
        char *header = (char*)Queue_peek(&headers);
        char *key = strtok(header, ":");
        char *value = strtok(NULL, "\n");
        if(value) {
            if(value[0] == ' ') {
                value++;
            }
            Dictionary_insert(&request->header_fields,
                                          key, sizeof(char[strlen(key)]),
                                          value, 
                                          sizeof(char[strlen(value)]));
        }
        Queue_pop(&headers);
    }
    Queue_destructor(&headers);
}

int compare_content_type_string(const char* test_str, const char* type_str) {
    if(strlen(test_str)>=strlen(type_str)) {
        int type_str_size = strlen(type_str);
        char comp_str[type_str_size];
        strncpy(comp_str,test_str ,type_str_size);
        //printf("type_str = %s\nwith length = %lu\ncomp_str = %s\nwith length = %lu\n",type_str,strlen(type_str),comp_str,strlen(type_str));
        int result = strcmp(comp_str,type_str);
        return result;
    }
    else
        return 1;

}

void extract_body(struct HTTPRequest *request, char* body) {
    struct Dictionary body_fields = Dictionary_constructor(dict_compare_entry_string_keys); 
    char *content_type = (char *)Dictionary_search(&request->header_fields,
                                                   "Content-Type",
                                                   sizeof("Content-Type"));
    int(*compare)(const char* test_str,const char* type_str) = strcmp; 

    if (content_type) {
        if (compare(content_type, "application/x-www-form-urlencoded") == 0) {
            extract_body_content_type_x_www_form_urlencoded(&body_fields, body);
        }
        else 
        if(compare(content_type, "application/json") == 0) {
            extract_body_content_type_json(&body_fields, body);
        }
        else {
            
            //printf("CONTENT-TYPE IS = %s\nwith length %lu\n", content_type,strlen(content_type));
            printf("ERR: DATA TYPE COULD NOT BE DETERMINED\ninserting raw data as \"data\"\n");
            Dictionary_insert(&body_fields, "data", sizeof("data"),
                                   body, sizeof(char[strlen(body)]));
        }
        
    request->body = body_fields;
    }
}

void extract_body_content_type_x_www_form_urlencoded(struct Dictionary
                                                     *body_fields,
                                                     char * body) {
    struct Queue fields = Queue_constructor();
    char *field = strtok(body, "&");
    
    while (field) {
        Queue_push(&fields, field, sizeof(char[strlen(field)]));
    }
 
    field = Queue_peek(&fields);
    while (field) {
        char *key = strtok(field, "=");
        char *value = strtok(NULL, "\0");
        if (value[0] == ' ') {
            value++;
        }
        Dictionary_insert(body_fields, key,
                            sizeof(char[strlen(key)]), value, 
                            sizeof(char[strlen(value)]));
        Queue_pop(&fields);
        field = Queue_peek(&fields);
    }
    Queue_destructor(&fields);
}


void recursively_parse_JSON(struct Dictionary *fields,cJSON *json,char* parent_name) {
    cJSON *component;
    cJSON_ArrayForEach(component,json) {
        if(!cJSON_IsString(component) && !cJSON_IsNumber(component)) {
            if(parent_name) {
                if(component->string) {
                    char* new_parent_name = malloc(sizeof(char[strlen(parent_name)])
                                                   +sizeof(char[strlen
                                                   (component->string)])+2);
                    strcpy(new_parent_name,parent_name);
                    strcat(new_parent_name,".");
                    strcat(new_parent_name,component->string);
                    recursively_parse_JSON(fields,component,new_parent_name);
                    free(new_parent_name);
                }
                else
                    recursively_parse_JSON(fields,component,parent_name);
            }
            else 
                recursively_parse_JSON(fields,component,component->string);
        }
        else {
            if(parent_name)
            {
                char new_key_name[sizeof(char[strlen(parent_name)])
                                            +sizeof(char[strlen
                                            (component->string)])+2];
                strcpy(new_key_name,parent_name);
                strcat(new_key_name,".");
                strcat(new_key_name,component->string);
                Dictionary_insert(fields,
                            new_key_name, 
                            sizeof(char[strlen(new_key_name)]),
                            component->valuestring,
                            sizeof(char[strlen(component->valuestring)]));

            } 
            else {
                Dictionary_insert(fields,
                            component->string, 
                            sizeof(char[strlen(component->string)]),
                            component->valuestring,
                            sizeof(char[strlen(component->valuestring)]));
            }
        }
    }
    free(component);
}

void extract_body_content_type_json(struct Dictionary *body_fields,
                              char* body_text) {
    cJSON *json = cJSON_Parse(body_text);
    recursively_parse_JSON(body_fields,json,NULL);
    cJSON_Delete(json);
}

