#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>

// configure project setup here
#define COMPILER "clang"
#define CURRENT_WORKING_DIR "../"
#define SRC_DIR "src/"
#define OUTPUT_DIR "output/"
#define PROGRAM_NAME "BCS_bot"
#define C_FILES "bcs_bot.c Whatsapp/whatsapp_bot.c Networking/Server.c Networking/HttpRequest.c DataStructures/Common/Node.c DataStructures/Dictionary/Entry.c DataStructures/Dictionary/Dictionary.c DataStructures/Lists/Queue.c DataStructures/Lists/LinkedList.c DataStructures/Trees/BinarySearchTree.c Parsers/cjson/cJSON.c"
#define INCLUDE "" 
#define LIB "-lcurl"
#define COMPILE_FLAGS "  "
#define RUN_PROGRAM_ON_COMPILE 0
#define RUN_FLAGS " test "

//concatonation functions
#define STR(x) #x
#define COMPILE_COMMAND(compiler,current_working_dir,program_name) compiler " " LIB " " C_FILES " -o " current_working_dir OUTPUT_DIR program_name " " COMPILE_FLAGS " " INCLUDE
#define RUN_COMMAND(directory,program_name,run_flags) directory program_name run_flags

void check_if_dir_exists() {
    DIR* dir = opendir(OUTPUT_DIR);
    if (dir) {
        closedir(dir);
    } 
    else if (ENOENT == errno) {
        mkdir(OUTPUT_DIR,0777);
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
