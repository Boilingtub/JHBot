#ifndef BCS_BOT_INCLUDE
#define BCS_BOT_INCLUDE
#include <sys/types.h>
#include "../Networking/Server.h"

char* read_text_file(char* file_path);

void write_text_file(char* file_path, char* buffer);

void set_global_listner_server(int domain,int service,int protocol,
                               unsigned long face,int port,int backlog);

struct HTTPRequest launch_listner_server(struct Server *server,
                                         char* server_response_message);

int post_data(char *URL, char *Headers[],unsigned long Header_count,
                char *Data);

struct HTTPRequest parse_to_httpresponse(char* msg);

#endif
