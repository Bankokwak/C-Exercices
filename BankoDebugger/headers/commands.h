#ifndef COMMANDS_H
#define COMMANDS_H

#include <sys/types.h>
#include <stdint.h>
#include "../headers/debugStruct.h"

void set_breakpoint(pid_t pid, uintptr_t addr_base, uintptr_t addr_function, uintptr_t offset, struct dataSaved *datasaved);
void rewrite_breakpoint(int *status, pid_t pid, struct dataSaved *datasaved);
void waitIF(int *status, pid_t pid, struct dataSaved *datasaved);
void cont(int *status, pid_t pid, struct dataSaved *datasaved);

#endif