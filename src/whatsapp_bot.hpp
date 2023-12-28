#ifndef BCS_BOT_INCLUDE
#define BCS_BOT_INCLUDE
#include <curl/curl.h>


const std::string CurlCmdSendMSG = "curl -X POST ";
const std::string whatsappData;
const std::string appDataTemplate = "-d '{ \"messaging_product\": \"whatsapp\", \"to\": \"27769827148\", \"type\": \"template\", \"template\": { \"name\": \"bcs_first\", \"language\": { \"code\": \"en_US\" } } }'";
const std::string appDataMSG = "-d '{\"messaging_product\":\"whatsapp\",\"recipient_type\":\"individual\",\"to\":\"27769827148\",\"type\":\"text\",\"text\": {\"body\":\"Hello!\"}}'";

int read_file(std::string filename , std::string &content);
int write_file(std::string filename , std::string content);

typedef struct CURLargument {
    CURLoption option;
    char* parameter;
} CURLargument;

int curl_easy_send(CURLargument arguments[], int arguments_count);

class whatsappCURL {
public:
    std::string CurlCmd;
    std::string URL = "https://graph.facebook.com/v17.0/149485694924975/messages";
    std::string Authorization = "-H 'Authorization: Bearer";
    std::string AccessToken = "EAAzl9nSKvWYBO6sAZA6myet0cB5c8pzklKdB59F6xzLk9BDZCZCItegIWZBGLzz5023ZBzZB3F7TpjYwZCijAGDWxnay9RKBMtw9yGQpGiUXuxVIKjkWRXIpAhZA9KZC9V8melPrtJBIv1SKQ5CqEBXvxENHLj59XDZCYPj2QOGl92F4nW53Jc91h8kohZB7wNLbXmeZCQ6LR1Gq6tbU7WOuM6dT6uC6C2UC'";
    std::string ContentType = "-H 'Content-Type: application/json'"; 
    std::string DataStart = "";
    std::string Data;
    
    std::string FinalCurlCommand();
};
 
class whatsapp_bot{
public:
    std::string name;
    whatsappCURL whatsappcurl;
    whatsapp_bot(std::string initializing_name) {
        name = initializing_name;
    }
    int send_whatsapp_msg();
    
};

#endif
