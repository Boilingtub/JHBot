#ifndef BCS_BOT_INCLUDE
#define BCS_BOT_INCLUDE

#include <curl/curl.h>
#include "../Networking/Server.h"



struct whatsappCURL {
    char URL[64];
    char Header_Autorization[256];
    char Header_ContentType[128];
    char* Data;
};

struct WhatsappMessage {
    char* name;
    char* wa_id;
    char* from;
    char* id;
    char* timestamp;
    char* type;
    char* text_body;
};
struct WhatsappBot{
    char* name;
    struct whatsappCURL whatsappcurl;
    struct Server webhook;
    struct WhatsappMessage received_message;
    void (*read_text_file)(char* file_path, char** buffer);
    void (*set_webhook_properties)(struct WhatsappBot *bot,
                                   int domain, int service, int protocol,
                                   u_long interface, int port, int backlog);
    void (*write_text_file)(char* file_path, char* buffer);
    void (*parse_received_msg)(struct WhatsappBot *bot,char *msg);
    int  (*send_whatsapp_msg)(struct WhatsappBot *bot);
    void (*set_sending_properties)(struct WhatsappBot *bot,
                                   char URL[] , char Authorization[],
                                   char ContentType[], char Data[],
                                   unsigned long data_size);
};
struct WhatsappBot whatsappbot_constructor(char* name);

void whatsappbot_destructor(struct WhatsappBot *bot);
#endif
