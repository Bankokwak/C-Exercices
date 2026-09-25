#include "../headers/utils.h"
#include "../headers/debugStruct.h"
#include "../headers/commands.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>

void set_breakpoint(pid_t pid, uintptr_t addr_base, uintptr_t addr_function, uintptr_t offset, struct dataSaved *datasaved){
    uintptr_t addr = addr_base + (addr_function - offset);

    errno = 0;
    long original_data = ptrace(PTRACE_PEEKTEXT, pid, (void*)addr, NULL);
    if (original_data == -1 && errno != 0) {
        perror("PTRACE_PEEKTEXT");
        return;
    }

    datasaved->addr = addr;
    datasaved->original_data = original_data;

    long breakpoint_data = (original_data & ~0xFFL) | 0xCC;
    if(ptrace(PTRACE_POKETEXT, pid, (void*)addr, (void*)breakpoint_data) == -1){
        perror("PTRACE_POKETEXT");
        return;
    }
}

void rewrite_breakpoint(int *status, pid_t pid, struct dataSaved *datasaved){
    struct user_regs_struct regs;
    if(ptrace(PTRACE_GETREGS, pid, NULL, &regs) == -1){
        perror("PTRACE_GETREGS");
        return;
    }
    
    if(ptrace(PTRACE_POKETEXT, pid, (void*)datasaved->addr, (void*)datasaved->original_data) == -1){
        perror("PTRACE_POKETEXT");
        return;
    }
    regs.rip -= 1;
    if(ptrace(PTRACE_SETREGS, pid, NULL, &regs) == -1){
        perror("PTRACE_SETREGS");
        return;
    }
}

void waitIF(int *status, pid_t pid, struct dataSaved *datasaved){
    if (WIFEXITED(*status)) {
        printf("Enfant terminé normalement, code retour: %d\n", WEXITSTATUS(*status));
    } else if (WIFSIGNALED(*status)) {
        printf("Enfant tué par le signal %d\n", WTERMSIG(*status));
    } else if (WIFSTOPPED(*status)) {
        printf("Enfant à nouveau stoppé (signal %d) — probablement un breakpoint futur\n", WSTOPSIG(*status));
    }
}

void cont(int *status, pid_t pid, struct dataSaved *datasaved){
    rewrite_breakpoint(status, pid, datasaved);
    
    if(ptrace(PTRACE_CONT, pid, NULL, NULL) == -1){
        perror("PTRACE_CONT");
        return;
    }
    waitpid(pid, status, 0);
}