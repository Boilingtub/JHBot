#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>

// configure project setup here
#define COMPILER "gcc"
#define CURRENT_WORKING_DIR "../"
#define SRC_DIR "src/"
#define OUTPUT_DIR "output/"
#define PROGRAM_NAME "bot_test"
#define C_FILES "tests.c Bot/bot.c Bot/bot.h Networking/Server.c Networking/HttpRequest.c DataStructures/Common/Node.c DataStructures/Dictionary/Entry.c DataStructures/Dictionary/Dictionary.c DataStructures/Lists/Queue.c DataStructures/Lists/LinkedList.c DataStructures/Trees/BinarySearchTree.c Parsers/cjson/cJSON.c FLI/Python/python_fli.c DataGenerators/Whatsapp/WhatsappDataGen.c"

#ifdef _WIN32
#define INCLUDE "-IC:/msys64/mingw64/include"
#define LIB "-LC:/msys64/mingw64/lib -lcurl -lws2_32 -lssl -lcrypto"
#elif __linux__
#define INCLUDE ""
#define LIB "-lcurl -lssl -lcrypto"
#endif

#define COMPILE_FLAGS " -Wall -O3 -Ofast "
//#define COMPILE_FLAGS " -Wall -g -ggdb3 "
#define RUN_PROGRAM_ON_COMPILE 0
#define RUN_FLAGS " test "

//concatonation functions
#define STR(x) #x
#define COMPILE_COMMAND(compiler,current_working_dir,program_name) compiler " " COMPILE_FLAGS " " INCLUDE " " C_FILES " " LIB " -o " current_working_dir OUTPUT_DIR program_name
#define RUN_COMMAND(directory,program_name,run_flags) directory program_name run_flags

void check_if_dir_exists() {
    DIR* dir = opendir(OUTPUT_DIR);
    if (dir) {
        closedir(dir);
    } 
    else if (ENOENT == errno) {
        #ifdef _WIN32
        mkdir(OUTPUT_DIR);
        #elif __linux__
        mkdir(OUTPUT_DIR,0777);
        #endif
    } 
}

int main() {
    check_if_dir_exists();
    remove(RUN_COMMAND(OUTPUT_DIR,PROGRAM_NAME,""));
    chdir(SRC_DIR);
    system(COMPILE_COMMAND(COMPILER,CURRENT_WORKING_DIR,PROGRAM_NAME));
    if (RUN_PROGRAM_ON_COMPILE == 1) {
        chdir(CURRENT_WORKING_DIR);
        chdir(OUTPUT_DIR);
        system(RUN_COMMAND("./",PROGRAM_NAME,RUN_FLAGS));
    }
}
