#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* formatScientificNotation(double number) {
    char numberStr[500];  // Adjust the size as needed
    snprintf(numberStr, sizeof(numberStr), "%.6e", number);
    //printf("%s\n", numberStr);
    char *ret = strstr(numberStr, "e");
    if(ret == NULL){
        return strdup(numberStr);
    }

    // Format the number in the desired format
    char resultString[50];  // Adjust the size as needed
    snprintf(resultString, sizeof(resultString), "%.6e", number);

    // Extract the exponent part from the formatted string
    char* exponentStr = strchr(resultString, 'e');
    //printf("%s\n", exponentStr);
    if (exponentStr != NULL) {
        *exponentStr = '*';
        
        // Move the exponent value to a separate string
        char exponentValue[10];
        strcpy(exponentValue, exponentStr + 1);
        
        // Remove the 'e' from the original string
        memmove(exponentStr, exponentStr + 2, strlen(exponentStr + 2) + 1);
        
        // Concatenate the '*10^{' and the exponent value
        strcat(resultString, "*10^{");
        strcat(resultString, exponentValue);
        strcat(resultString, "}");
    }

    // Dynamically allocate memory for the result string
    return strdup(resultString);
}

//sed -i 's/^[ ]*//' file.txt
//sed -i 's/endfacet//' file.txt

// Function to trim leading spaces from a string
char* trimLeadingSpaces(char* str) {
    while (*str == ' ' || *str == '\t') {
        str++;
    }
    return str;
}

void process_stl(char *stl){
    FILE *file;
    char line[1000];
    double x, y, z;
    int firstEntry = 1; // Variable to track the first entry in a line
    
    if ((file = fopen(stl, "r")) == NULL) {
        fprintf(stderr, "Error opening file.\n");
    }

    while (fgets(line, sizeof(line), file) != NULL) {

        char *trimmedline = trimLeadingSpaces(line);

        if(trimmedline[0] == 'v'){
            if (sscanf(trimmedline, "vertex %lf %lf %lf", &x, &y, &z) == 3) { // if we got a line with vertexes
                if (!firstEntry) {
                    printf(", ");
                }
                printf("(%s, %s, %s)", formatScientificNotation(x), formatScientificNotation(y), formatScientificNotation(z));
                firstEntry = 0; // Set to 0 after printing the first entry
            }
        } else if (trimmedline[0] == 'e') { //if we got an endfacet
            if (fgets(line, sizeof(line), file) == NULL) {
                // If it's the last line of the file, don't print "\\right)"
                break;
            }
            printf("\\right)\n");
            firstEntry = 1; // Reset for the next line
        } else if (trimmedline[0] == 'f') { // if we have "facet normal"
            printf("\\triangle\\left");
        } else if (trimmedline[0] == '\n') { // Skip over empty lines
            printf("\n");
        }
    }

    fclose(file);
}

int main(int argc, char* argv[]) {

    // First, user must run
    //sed -i 's/^[ ]*//' file.stl
    //sed -i 's/^endfacet//' file.stl
    if (strstr(argv[1], ".stl") == NULL)
    {
        printf("This ain't an stl file... ruh roh...\n");
        return 1;
    }
    process_stl(argv[1]);
    return 0;
}