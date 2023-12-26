#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

// configure project setup here
#define COMPILER "g++"
#define SRC_DIR "src/"
#define OUTPUT_DIR "output/"
#define PROGRAM_NAME "BCS_bot"
#define C_FILES "bcs_bot.cpp"
#define INCLUDE "" 
#define LIB "-lcurl"
#define FLAGS "-std=c++20"
#define RUN_PROGRAM_ON_COMPILE 1

//concatonation functions
#define STR(x) #x
#define COMPILE_COMMAND(compiler,main_file,program_name) compiler " " LIB " " SRC_DIR main_file " -o " OUTPUT_DIR program_name " " FLAGS
#define RUN_COMMAND(directory,program_name) directory program_name

int main() {
    remove(RUN_COMMAND(OUTPUT_DIR,PROGRAM_NAME));
    system(COMPILE_COMMAND(COMPILER,C_FILES,PROGRAM_NAME));
    if (RUN_PROGRAM_ON_COMPILE == 1) {
        chdir(OUTPUT_DIR);
        system(RUN_COMMAND("./",PROGRAM_NAME));
    }
}
