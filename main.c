#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "morse.c"

// Check if a character is morse char
int isMorse(char c) {
    return (c == '.' || c == '-' || c == '/' || c == ' ');
}

// Check if a line is morse line
int isMorseLine(char *str) {
    for (int i = 0; i < strlen(str); i++) {
        if (!isMorse(str[i])) {
            return 0;
        }
    }
    return 1;
}

// Check if a file is morse file
int isMorseFile(FILE *file) {
    char *str;
    while (!feof(file)) {
        str = getline(file);
        if (str[strlen(str) - 1] == '\n') {
            str[strlen(str) - 1] = '\0';
        }
        if (!isMorseLine(str)) {
            return 0;
        }
    }
    return 1;
}

// argument count
// argument vector
int main(int argc, char **argv) {
    // Check argument count
    if (argc != 3) {
        printf("Error: Invalid arguments count.\n");
        return 0;
    }
    // Get file in path
    char *fileIn = argv[1];
    // Get file out path
    char *fileOut = argv[2];

    // Read file in
    FILE *read = fopen(fileIn, "r");
    // In case unreadable
    if (read == NULL) {
        printf("Error: %s could not be opened.\n", fileIn);
    }
    FILE *write;

    // Check if file out has already exsisted
    if (access(fileOut, F_OK) == 0) {
        printf("Warning: %s already exists. Do you wish to overwrite (y,n)? ", fileOut);
        char option;
        scanf("%c", &option);

        // Ovewrite
        if (option == 'y') {
            write = fopen(fileOut, "w");
        }
        // Exit
        else {
            printf("Exit.\n");
            return 1;
        }
    }
    // File not exist
    else {
        write = fopen(fileOut, "w");
    }

    char *str;
    int lineNumber = 1;
    char plain[1024] = "";

    // Check type of file
    int type = isMorseFile(read);

    // After checking type, the file pointer has reached the end of file
    // Move the file pointer to the beginning of file
    fseek(read, 0, SEEK_SET);

    // Is morse file
    if (type) {
        while (!feof(read)) {
            str = getline(read);
            if (str[strlen(str) - 1] == '\n') {
                str[strlen(str) - 1] = '\0';
            }
            // Reset value for plain
            memset(plain, 0, 1024);
            decode(str, plain, lineNumber++);
            printf("%s\n", plain);
            fprintf(write, "%s\n", plain);
        }
    }
    // Is text file
    else {
        printf("Encode\n");
        while (!feof(read)) {
            str = getline(read);
            // Lowercase a string
            toLowerStr(str);
            // Encode the string and assign to cipher
            char *cipher = encode(str, lineNumber++);
            printf("%s", cipher);
            fprintf(write, "%s", cipher);
        }
    }

    fclose(read);
    fclose(write);
}