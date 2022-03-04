#include <stdio.h>

// Read a line without knowing length
char *getline(FILE *f) {
    int size = 0;
    int len = 0;
    int last = 0;
    char *buf = NULL;

    do {
        size += BUFSIZ;
        buf = realloc(buf, size);
        if (buf == NULL) return NULL;
        fgets(buf + last, BUFSIZ, f);
        len = strlen(buf);
        last = len - 1;
    } while (!feof(f) && buf[last] != '\n');

    return buf;
}

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

int main(int argc, char **argv) {
    char *fileIn = argv[1];
    char *fileOut = argv[2];

    FILE *read = fopen(fileIn, "r");
    int type = isMorseFile(read);

    if(type){
        printf("Morse\n");
    }else{
        printf("Text\n");
    }
}