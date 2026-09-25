#include <stdio.h>
#include <stdlib.h>

void print_bin(const char c){
    for(int i = 7; i >= 0; i--){
        printf("%c", (c & (1 << i)) ? '1' : '0');
    }
    printf("\n");
}

int main(
    int argc,
    char *argv[],
    char **envp
){
    FILE *fptr = NULL;
    char pathFile[] = "./test.bin";
    fptr = fopen(pathFile, "rb");
    if(fptr == NULL) { return 1; }
    fseek(fptr, 0L, SEEK_END);
    long size = ftell(fptr);
    fseek(fptr, 0L, SEEK_SET);
    if(size == -1) { fclose(fptr); return 1; }

    unsigned char *content = malloc(size + 1);
    size_t read = fread(content, 1, size, fptr);
    for(int i = 0; i < read; i++){
        print_bin(content[i]);
    }

    free(content);
    fclose(fptr);
    
    return 0;
}