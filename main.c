/*

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "register.h"

void printhelp();
int readflags(int*, char**, char**, char**, char**, int, char**);

int main(int argc, char* argv[]){
    // Getopt related (flags, file path, iterators, etc.)
    int flags[5] = {0, 0, 0, 0, 0}; // [n, s, r, i, c]
    char *sarg = NULL; // Search argument
    char *rarg = NULL; // Remove argument
    char *iarg = NULL; // Insert argument
    char *filepath = NULL; // File path argument   

    // Insertion related
    unsigned int regnum; // Number of register to insert
    reg regin; // Array of registers to insert

    int index;

    // Checks if there's a file path argument
    // If file path argument is missing, print the usage and exit with 1
    /*
        Options order:
        n > s > r > i > c
        
        So, if all the options are selected, the program will first create the file, then search it, remove the register, insert new registers, and then compact the file.
    */
    if(argc < 2){
        printhelp();
        return 1;
    }

    if(!readflags(flags, &sarg, &rarg, &iarg, &filepath, argc, argv))
        return 1;

    // Now for the operations, it's necessary to mantain the precedence
    // 0. If no option flag is set, only list register
    if(flags[0] != 0 && flags[1] != 0 && flags[2] != 0 && flags[3] != 0 && flags[4] != 0){
        listreg(filepath); 

        // Free all argument strings       
        free(sarg);
        free(rarg);
        free(iarg);
        free(filepath);

        return 0;
    }

    // 1. Create a new file
    if(flags[0] == 1){
        // If it's not possible to create the file, quits the program
        if(!createfile(filepath)){
            printf("Failed to create file %s\n", filepath);
            return 1;
        }
        printf("Created file successfully\n");
    }
    
    // 2. Search register
    if(flags[1] == 1){
        if(!searchreg(&regin, sarg, filepath)){
            printf("Search on %s failed\n", filepath);
            return 1;
        }
        
        printf("%s %s %s %s", regin.key, regin.title, regin.author, regin.link);
        
        free(sarg);
    }
    
    // 3. Remove register
    if(flags[2] == 1){
        removereg(rarg, filepath);
        free(rarg);
    }

    // 4. Insert registers
    if(flags[3] == 1){
        regnum = (iarg == NULL)?1:(atoi(iarg)==0)?1:atoi(iarg);
        for(index = 0; index < regnum; index++){
            readreg(&regin);
            insertreg(regin, filepath);
        }
        free(iarg);
    }
    
    // 5. Compact file
    if(flags[4] == 1){
        compactfile(filepath);
        free(filepath);
    }   

    return 0;
}

void printhelp(){
    printf("usage: archiver [options] file_path\n");
    printf("\toptions:\n");
    printf("\t\t-n\t\t\tCreates a new file\n");
    printf("\t\t-i[number_registers]\tInserts registers into file\n");
    printf("\t\t-s register_key\t\tSearch register into file\n");
    printf("\t\t-r register_key\t\tRemoves register\n");
    printf("\t\t-c\t\t\tCompact file\n");
}

int readflags(int* flags, char** sarg, char** rarg, char** iarg, char** filepath, int argc, char** argv){
    int opt;    

    // Reads all the options from command line
    // The options set flags to 1, an if a argument is needed, it's passed to a char*
    while((opt = getopt(argc, argv, "ncr:s:i::")) != -1){
        switch(opt){
            case 'n':
                flags[0] = 1;
                break;
            case 'c':
                flags[4] = 1;
                break;
            case 'r':
                flags[2] = 1;
                *rarg = malloc(strlen(optarg));
                strcpy(*rarg, optarg);
                break;
            case 's':
                flags[1] = 1;
                *sarg = malloc(strlen(optarg));
                strcpy(*sarg, optarg);
                break;
            case 'i':
                flags[3] = 1;
                *iarg = malloc(strlen(optarg));
                strcpy(*iarg, optarg);
                break;
        }
    }

    // Checks for the required filepath
    // Only reads the first value from argv
    if(optind < argc){
        *filepath = malloc(strlen(argv[optind]));
        strcpy(*filepath, argv[optind]);
    }else{
        printhelp();
        return 0;
    }

    return 1;
}
