#include <iostream>
#include <string>
#include <fstream>
#include "whatsapp_bot.hpp"

std::string whatsappCURL::FinalCurlCommand() {
    return CurlCmd + " " + URL + " " + Authorization + " " +AccessToken + " " + ContentType + " " + Data;
}

int curl_easy_send(CURLargument arguments[]);

int whatsapp_bot::send_whatsapp_msg() {
    std::string sendmsg = whatsappcurl.FinalCurlCommand();
    std::cout << sendmsg << '\n';
    std::system(sendmsg.c_str());
    return 0;
}

int read_file(std::string filename , std::string &content) {
    using namespace std;
    string line;
    ifstream file(filename);
    if(file.is_open()) {
        content.assign( (std::istreambuf_iterator<char>(file)),
                        (std::istreambuf_iterator<char>()));
        file.close();
    }
    else {
        cout << "Unable to open file";
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

