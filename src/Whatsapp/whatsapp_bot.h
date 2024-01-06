#ifndef BCS_BOT_INCLUDE
#define BCS_BOT_INCLUDE

#include <curl/curl.h>

int read_file(char* filename , char* *content);
int write_file(char* filename, char* content);

struct whatsappCURL {
    char URL[64];
    char Header_Autorization[256];
    char Header_ContentType[128];
    char* Data;
};

struct WhatsappBot{
    char* name;
    struct whatsappCURL whatsappcurl;
    void (*parse_received_msg)(char *msg);
    void (*interpret_msg)(struct WhatsappBot *bot, char* msg);
    int (*send_whatsapp_msg)(struct WhatsappBot *bot);
    void (*set_bot_curl_properties)(struct WhatsappBot *bot,
                                   char URL[] , char Authorization[],
                                   char ContentType[], char Data[],
                                   unsigned long data_size);
};
struct WhatsappBot whatsappbot_constructor(char* name);

#endif
