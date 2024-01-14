#ifndef BCS_BOT_INCLUDE
#define BCS_BOT_INCLUDE
#include <sys/types.h>
#include "../Networking/Server.h"

void read_text_file(char* file_path, char** buffer);
void write_text_file(char* file_path, char* buffer);

struct HTTPRequest launch_listner_server(struct Server *server);
int post_data(char *URL, char *Headers[],unsigned long Header_count,
                char *Data);

struct HTTPRequest parse_to_httpresponse(char* msg);

#endif
