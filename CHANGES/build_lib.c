#include <stdlib.h>
#include <stdio.h>
#ifdef _WIN32
#include <io.h>
#elif __linux__
#include <unistd.h>
#endif
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
// configure project setup here
#define COMPILER "gcc"
#define ARCHIVER = "ar"
#define CURRENT_WORKING_DIR "../"
#define SRC_DIR "src/"
#define OUTPUT_DIR "objects/"
#define PROGRAM_NAME "libJHBot"
#define C_FILES "Bot/bot.c Networking/Server.c Networking/HttpRequest.c DataStructures/Common/Node.c DataStructures/Dictionary/Entry.c DataStructures/Dictionary/Dictionary.c DataStructures/Lists/Queue.c DataStructures/Lists/LinkedList.c DataStructures/Trees/BinarySearchTree.c Parsers/cjson/cJSON.c FLI/Python/python_fli.c DataGenerators/Whatsapp/WhatsappDataGen.c"
 

#ifdef _WIN32
#define INCLUDE "-IC:/msys64/mingw64/include"
#define LIB "-LC:/msys64/mingw64/lib/ -lcurl -lws2_32 -lssl -lcrypto -lpsl -lidn2 -lssh2 -lcrypt32 -lwldap32 -lbcrypt -lunistring -lzstd -lbrotlicommon -lbrotlidec -lz -liconv"
#elif __linux__
#define INCLUDE ""
#define LIB "-lcurl -lssl -lcrypto"
#endif

#define COMPILE_FLAGS " -Wall -c -fPIC"
#define RUN_PROGRAM_ON_COMPILE 0
#define RUN_FLAGS " test "
#define STATIC_COMPILE_FLAGS " rcs "
#define SHARED_COMPILE_FLAGS " -shared -O3 -Ofast "

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

void compile_obj(char* file_name) {
    char buffer[3000];
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
    system(buffer);
}

void link_static(char* o_files) {
    char buffer[3000];
    strcpy(buffer,"ar");
    strcat(buffer," ");
    strcat(buffer,STATIC_COMPILE_FLAGS);
    strcat(buffer," ");
    strcat(buffer,PROGRAM_NAME);
    strcat(buffer,".a");
    strcat(buffer," ");
    strcat(buffer,o_files);
    strcat(buffer," ");
    strcat(buffer,LIB);
    strcat(buffer," ");
    system(buffer);
}

void create_unified_obj() {
    char buffer[3000];
    strcpy(buffer,COMPILER);
    strcat(buffer," ");
    strcat(buffer,COMPILE_FLAGS);
    strcat(buffer," ");
    strcat(buffer,PROGRAM_NAME); 
    strcat(buffer,".c");
    strcat(buffer," ");
    strcat(buffer,"-o");
    strcat(buffer," ");
    strcat(buffer,PROGRAM_NAME);
    strcat(buffer,".o");
    strcat(buffer," ");
    system(buffer);

}

void link_shared(char* o_files) {
    char buffer[3000];
    strcpy(buffer,COMPILER);
    strcat(buffer," ");
    strcat(buffer,SHARED_COMPILE_FLAGS);
    strcat(buffer," ");
    strcat(buffer,"-o");
    strcat(buffer," ");
    strcat(buffer,PROGRAM_NAME);
    strcat(buffer,EXTENSION);
    strcat(buffer," ");
    strcat(buffer,o_files);
    strcat(buffer," ");
    strcat(buffer,LIB);
    strcat(buffer," ");
    system(buffer);
}



char* get_file_from_path(char* input_str) {
    int start = 0; 
    for(int i = 0;i < strlen(input_str);i++)    
        if(input_str[i] == '/')
            start = i+1;

    int buffer_size = sizeof(char[strlen(input_str) - start + 1]);
    char *buffer = malloc(buffer_size);
    for(int i = 0;i < buffer_size;i++) {
        buffer[i] = input_str[start + i];
    }

    buffer[buffer_size-1] = ' ';
    buffer[buffer_size] = '\0';
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


void add_header_to_unified_header(char* header_to_add) {
    FILE* target = fopen(CAT2(PROGRAM_NAME,".c"),"a");
    if(!target) {
        printf("unable to open target file");
        fclose(target);
        return;
    }

    char path_to_header[strlen("#include \"") + strlen(CURRENT_WORKING_DIR) +
                        strlen(SRC_DIR) + strlen(header_to_add)+3];
    strncpy(path_to_header,"#include \"",strlen("#include \"")+1);
    strncat(path_to_header,CURRENT_WORKING_DIR,strlen(CURRENT_WORKING_DIR)+1);
    strncat(path_to_header,SRC_DIR,strlen(SRC_DIR)+1);
    strncat(path_to_header,header_to_add,strlen(header_to_add)+1);
    strncat(path_to_header,"\"\n",strlen("\"\n")+1);
    
    fprintf(target,"%s",path_to_header);
    rewind(target);
    fclose(target); 
}

int main() {
    check_if_dir_exists();
    remove(RUN_COMMAND(OUTPUT_DIR,PROGRAM_NAME,""));
    remove(CAT3(OUTPUT_DIR,PROGRAM_NAME,".c"));
    chdir(OUTPUT_DIR);
    char c_files[strlen(C_FILES)+2];
    char o_files[strlen(C_FILES)+2];
    strncpy(c_files,C_FILES,strlen(C_FILES)+1);
    char* c_file = strtok(c_files," ");
    while(c_file) {
        compile_obj(c_file);
        //c_file[strlen(c_file)-1] = 'h';
        //add_header_to_unified_header(c_file);
        c_file[strlen(c_file)-1] = 'o';
        strcat(o_files,get_file_from_path(c_file)); 
        c_file = strtok(NULL," ");
    }
    //link_static(o_files);
    //create_unified_obj();
    link_shared(o_files);
    chdir(CURRENT_WORKING_DIR);
    cpy_lib_to_dependencies();
}
