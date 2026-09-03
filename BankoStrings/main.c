#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int CheckValideASCII(const unsigned char value){
    if(
        (value >= 0x20 && value <= 0x7d)
    ){
        return 1;
    }else{
        return 0;
    }
}

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

int main(
    int argc,
    char *argv[],
    char **envp
){
    const char *pathFile = NULL;
    const char *outputFile = NULL;

    if(argc < 2){
        printf("Usage: bankostrings [filepath] {-o}");
        return -1;
    }
    if(FileExist(argv[1]) == 1){
        pathFile = argv[1];
    }
    for(int i = 0; i < argc; i++){
        if (strcmp(argv[i], "-o") == 0 ||
            strcmp(argv[i], "--output") == 0) {

            if (i + 1 >= argc) {
                fprintf(stderr, "Missing output file\n");
                return 1;
            }
            outputFile = argv[++i];
        }
    }

    FILE *fptr = NULL;
    FILE *foutput = NULL;
    if(outputFile != NULL){
        foutput = fopen(outputFile, "w");
    }
    fptr = fopen(pathFile, "rb");
    if(fptr == NULL) { return 1; }

    long size = get_file_size(pathFile);
    if(size == -1) { fclose(fptr); return 1; }
    unsigned char *content = malloc(size + 1);
    size_t read = fread(content, 1, size, fptr);

    int startString = -1;
    int endString = -1;
    for(int i = 0; i < read; i++){
        if(CheckValideASCII(content[i])){
            if(startString == -1){
                startString = i;
            }
            continue;
        }else if(startString != -1){
            endString = i - 1;

            if(endString - startString == 0 || endString - startString == 1 || endString - startString == 2){
                startString = -1;
                endString = -1;
                continue;
            }
            
            size_t size = endString - startString + 1;
            char output[size + 1];
            int sizeOutput = 0;
            for(int i=startString; endString >= i;i++){
                printf("%c", content[i]);
                output[sizeOutput] = content[i];
                sizeOutput++;
            }
            output[sizeOutput] = '\0';
            if(foutput != NULL){
                fprintf(foutput, "%s\n", output);
            }
            printf("\n");

            startString = -1;
            endString = -1;
        }
    }

    free(content);
    fclose(fptr);
    if(foutput != NULL){
        fclose(foutput); 
    }

    return 0;
}