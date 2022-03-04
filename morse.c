#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SLASH '/'
#define SEPARATOR ' '
#define INVALID '*'
#define ERROR '#'

typedef struct {
    char code[9];
} Morse;

// Create an empty morse
Morse *newMorse() {
    Morse *morse;
    // 000000
    // Allocate memory for morse
    morse = (Morse *)malloc(sizeof(Morse));
    // Set NULL value for morse
    // memset is faster than for loop as it deals with multiple bytes at a time
    memset(morse->code, 0, 9);
    return morse;
}

#define NUM_COUNT 10
// Declare morse code for numbers
char Num[][5] = {
    {'-', '-', '-', '-', '-'},  // 0
    {'.', '-', '-', '-', '-'},  // 1
    {'.', '.', '-', '-', '-'},  // 2
    {'.', '.', '.', '-', '-'},  // 3
    {'.', '.', '.', '.', '-'},  // 4
    {'.', '.', '.', '.', '.'},  // 5
    {'-', '.', '.', '.', '.'},  // 6
    {'-', '-', '.', '.', '.'},  // 7
    {'-', '-', '-', '.', '.'},  // 8
    {'-', '-', '-', '-', '.'}   // 9
};

// Encode a char number to morse
// num: char number ('0' -> '9')
// morse: result after encoding
int encodeNum(char num, Morse *morse) {
    // '0' = 48 -> '9' = 57
    // Get position of number in number array
    int pos = num - '0';

    if (pos >= 0 && pos <= 9) {
        // Copy the morse code of corresponding num
        memcpy(morse->code, Num[pos], 5);
        return 1;
    }

    return 0;
}

// Decode a char number
// morse: morse code
// num: character after decoding
int decodeNum(Morse *morse, char *num) {
    // Loop through the number morse code array
    for (int i = 0; i < NUM_COUNT; i++) {
        // Compare number morse code
        // memcmp(str1, str2, n): compare n.st char of str1 and str2
        if (memcmp(morse->code, Num[i], 5) == 0) {
            // On finding the corresponding morse code,
            // num = i + '0'
            *num = (char)(i + '0');
            return 1;
        }
    }
    return 0;
}

#define MARK_COUNT 7
// Declare morse code for mark
// 6 first char is morse code, the 7th char is corresponding mark to preceding morse
char Mark[][7] = {
    {'.', '-', '.', '-', '.', '-', '.'},  // .
    {'-', '-', '-', '.', '.', '.', ':'},  // :
    {'-', '-', '.', '.', '-', '-', ','},  // ,
    {'-', '.', '-', '.', '-', '.', ';'},  // ;
    {'.', '.', '-', '-', '.', '.', '?'},  // ?
    {'-', '.', '.', '.', '.', '-', '-'}   // -
};

// Check if a character is mark
int isMark(char c) {
    return (c == '.' || c == '-' || c == ':' || c == ';' || c == '?' || c == ',');
}

// mark: character to be encoded
// morse: morse after encoding
int encodeMark(char mark, Morse *morse) {
    // Loop through the mark array
    for (int i = 0; i < MARK_COUNT; i++) {
        // Look for the corresponding mark
        if (mark == Mark[i][6]) {
            // On finding the matching mark, copy the code to morse
            memcpy(morse->code, Mark[i], 6);
            return 1;
        }
    }
    return 0;
}

// morse: morse to be decoded
// mark: character after decoding
int decodeMark(Morse *morse, char *mark) {
    // Loop through the mark array
    for (int i = 0; i < MARK_COUNT; i++) {
        // Look for the corresponding morse
        if (memcmp(morse->code, Mark[i], 6) == 0) {
            // On finding the matching morse, copy the char to mark
            *mark = Mark[i][6];
            return 1;
        }
    }
    return 0;
}

