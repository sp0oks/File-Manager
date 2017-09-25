#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define REGSIZE 84

// Defines the format of the register
// Register for an archiver of book download links
// Total number of bytes: 84 bytes
typedef struct{
    char key[4]; // 4 bytes
    char title[30]; // 30 bytes
    char author[30]; // 30 bytes
    char link[20]; // 20 bytes [better to story shortened link]
} reg;

// Register related operations
int searchreg(reg*, char*, char*);
int insertreg(reg, char*);
int listreg(char*);
int removereg(char*, char*);
int readreg(reg*);

// General file operations
int createfile(char*);
int compactfile(char*);
