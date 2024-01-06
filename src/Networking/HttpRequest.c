#include "HttpRequest.h"
#include "../DataStructures/Lists/Queue.h"
#include <string.h>
#include <stdlib.h>

void extract_request_line_fields(struct HTTPRequest *request,
                                 char *request_line);
void extract_header_fields(struct HTTPRequest *request,
                           char* header_fields);
void extract_body(struct HTTPRequest *request, char* body);
    void extract_body_content_type_x_www_form_urlencoded(struct Dictionary
                                                         *body_fields,
                                                         char* body); 

struct HTTPRequest http_request_constructor(char *request_input_string) {

    struct HTTPRequest request;
    char request_string[strlen(request_input_string)];
    strcpy(request_string, request_input_string);

    for(int i = 0; i <= strlen(request_string) -1; i++) {
        if (request_string[i] == '\n' && request_string[i+1] == '\n'){
            request_string[i+1] = '|';//separe header from other fields
        }       
    }   
    char *request_line = strtok(request_string , "\n");
    char *header_fields = strtok(NULL, "|");
    char *body = strtok(NULL , "|");

    extract_request_line_fields(&request, request_line);
    extract_header_fields(&request, header_fields);
    extract_body(&request, body);

    return request;
}

void http_request_destructor(struct HTTPRequest *request) {
    dictionary_destructor(&request->request_line);
    dictionary_destructor(&request->header_fields);
    dictionary_destructor(&request->body);
}

void extract_request_line_fields(struct HTTPRequest *request,
                                 char *request_line) {

    char fields[strlen(request_line)];
    strcpy(fields, request_line);

    char *method = strtok(fields, " ");
    char *uri = strtok(NULL, " ");
    char *http_version = strtok(NULL, "\0");

    struct Dictionary request_line_dict = dictionary_constructor(
                                                compare_string_keys);
    request_line_dict.insert(&request_line_dict, 
                             "method", sizeof("method"),
                             method , sizeof(char[strlen(method)]));
    request_line_dict.insert(&request_line_dict, 
                             "uri", sizeof("uri"),
                             uri , sizeof(char[strlen(uri)]));
    request_line_dict.insert(&request_line_dict,
                             "http_version", sizeof("http_version"),
                             http_version, 
                             sizeof(char[strlen(http_version)]));
    request->request_line = request_line_dict;
    if (request->request_line.search(&request->request_line, 
                                     "GET", sizeof("GET"))){
        extract_body(request, (char*)request->request_line.search(
                &request->request_line, "uri", sizeof("uri")));
    }

}

void extract_header_fields(struct HTTPRequest *request,
                           char* header_fields) {
    char fields[strlen(header_fields)];
    strcpy(fields, header_fields);
    struct Queue headers = queue_constructor();
    char *field = strtok(fields, "\n");

    while (field) {
        headers.push(&headers, field, sizeof(char[strlen(field)]));
        field = strtok(NULL, "\n");
    }
    
    request->header_fields = dictionary_constructor(compare_string_keys);
    char *header = (char*)headers.peek(&headers);
    while(header) {
        char *key = strtok(header, ":");
        char *value = strtok(NULL, "\n");
        if(value) {
            if(value[0] == ' ') {
                value++;
            }
            request->header_fields.insert(&request->header_fields,
                                          key, sizeof(char[strlen(key)]),
                                          value, 
                                          sizeof(char[strlen(value)]));
        }
        headers.pop(&headers);
        header = (char*)headers.peek(&headers);
    }
    queue_destructor(&headers);
}

void extract_body(struct HTTPRequest *request, char* body) {
    char *content_type = (char *)request->header_fields.search(&request->header_fields, "Content-Type", sizeof("Content-Type"));
    if (content_type) {
        struct Dictionary body_fields = dictionary_constructor(compare_string_keys); 
        if (strcmp(content_type, "application/x-www-form-urlencoded") == 0) {
            extract_body_content_type_x_www_form_urlencoded(&body_fields,body);
        }
        else {
            body_fields.insert(&body_fields, "data", sizeof("data"), body, sizeof(char[strlen(body)]));
        }
        request->body = body_fields;
    }
}

void extract_body_content_type_x_www_form_urlencoded(struct Dictionary *body_fields, char * body) {
    struct Queue fields = queue_constructor();
    char *field = strtok(body, "&");
    
    while (field) {
        fields.push(&fields, field, sizeof(char[strlen(field)]));
    }
    
    field = fields.peek(&fields);
    while (field) {
        char *key = strtok(field, "=");
        char *value = strtok(NULL, "\0");
        if (value[0] == ' ') {
            value++;
        }
        body_fields->insert(body_fields, key, sizeof(char[strlen(key)]), value, sizeof(char[strlen(value)]));
        fields.pop(&fields);
        field = fields.peek(&fields);
    }
    queue_destructor(&fields);
}

