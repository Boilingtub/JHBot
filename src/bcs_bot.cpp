#include <iostream>
#include <string>
#include <fstream>
#include <curl/curl.h>

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


int curl_test(std::string url) {
    CURL *easy_handle = curl_easy_init();
    if(easy_handle) {
        const long timeout_time = 5;
        CURLcode result;

        result = curl_easy_setopt(easy_handle, CURLOPT_URL , url.c_str());
        result = curl_easy_setopt(easy_handle , CURLOPT_TIMEOUT , timeout_time);

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

    curl_test(""); 

    return 0;
}
