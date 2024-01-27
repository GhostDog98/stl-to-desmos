#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
char* formatScientificNotation(double number) {
    char numberStr[500];  // Adjust the size as needed
    snprintf(numberStr, sizeof(numberStr), "%.6e", number);

    char *ret = strstr(numberStr, "e");
    if(ret == NULL){
        return strdup(numberStr);
    }

    char resultString[50];  // Adjust the size as needed
    snprintf(resultString, sizeof(resultString), "%.6e", number);

    char* exponentStr = strchr(resultString, 'e');
    if (exponentStr != NULL) {
        *exponentStr = '*';

        char exponentValue[10];
        strcpy(exponentValue, exponentStr + 1);
        memmove(exponentStr, exponentStr + 2, strlen(exponentStr + 2) + 1);
        strcat(resultString, "*10^{");
        strcat(resultString, exponentValue);
        strcat(resultString, "}");
    }

    return strdup(resultString);
}

char* trimLeadingSpaces(char* str) {
    while (*str && isspace(*str)) {
        str++;
    }
    return str;
}


void process_stl(char *stl, const char *outputFileName) {
    FILE *file, *outputFile;
    char line[1000];
    double x, y, z;
    int firstEntry = 1;
    
    if ((file = fopen(stl, "r")) == NULL) {
        fprintf(stderr, "Error opening input file.\n");
        return;
    }

    if ((outputFile = fopen(outputFileName, "w")) == NULL) {
        fprintf(stderr, "Error opening output file.\n");
        fclose(file);
        return;
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        char *trimmedline = trimLeadingSpaces(line);

        if(trimmedline[0] == 'v'){
            if (sscanf(trimmedline, "vertex %lf %lf %lf", &x, &y, &z) == 3) {
                if (!firstEntry) {
                    fprintf(outputFile, ", ");
                }
                fprintf(outputFile, "(%s, %s, %s)", formatScientificNotation(x), formatScientificNotation(y), formatScientificNotation(z));
                firstEntry = 0;
            }
        } else if (trimmedline[0] == 'e') {
            if (fgets(line, sizeof(line), file) == NULL) {
                break;
            }
            fprintf(outputFile, "\\right)\n");
            firstEntry = 1;
        } else if (trimmedline[0] == 'f') {
            fprintf(outputFile, "\\triangle\\left");
        } else if (trimmedline[0] == '\n') {
            fprintf(outputFile, "\n");
        }
    }

    fclose(file);
    fclose(outputFile);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s input.stl output.txt\n", argv[0]);
        return 1;
    }

    if (strstr(argv[1], ".stl") == NULL) {
        fprintf(stderr, "This ain't an stl file... ruh roh...\n");
        return 1;
    }

    process_stl(argv[1], argv[2]);
    return 0;
}
