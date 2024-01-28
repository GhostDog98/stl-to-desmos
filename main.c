// Copyright 2024 Lilly Aronleigh
// Licensed under GNU GPL v3
/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.


*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void print_gnu_gpl() {
    printf(""
    "stltodesmos V3.0\n"
    "Copyright (C) 2024  Lilly Aronleigh\n"

    "This program is free software: you can redistribute it and/or modify\n"
    "it under the terms of the GNU General Public License as published by\n"
    "the Free Software Foundation, either version 3 of the License, or\n"
    "(at your option) any later version.\n"

    "This program is distributed in the hope that it will be useful,\n"
    "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
    "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
    "GNU General Public License for more details.\n"
    "\n"
    "You should have received a copy of the GNU General Public License\n"
    "along with this program.  If not, see <https://www.gnu.org/licenses/>.\n");
}


/* A realloc-based function by Albert Chan <albertmcchan@yahoo.com>, sent to Laird Shaw
 * privately, and reproduced here with Albert's permission as public domain
 * code, with Laird Shaw's only edit being to qualify the parameters as const.
 */
char* replace_smart(const char *str, const char *sub, const char *rep) {
    size_t slen = strlen(sub);
    size_t rlen = strlen(rep);
    size_t size = strlen(str) + 1;
    size_t diff = rlen - slen;
    size_t capacity = (diff > 0 && slen) ? 2 * size : size;
    char *buf = malloc(capacity);
    char *find, *b = buf;
    if (b == NULL) return NULL;
    if (slen == 0) return memcpy(b, str, size);

    while ((find = strstr(str, sub))) {
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
    if (strstr(number, "e") == NULL) {
        return number;
    }
    // Also quit early if we have something that's 0.0 and has an exponent... for whatever reason...
    if (number == "0.0") {
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

        if (trimmedline[0] == 'v') {
            if (sscanf(trimmedline, "vertex %s %s %s", &x, &y, &z) == 3) {
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
            fprintf(outputFile, "\\triangle\\left(");
        } else if (trimmedline[0] == '\n') {
            fprintf(outputFile, "\n");
        }
    }
    fprintf(outputFile, "\n");  // To remain byte-compatable with the python version
    fclose(file);
    fclose(outputFile);
}

int check_stl_type(char *filename) {
    FILE* fptr;

    fptr = fopen(filename, "r");

    if (fptr == NULL) {
        printf("Failed to open file... Do we have read permissions?\n");
    }

    // Loop to read required bytes from file
    char c;
    char str[6];
    for (int i = 0; i <= 5; i++) {
        c = fgetc(fptr);
        str[i] = c;
    }
    if (strstr(str, "solid")) {
        return 0;
    } else {
        return 1;
    }
}

int main(int argc, char* argv[]) {
    print_gnu_gpl();

    if (argc != 3) {
        fprintf(stderr, "Usage: %s input.stl output.txt\n", argv[0]);
        return 1;
    }

    printf("stltodesmos Copyright (C) 2024  Lilly Aronleigh (Forked from WagyuDEV's desmos3d)"
    "This program comes with ABSOLUTELY NO WARRANTY;"
    "This is free software, and you are welcome to redistribute it"
    "under certain conditions."
    "Please use `--license` to see the full license text.\n");



    // todo -- detect stl file type (binary vs ascii)
    // ascii starts with bytes `73 6F 6C 69 64` (solid)


    if (strstr(argv[1], ".stl") == NULL) {
        fprintf(stderr, "This ain't an stl file... ruh roh...\n");
        return 1;
    } else {
        if (check_stl_type(argv[1]) == 1) {
            printf("stl isn't of an ascii type... Aborting!\n");
            return 1;
        }
    }

    process_stl(argv[1], argv[2]);
    return 0;
}

