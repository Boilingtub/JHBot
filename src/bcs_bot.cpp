#include <iostream>
#include <string>
#include <fstream>
#include <curl/curl.h>


const char appID[] = "149485694924975";
const char msgURL[] = "https://graph.facebook.com/v17.0/149485694924975/messages";

typedef struct CURLargument {
    CURLoption option;
    char* parameter;
} CURLargument;

int curl_easy_send(CURLargument arguments[]);


class BCS_bot {
    public:
        BCS_bot(std::string name) {
            std::cout << "BCS_bot : " << name << " initialized\n";
        }
        int send_wattsapp_msg();
};


int BCS_bot::send_wattsapp_msg() {

    return 0;
}

int read_file(std::string filename , std::string &content) {
    std::string line;
    std::ifstream file(filename);
    if(file.is_open()) {
        content.assign( (std::istreambuf_iterator<char>(file)),
                        (std::istreambuf_iterator<char>()));
        file.close();
    }
    else {
        std::cout << "Unable to open file";
    }
    return 0;
}

int write_file(std::string filename , std::string content) {
    std::ofstream file (filename);
    if (file.is_open()) {
        file << content;
        file.close();
    }
    else { 
        std::cout << "Unable to open file";
    }
    return 0;
} 


int curl_easy_send(CURLargument arguments[], int arguments_count) {
    CURL *easy_handle = curl_easy_init();
    if(easy_handle) {
        const long timeout_time = 5;
        CURLcode result;
        result = curl_easy_setopt(easy_handle , CURLOPT_TIMEOUT , timeout_time);
        for(int i; i < arguments_count; i++) {
            result = curl_easy_setopt(easy_handle , arguments[i].option,
                                                arguments[i].parameter);            
        }
        result = curl_easy_perform(easy_handle);
        if(result != CURLE_OK) {
            fprintf(stderr , "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(result));
        }

        curl_easy_cleanup(easy_handle);
    }
    return 0;
}

int main(int argc , char* argv[]) {
    using namespace std;
    for(int i = 0; i < argc; i++) {
        printf("argv[%d] = %s\n",i,argv[i]);
    }
    printf("\nStarting BCS_bot...\n\n");

    BCS_bot bot("Henry");
    bot.send_wattsapp_msg();

    return 0;
}

/*
curl -i -X POST `
  https://graph.facebook.com/v17.0/149485694924975/messages `
  -H 'Authorization: Bearer EAAzl9nSKvWYBOZCuDgfICV426Yhc4ZB9aqXZCgwAyLnKLY5ytFXWZC45OKrd8k4OjgERCo20A6LZCm2hPLqntgNsZBekI8dSZCI0S3dJMIhSjBUw95ZARJBdR8Df8160K0ngEbJ30PH5gLzzGXZAQ3F8D4uUCF9aNqB8QeqWZCT1jRAROZBro0dk81LszcSLy8NMjnrFJN5oiszwnofQOLlslZA5edDdHGqg' `
  -H 'Content-Type: application/json' `
  -d '{ \"messaging_product\": \"whatsapp\", \"to\": \"\", \"type\": \"template\", \"template\": { \"name\": \"hello_world\", \"language\": { \"code\": \"en_US\" } } }'
*/
