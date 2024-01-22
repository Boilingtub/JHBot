#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
// configure project setup here
#define COMPILER "gcc"
#define CURRENT_WORKING_DIR "../"
#define SRC_DIR "src/"
#define OUTPUT_DIR "objects/"
#define PROGRAM_NAME "libJHBot"
#define C_FILES "Bot/bot.c Networking/Server.c Networking/HttpRequest.c DataStructures/Common/Node.c DataStructures/Dictionary/Entry.c DataStructures/Dictionary/Dictionary.c DataStructures/Lists/Queue.c DataStructures/Lists/LinkedList.c DataStructures/Trees/BinarySearchTree.c Parsers/cjson/cJSON.c FLI/Python/python_fli.c DataGenerators/Whatsapp/WhatsappDataGen.c"
#define INCLUDE "" 
#define LIB "-lcurl"
#define COMPILE_FLAGS " -c -fPIC "
#define RUN_PROGRAM_ON_COMPILE 0
#define RUN_FLAGS " test "
#define STATIC_COMPILE_FLAGS " -shared "

#ifdef _WIN32
#define EXTENSION ".dll"
#elif __linux__
#define EXTENSION ".so"
#endif
//concatonation functions
#define STR(x) #x

#define RUN_COMMAND(directory,file_name,run_flags) directory file_name run_flags

void check_if_dir_exists() {
    DIR* dir = opendir(OUTPUT_DIR);
    if (dir) {
        closedir(dir);
    } 
    else if (ENOENT == errno) {
        mkdir(OUTPUT_DIR,0777);
    } 
}

char* compile_obj(char* file_name) {
    char* buffer = malloc(3000);
    strcat(buffer,COMPILER);
    strcat(buffer," ");
    strcat(buffer,COMPILE_FLAGS);
    strcat(buffer," ");
    strcat(buffer,CURRENT_WORKING_DIR);
    strcat(buffer,SRC_DIR);
    strcat(buffer,file_name);
    strcat(buffer," ");
    return buffer;
}

char* link_so(char* o_files) {
    char *buffer = malloc(3000);
    strcat(buffer,COMPILER);
    strcat(buffer," ");
    strcat(buffer,STATIC_COMPILE_FLAGS);
    strcat(buffer," ");
    strcat(buffer,o_files);
    strcat(buffer," ");
    strcat(buffer,LIB);
    strcat(buffer," ");
    strcat(buffer,"-o");
    strcat(buffer," ");
    strcat(buffer,PROGRAM_NAME);
    strcat(buffer,EXTENSION);
    strcat(buffer," ");
    return buffer;
}

char* get_file_from_path(char* input_str) {
    int start = 0; 
    for(int i = 0;i < strlen(input_str);i++)    
        if(input_str[i] == '/')
            start = i+1;

    char *buffer = malloc((strlen(input_str) - start)*sizeof(char));
    int buffer_index = 0;
    for(int i = start;i < strlen(input_str);i++) {
        buffer[buffer_index] = input_str[i];
        buffer_index++;
    }
    buffer[strlen(buffer)] = ' ';
    return buffer;
}

int main() {
    check_if_dir_exists();
    remove(RUN_COMMAND(OUTPUT_DIR,PROGRAM_NAME,""));
    chdir(OUTPUT_DIR);
    char *c_files = malloc(strlen(C_FILES));
    char *o_files = malloc(strlen(C_FILES));
    strncpy(c_files,C_FILES,strlen(C_FILES));
    char* c_file = strtok(c_files," ");
    while(c_file) {
        system( compile_obj(c_file) );
        c_file[strlen(c_file)-1] = 'o';
        strcat(o_files,get_file_from_path(c_file)); 
        c_file = strtok(NULL," ");
    }
    system(link_so(o_files));
}
