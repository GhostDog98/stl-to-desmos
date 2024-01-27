#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// snprintf result
/*
1: ./stltodesmos tester-whole.stl outfile.txt
            Mean        Std.Dev.    Min         Median      Max
real        5.523       0.673       4.670       5.256       8.880       
user        4.288       0.078       4.171       4.278       5.264       
sys         0.471       0.037       0.353       0.472       0.572       
*/
// spintf result
/*
            Mean        Std.Dev.    Min         Median      Max
real        5.354       0.388       4.697       5.241       8.626       
user        4.287       0.069       4.180       4.280       4.749       
sys         0.477       0.036       0.336       0.480       0.560       

*/

/* A realloc-based function by Albert Chan <albertmcchan@yahoo.com>, sent to Laird Shaw
 * privately, and reproduced here with Albert's permission as public domain
 * code, with Laird Shaw's only edit being to qualify the parameters as const.
 */
char* replace_smart (const char *str, const char *sub, const char *rep)
{  
    size_t slen = strlen(sub);
    size_t rlen = strlen(rep);
    size_t size = strlen(str) + 1;
    size_t diff = rlen - slen;
    size_t capacity = (diff>0 && slen) ? 2 * size : size;
    char *buf = malloc(capacity);
    char *find, *b = buf;
    
    if (b == NULL) return NULL;
    if (slen == 0) return memcpy(b, str, size);
  
    while((find = strstr(str, sub))) {
        if ((size += diff) > capacity) {
            char *ptr = realloc(buf, capacity = 2 * size);
            if (ptr == NULL) {free(buf); return NULL;}
            b = ptr + (b - buf);
            buf = ptr;
        }
        memcpy(b, str, find - str); /* copy up to occurrence */
        b += find - str;
        memcpy(b, rep, rlen);       /* add replacement */
        b += rlen;
        str = find + slen;
    }
    memcpy(b, str, size - (b - buf));
    b = realloc(buf, size);         /* trim to size */
    return b ? b : buf;
}

char* formatScientificNotation(char *number) {
    // Quit early if we don't have an 'e' number
    if(strstr(number, "e") == NULL){
        return number;
    }
    // Also quit early if we have something that's 0.0 and has an exponent... for whatever reason...
    if(number == "0.0"){
        return number;
    }

    // Now we do expensive shit...
    strcpy(number, replace_smart(number, "e", "*10^{"));

    // This is so stupid... but it's fast!
    strcpy(number, replace_smart(number, "{-0", "{-"));
    strcpy(number, replace_smart(number, "{0", "{"));
    strcat(number, "}");
    return number;
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
    char x[100], y[100], z[100];
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
            //printf("Got trimmed vertex of %s ", trimmedline);
            if (sscanf(trimmedline, "vertex %s %s %s", &x, &y, &z) == 3) {
                if (!firstEntry) {
                    fprintf(outputFile, ", ");
                }
                

                fprintf(outputFile, "(%s, %s, %s)", formatScientificNotation(x), formatScientificNotation(y), formatScientificNotation(z));
                //printf("output of (%s, %s, %s)\n", x, y, z);
                firstEntry = 0;
            }
        } else if (trimmedline[0] == 'e') {
            if (fgets(line, sizeof(line), file) == NULL) {
                break;
            }
            fprintf(outputFile, "\\right)\n");
            firstEntry = 1;
        } else if (trimmedline[0] == 'f') {
            fprintf(outputFile, "\\triangle\\left(");
        } else if (trimmedline[0] == '\n') {
            fprintf(outputFile, "\n");
        }
    }
    fprintf(outputFile, "\n"); // To remain byte-compatable with the python version
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
