#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long get_file_size(const char *filepath){
    FILE *fp = fopen(filepath, "rb");
    if(fp==NULL) return -1;

    fseek(fp, 0L, SEEK_END);
    long size = ftell(fp);

    fclose(fp);
    return size;
}

int FileExist(const char *filepath){
    FILE *file;
    if ((file = fopen(filepath, "r")))
    {
        fclose(file);
        return 1;
    }
    return 0;
}

int main(int argc,
    char *argv[],
    char **envp
) {
    const char *pathFile = NULL;
    const char *outputFile = NULL;
    int reverse = 0;

    if(argc < 2){
        printf("Usage: bankohexdump [filepath] {-o|-r}");
        return -1;
    }
    if(FileExist(argv[1]) == 1){
        pathFile = argv[1];
    }
    for(int i = 0; i < argc; i++){
        if(strcmp(argv[i], "--reverse-hex") == 0 || strcmp(argv[i], "-r") == 0){
            reverse = 1;
        }
        if (strcmp(argv[i], "-o") == 0 ||
            strcmp(argv[i], "--output") == 0) {

            if (i + 1 >= argc) {
                fprintf(stderr, "Missing output file\n");
                return 1;
            }

            outputFile = argv[++i];
        }
    }
    
    FILE *fptr;
    FILE *foutput;
    if(outputFile != NULL){
        foutput = fopen(outputFile, "w");
    }

    fptr = fopen(pathFile, "rb");
    if(fptr == NULL) { return 1; }
    
    long size = get_file_size(pathFile);
    if(size == -1) { fclose(fptr); return 1; }
    unsigned char *content = malloc(size + 1);
    size_t read = fread(content, 1, size, fptr);
    content[read] = '\0';

    unsigned char *content2 = malloc(size + 1);

    int line = 0;
    int printLine = 1;

    int flipflop = 1;
    for(int i = 0; i < read; i++){
        if(printLine){
            if(foutput != NULL){
                fprintf(foutput, "%07x ", line);
            }
            printf("%07x ", line);
            printLine = 0;
        }
        printf("%02x", content[i]);
        if(foutput != NULL){
            fprintf(foutput, "%x", content[i]);
        }
        if(flipflop){
            content2[i+1] = content[i];
            flipflop = 0;
        }else{
            content2[i-1] = content[i];
            flipflop = 1;
            printf(" ");
            if(foutput != NULL){
                fprintf(foutput, " ");
            }
        }
        
        line++;
        if(i == 15){
            if(foutput != NULL){
                fprintf(foutput, "\n");
            }
            printf("\n");
            printLine = 1;
        }
    }
    printf("\n%07x ", line);

    if(reverse){
        line = 0;
        printLine = 1;
        flipflop = 1;
        printf("\nReverse hex:\n");
        if(foutput != NULL){
            fprintf(foutput, "\nReverse hex:\n");
        }
        for(int i = 0; i < read; i++){
            if(printLine){
                if(foutput != NULL){
                    fprintf(foutput, "%07x ", line);
                }
                printf("%07x ", line);
                printLine = 0;
            }
            printf("%02x", content2[i]);
            if(foutput != NULL){
                fprintf(foutput, "%x", content2[i]);
            }
            if(flipflop){
                flipflop = 0;
            }else{
                flipflop = 1;
                printf(" ");
                if(foutput != NULL){
                    fprintf(foutput, " ");
                }
            }
            line++;
            if(i == 15){
                if(foutput != NULL){
                    fprintf(foutput, "\n");
                }
                printf("\n");
                printLine = 1;
            }
        }
        printf("\n%07x ", line);
    }

    free(content);
    free(content2);
    fclose(fptr);
    if(foutput != NULL){
        fclose(foutput); 
    }
    return 0;
} 