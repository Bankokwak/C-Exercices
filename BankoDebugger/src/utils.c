#include "../headers/utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int FileExist(const char *filepath){
    FILE *file;
    if ((file = fopen(filepath, "r")))
    {
        fclose(file);
        return 1;
    }
    return 0;
}

void hexToDecimal(char hexdata[], long *originaldata){
    *originaldata = strtol(hexdata, NULL, 16);
}

void decimalToHex(char hexdata[], long originaldata){
    sprintf(hexdata, "%lx", originaldata);
}

int SplitString(char stringToSplit[], char pattern[], int maxResult, int sizeStringResponse, char result[maxResult][sizeStringResponse]){
    int stringToSplitLen = strlen(stringToSplit);
    int patterLen = strlen(pattern);
    int iterPattern = 0;
    int indiceStartPatern = -1;
    int indiceLastPatern = -1;
    int indiceStartString = 0;

    int resultNumber = 0;

    for(int i = 0; i < stringToSplitLen; i++){
        for(int j = 0; j < patterLen; j++){
            if(stringToSplit[i] == pattern[j]){
                if(indiceStartPatern == -1){
                    indiceStartPatern = i;
                }

                iterPattern++;
                i++;
                
                if(iterPattern == patterLen){
                    if(indiceStartPatern - indiceStartString > sizeStringResponse){
                        printf("sizeStringResponse is too small for sizeResult\n");
                        exit(1);
                    }
                    int x = 0;
                    for(int y = indiceStartString; y < indiceStartPatern; y++){
                        result[resultNumber][x] = stringToSplit[y];
                        x++;
                    }
                    if(resultNumber + 1 > maxResult){
                        printf("maxResult is too small for resultNumber\n");
                        exit(1);
                    }
                    result[resultNumber][x] = '\0';
                    resultNumber++;

                    indiceStartString = i;
                    iterPattern = 0;
                    indiceStartPatern = -1;
                    indiceLastPatern = i;
                }
            }else{
                iterPattern=0;
                indiceStartPatern=-1;
                break;
            }
        }
    }

    if(indiceStartPatern - indiceStartString > sizeStringResponse){
        printf("sizeStringResponse is too small for sizeResult\n");
        exit(1);
    }
    int x = 0;
    for(int y = indiceLastPatern; y < stringToSplitLen; y++){
        result[resultNumber][x] = stringToSplit[y];
        x++;
    }
    if(resultNumber + 1 > maxResult){
        printf("maxResult is too small for resultNumber\n");
        exit(1);
    }
    result[resultNumber][x] = '\0';

    if(resultNumber == 0){
        strcpy(result[0], stringToSplit);
        
        
    }
    return resultNumber + 1;
}