#define LETTER_COUNT 26
// Declare morse code for character
char Letter[][4] = {
    {'.', '-', '*', '*'},  // a
    {'-', '.', '.', '.'},  // b
    {'-', '.', '-', '.'},  // c
    {'-', '.', '.', '*'},  // d
    {'.', '*', '*', '*'},  // e
    {'.', '.', '-', '.'},  // f
    {'-', '-', '.', '*'},  // g
    {'.', '.', '.', '.'},  // h
    {'.', '.', '*', '*'},  // i
    {'.', '-', '-', '-'},  // j
    {'-', '.', '-', '*'},  // k
    {'.', '-', '.', '.'},  // l
    {'-', '-', '*', '*'},  // m
    {'-', '.', '*', '*'},  // n
    {'-', '-', '-', '*'},  // o
    {'.', '-', '-', '.'},  // p
    {'-', '-', '.', '-'},  // q
    {'.', '-', '.', '*'},  // r
    {'.', '.', '.', '*'},  // s
    {'-', '*', '*', '*'},  // t
    {'.', '.', '-', '*'},  // u
    {'.', '.', '.', '-'},  // v
    {'.', '-', '-', '*'},  // w
    {'-', '.', '.', '-'},  // x
    {'-', '.', '-', '-'},  // y
    {'-', '-', '.', '.'}   // z
};

// Encode a single letter
// m: letter to be encoded
// morse: result after encoding
int encodeLetter(char m, Morse *morse) {
    // 'a' = 97, 'z' = 122
    // Get position of letter in char array
    int pos = m - 'a';

    // Encode if able
    if (pos >= 0 && pos <= 25) {
        memcpy(morse->code, Letter[pos], 4);
        return 1;
    }
    return 0;
}

// Decode a single letter
// morse: morse to decode
// ch: result after decoding
int decodeLetter(Morse *morse, char *ch) {
    // Loop through the letter morse code array
    for (int i = 0; i < LETTER_COUNT; i++) {
        // Compare letter morse code
        if (memcmp(morse->code, Letter[i], 4) == 0) {
            *ch = (char)(i + 'a');
            return 1;
        }
    }
    return 0;
}

// Declare string error
char error[] = "........";

// Check if a morse is error
int isError(Morse *morse) {
    return strcmp(morse->code, error) == 0;
}

// Decode whole string
// morse: morse to be decoded
// plain: string to store decode result
void decode(char *morse, char *plain, int lineNumber) {
    Morse *temp = newMorse();
    int j = 0;
    int k = 0;
    char ch = '*';
    memset(temp->code, '*', 8);

    // Loop through each character in morse code
    for (int i = 0; i < strlen(morse); i++) {
        // If current character is not ' ' or '/'
        if (morse[i] != SEPARATOR && morse[i] != SLASH && morse[i] != '\n') {
            // Copy the current character to morse code
            temp->code[j++] = morse[i];
        }
        // If current character is ' ' or '/'
        else if ((morse[i] == SEPARATOR && morse[i - 1] != SLASH) && temp->code[0] != '*') {
            // Decode the copied morse
            // If it is a letter
            if (j < 5 && decodeLetter(temp, &ch)) {
                plain[k++] = ch;
            }
            // If it is a number
            else if (j < 6 && decodeNum(temp, &ch)) {
                plain[k++] = ch;
            }
            // If it is a mark
            else if (decodeMark(temp, &ch)) {
                plain[k++] = ch;
            }
            // If it is error
            else if (isError(temp)) {
                plain[k++] = ERROR;
            }
            // If it is invalid
            else {
                printf("Error: Invalid Morse code \"");
                for (int i = 0; i < 9 && temp->code[i] != '*'; i++) {
                    printf("%c", temp->code[i]);
                }
                printf("\" on line %d.\n", lineNumber);
                plain[k++] = INVALID;
            }
            // Reset temporary morse
            j = 0;
            memset(temp->code, '*', 8);
        }
        // If current is a '/'
        else if (morse[i] == SLASH) {
            plain[k++] = ' ';
        }
        // End of line
        else if (morse[i] == '\n') {
            if (temp->code[0] != '*') {
                // Decode the copied morse
                // If it is a letter
                if (j < 5 && decodeLetter(temp, &ch)) {
                    plain[k++] = ch;
                }
                // If it is a number
                else if (j < 6 && decodeNum(temp, &ch)) {
                    plain[k++] = ch;
                }
                // If it is a mark
                else if (decodeMark(temp, &ch)) {
                    plain[k++] = ch;
                }
                // If it is error
                else if (isError(temp)) {
                    plain[k++] = ERROR;
                }
                // If it is invalid
                else {
                    printf("Error: Invalid Morse code \"");
                    for (int i = 0; i < 9 && temp->code[i] != '*'; i++) {
                        printf("%c", temp->code[i]);
                    }
                    printf("\" on line %d.\n", lineNumber);

                    plain[k++] = INVALID;
                }
            }
            // Reset temporary morse
            j = 0;
            memset(temp->code, '*', 8);

            plain[k++] = '\n';
        }
    }

    if (temp->code[0] != '*') {
        // Decode the copied morse
        // If it is a character
        if (j < 5 && decodeLetter(temp, &ch)) {
            plain[k++] = ch;
        }
        // If it is a number
        else if (j < 6 && decodeNum(temp, &ch)) {
            plain[k++] = ch;
        }
        // If it is a mark
        else if (decodeMark(temp, &ch)) {
            plain[k++] = ch;
        }
        // If it is error
        else if (isError(temp)) {
            plain[k++] = ERROR;
        }
        // If it is invalid
        else {
            printf("Error: Invalid Morse code \"");
            for (int i = 0; i < 9 && temp->code[i] != '*'; i++) {
                printf("%c", temp->code[i]);
            }
            printf("\" on line %d.\n", lineNumber);
            plain[k++] = INVALID;
        }
    }
}

