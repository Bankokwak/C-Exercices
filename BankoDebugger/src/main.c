#include "../headers/utils.h"
#include "../headers/commands.h"
#include "../headers/debugStruct.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <inttypes.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>

void CommandeProcess(int status, pid_t pid, uintptr_t addr_base, struct dataSaved *datasaved){
    while(1){    
        printf("#> ");
        char userInput[64] = {};
        fgets(userInput, sizeof(userInput), stdin);
        if(userInput[strlen(userInput)-1] == '\n'){
            userInput[strlen(userInput)-1] =  '\0';
        }

        int maxResult = 10;
        int maxCharResult = 64;
        char result[maxResult][maxCharResult] = {};
        int resultCount = SplitString(userInput, " ", maxResult, maxCharResult, result);
        
        if(!strcmp(result[0], "cont")){
            cont(&status, pid, datasaved);
        }else if(!strcmp(result[0], "break")){
            /*
            if(strlen(result[1]) == 0 && strlen(result[2]) == 0){
                printf("Usage: break (pointer_function) (offset)\n");
                continue;
            }
            */
            uintptr_t addr_function = (uintptr_t)strtoumax(result[1], NULL, 16);
            uintptr_t offset = (uintptr_t)strtoumax(result[2], NULL, 16);
            //set_breakpoint(pid, addr_base, addr_function, offset, datasaved);
            set_breakpoint(pid,addr_base,0x00101147, 0x100000, datasaved);
        }else if(!strcmp(result[0], "quit")){
            break;
        }
    }
}

int main(
    int argc,
    char *argv[],
    char **envp
){
    if(argc < 2 && !FileExist(argv[1])){
        printf("Usage: BankoDebugger [filepath]\n");
        return 1;
    }

    pid_t pid = fork();
    if(pid == -1){
        return -1;
    }else if(pid == 0){
        //child
        if(ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1){
            perror("PTRACE_TRACEME");
            exit(1);
        }
        char *args[] = { argv[1], NULL };
        execve(argv[1], args, envp);
    }else{
        //parent
        int status;
        waitpid(pid, &status, 0);

        if (WIFSTOPPED(status)) {
            #pragma region READ BASE
            FILE *fptr;
            char filepath[64];

            snprintf(filepath, sizeof(filepath), "/proc/%d/maps", pid);

            fptr = fopen(filepath, "r");

            if (fptr == NULL) {
                perror("fopen");
                return 1;
            }

            char buffer[4096];

            char firstAd[64] = {};
            while (fgets(buffer, sizeof(buffer), fptr) != NULL) {
                if(strlen(firstAd) == 0){
                    int maxResult = 6;
                    int maxCharResult = 64;
                    char result[maxResult][maxCharResult] = {};
                    int resultCount = SplitString(buffer, "-", maxResult, maxCharResult, result);
                    strcpy(firstAd, result[0]);
                }
            }

            fclose(fptr);
            #pragma endregion
            struct dataSaved datasaved = {};

            uintptr_t base_addr = (uintptr_t)strtoumax(firstAd, NULL, 16);

            CommandeProcess(status, pid, base_addr, &datasaved);
        }
    }
    return 0;
}