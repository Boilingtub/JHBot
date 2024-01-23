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
 

#ifdef _WIN32
#define INCLUDE "-IC:/msys64/usr/include"
#define LIB "-LC:/msys64/usr/lib/ -lcurl -lws2_32"
#elif __linux__
#define INCLUDE ""
#define LIB "-lcurl"
#endif

#define COMPILE_FLAGS " -Wall -c -fPIC "
#define RUN_PROGRAM_ON_COMPILE 0
#define RUN_FLAGS " test "
#define STATIC_COMPILE_FLAGS " -shared "

#ifdef _WIN32
#define EXTENSION ".dll"
#elif __linux__
#define EXTENSION ".so"
#endif
//concatonation functions
#define CAT3(x,y,z) x y z
#define CAT2(x,y) x y

#define RUN_COMMAND(directory,file_name,run_flags) directory file_name run_flags

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

char* compile_obj(char* file_name) {
    char* buffer = malloc(3000);
    strcpy(buffer,COMPILER);
    strcat(buffer," ");
    strcat(buffer,INCLUDE);
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
    strcpy(buffer,COMPILER);
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

void cpy_lib_to_dependencies() {
    char source_file_path[] = CAT3(OUTPUT_DIR,PROGRAM_NAME,EXTENSION);
    char source_file_name[] = CAT2(PROGRAM_NAME,EXTENSION);
    #define target_folders_count 1
    char* target_folders[target_folders_count] = {"python_implementation/"};
    for(int i = 0;i < target_folders_count;i++) {
        char target_file_path[strlen(target_folders[i])+strlen(source_file_name)+2];
        strcpy(target_file_path, target_folders[i]);
        strcat(target_file_path, source_file_name);
        
        FILE* source = fopen(source_file_path,"r");
        if( source == NULL) {
            printf("Could not open Source File\n");
            exit(EXIT_FAILURE);
        }

        FILE* target = fopen(target_file_path, "w");
        if(target == NULL) {
            fclose(source);
            printf("Could not open Target File\n");
            exit(EXIT_FAILURE);
        }

        int ch;
        while( (ch = fgetc(source)) != EOF)
            fputc(ch, target);
        fclose(source);
        fclose(target);
    }
}

int main() {
    check_if_dir_exists();
    remove(RUN_COMMAND(OUTPUT_DIR,PROGRAM_NAME,""));
    chdir(OUTPUT_DIR);
    char c_files[strlen(C_FILES)+2];
    char o_files[strlen(C_FILES)+2];
    strncpy(c_files,C_FILES,strlen(C_FILES)+1);
    char* c_file = strtok(c_files," ");
    while(c_file) {
        system( compile_obj(c_file) );
        c_file[strlen(c_file)-1] = 'o';
        strcat(o_files,get_file_from_path(c_file)); 
        c_file = strtok(NULL," ");
    }
    system(link_so(o_files));
    chdir(CURRENT_WORKING_DIR);
    cpy_lib_to_dependencies();
}
