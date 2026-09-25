#ifndef UTILS_H
#define UTILS_H

int FileExist(const char *filepath);
void hexToDecimal(char hexdata[], long *originaldata);
void decimalToHex(char hexdata[], long originaldata);

int SplitString(char stringToSplit[], char pattern[], int maxResult, int sizeStringResponse, char result[maxResult][sizeStringResponse]);

#endif