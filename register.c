#include "register.h"

/*
    Invalid register will have a starting byte 7F (ASCII Del), the relative address of the next invalid register (in register units, not bytes);
*/

int searchreg(reg* regout, char* key, char* filepath){
    FILE* fp = fopen(filepath, "rb");
    char regaux[REGSIZE];

    if(!fp)
        return 0;

    while(fread(regaux, REGSIZE, 1, fp)){
        // Checks if keys match and if the register is valid
        if(!strncmp(regaux, key, 4) && strncmp(regaux, "\x7F", 1)){
            strncpy(regout->key, regaux, 4);
            strncpy(regout->title, regaux+4, 30);
            strncpy(regout->author, regaux+34, 30);
            strncpy(regout->link, regaux+64, 20);

            return 1;
        }
    }
    
    fclose(fp);
    return 0;
}

int insertreg(reg regin, char* filepath){
    return 1;
}

int listreg(char* filepath){
    return 1;
}

int removereg(char* key, char* filepath){
    return 1;
}

int readreg(reg* regout){
    return 1;
}

int createfile(char* filepath){
    FILE* fp = fopen(filepath, "wb");
    
    if(!fp)
        return 0;
    
    fclose(fp);
    return 1;
}

int compactfile(char* filepath){
    return 1;
}