// Encode whole string
// string: string to be encoded
// morse: morse string after encoding
char *encode(char *string, int lineNumber) {
    int j = 0;
    int len = BUFSIZ;
    char *cipher = NULL;
    Morse *morse = newMorse();

    // Allocate memory for cipher string
    cipher = realloc(cipher, len);

    // Loop through the string
    for (int i = 0; i < strlen(string); i++) {
        if (string[i] != ' ') {
            // If it is a character
            if (string[i] >= 'a' && string[i] <= 'z') {
                if (encodeLetter(string[i], morse)) {
                    for (int k = 0; k < 4 && (morse->code[k] != '*'); k++) {
                        cipher[j++] = morse->code[k];

                        // In case reach length limit, broaden cipher string
                        if (j >= len - 1) {
                            len += BUFSIZ;
                            cipher = realloc(cipher, len);
                        }
                    }
                }
            }
            // If current character is a number
            else if (string[i] >= '0' && string[i] <= '9') {
                if (encodeNum(string[i], morse)) {
                    for (int k = 0; k < 5; k++) {
                        cipher[j++] = morse->code[k];
                        if (j >= len - 1) {
                            len += BUFSIZ;
                            cipher = realloc(cipher, len);
                        }
                    }
                }
            }
            // If current character is a mark
            else if (isMark(string[i])) {
                if (encodeMark(string[i], morse)) {
                    for (int k = 0; k < 6; k++) {
                        cipher[j++] = morse->code[k];
                        if (j >= len - 1) {
                            len += BUFSIZ;
                            cipher = realloc(cipher, len);
                        }
                    }
                }
            } else if (string[i] == '\n') {
                cipher[j++] = '\n';
                if (j >= len - 1) {
                    len += BUFSIZ;
                    cipher = realloc(cipher, len);
                }
            } else {
                printf("Error: Unrecognised character \"%c\" on line %d.\n", string[i], lineNumber);
            }
            // Reset temporary morse
            memset(morse->code, '*', 8);
            cipher[j++] = SEPARATOR;
            if (j >= len - 1) {
                len += BUFSIZ;
                cipher = realloc(cipher, len);
            }
        }
        // On finding a space
        else if (string[i] == ' ') {
            cipher[j++] = SLASH;
            if (j >= len - 1) {
                len += BUFSIZ;
                cipher = realloc(cipher, len);
            }
            // cipher[j++] = SEPARATOR;
            // if (j >= len - 1) {
            //     len += BUFSIZ;
            //     cipher = realloc(cipher, len);
            // }
        }
    }

    cipher[j - 1] = '\0';
    return cipher;
}

// Lowercase a string
void toLowerStr(char str[]) {
    for (int i = 0; i < strlen(str); i++) {
        str[i] = tolower(str[i]);
    }
}

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