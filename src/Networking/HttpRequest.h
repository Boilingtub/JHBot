#ifndef HTTPRequest_h
#define HTTPRequest_h
#include "../DataStructures/Dictionary/Dictionary.h"

struct HTTPRequest {
    struct Dictionary request_line;
    struct Dictionary header_fields;
    struct Dictionary body;
    void(*extract_body_function)(struct Dictionary *body_fields, char* body_text);
};

struct HTTPRequest http_request_constructor(char *request_string,
                                            void(*extract_body_function)(
                                            struct Dictionary *body_fields,
                                            char* body_text));

void http_request_destructor(struct HTTPRequest *request);

#endif
