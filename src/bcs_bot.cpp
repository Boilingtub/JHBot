#include <iostream>
#include "whatsapp_bot.hpp"

int main(int argc , char* argv[]) {
    using namespace std;
    for(int i = 0; i < argc; i++) {
        cout << "argv[" << i << "] = " << argv[i];
    }
    printf("\nStarting BCS_bot...\n\n");

    whatsapp_bot BCS_bot("Henry");
    BCS_bot.whatsappcurl.CurlCmd = "curl -X POST";
    BCS_bot.whatsappcurl.Data = appDataMSG;
    BCS_bot.send_whatsapp_msg();

    return 0;
}
