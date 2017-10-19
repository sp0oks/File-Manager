#include "register.h"

void printhelp();
int readflags(int*, char**, char**, char**, char**, int, char**);

int main(int argc, char* argv[]){
    // Getopt related (flags, file path, iterators, etc.)
    int flags[6] = {0, 0, 0, 0, 0, 0}; // [n, s, r, i, c, l]
    char *sarg = NULL; // Search argument
    char *rarg = NULL; // Remove argument
    char *iarg = NULL; // Insert argument
    char *filepath = NULL; // File path argument

    int index;
    /*
     Checks if there's a file path argument
     If file path argument is missing, print the usage and exit with 1

     Options order:
     n > s > r > i > c > l

     So, if all the options are selected, the program will first create the file, then search it, remove the register, insert new registers, compact the file and finally list current registers in the file.
    */
    if(argc < 2){
        printhelp();
        return 1;
    }

    if(!readflags(flags, &sarg, &rarg, &iarg, &filepath, argc, argv))
        return 1;

    // Now for the operations, it's necessary to maintain the precedence

    // 1. Create a new file
    if(flags[0] == 1){
        // If it's not possible to create the file, quits the program
        if(!createfile(filepath)){
            printf("Failed to create file \"%s\"\n", filepath);
            return 1;
        }
        else
            printf("Created file \"%s\" successfully.\n",filepath);
    }

    // 2. Search register
    if(flags[1] == 1){
        reg regin;
        memset(&regin, 0, sizeof(regin));
        if(!searchreg(&regin, sarg, filepath)){
            printf("Search on \"%s\" failed\n", filepath);
            printf("Search was unable to find any registers related to the term '%s'\n", sarg);
            return 1;
        }
        else{
            printf("Register found!\n");
            writereg(regin);
        }
    }

    // 3. Remove register
    if(flags[2] == 1){
        if(removereg(rarg, filepath)){
          printf("Register with term '%s' found and removed on \"%s\".\n", rarg, filepath);
        }
        else{
          printf("Failed to remove register from \"%s\".\n", filepath);
          return 1;
        }
    }

    // 4. Insert registers
    if(flags[3] == 1){
        int regnum = (iarg == NULL) ? 0 : atoi(iarg);
        int error;
        if (regnum < 0){
          printf("Invalid option: cannot add %d registers to file.\n", atoi(iarg));
          return 1;
        }
        else if (regnum == 0){
          char ans;
          printf("Insert dummy registers into file '%s'? (y/n)\n", filepath);
          scanf("%c",&ans);
          if(ans == 'y'){
            printf("Choose number of dummy registers to add:\n");
            scanf("%d", &regnum);
            reg regin[regnum];
            memset(regin, 'F', sizeof(regin));
            for(index = 0; index < regnum; index++) error = insertreg(regin[index], filepath);
          }
          else{
            printf("Operation cancelled.\n");
            return 1;
          }
        }
        else{
          reg regin[regnum];
          for(index = 0; index < regnum; index++){
              readreg(&regin[index]);
              error = insertreg(regin[index], filepath);
            }
        }
        if(!error){
            printf("Failed to insert registers on file \"%s\".\n", filepath);
            return 1;
        }
    }

    // 5. Compact file
    if(flags[4] == 1)
        compactfile(filepath);

    // 6. List registers
    if(flags[5] == 1)
        listreg(filepath);

    return 0;
}

void printhelp(){
    printf("Usage: archiver [options] <file_path>\n");
    printf("Options:\n");
    printf("\t-n\t\t\tCreates a new file\n");
    printf("\t-i <reg_number>\t\tInserts <reg_number> registers into file\n");
    printf("\t-i 0\t\t\tInserts dummy registers into file\n");
    printf("\t-s <term>\t\tSearches for <term> in all registers in file\n");
    printf("\t-r <reg_key>\t\tRemoves register from file\n");
    printf("\t-c\t\t\tCompact file\n");
    printf("\t-l\t\t\tList all registers in file\n");
}

int readflags(int* flags, char** sarg, char** rarg, char** iarg, char** filepath, int argc, char** argv){
    int opt;

    // Reads all the options from command line
    // The options set flags to 1, and if an argument is needed, it's passed to a char pointers
    while((opt = getopt(argc, argv, "nclr:s:i:")) != -1){
        switch(opt){
            case 'n':
                flags[0] = 1;
                break;
            case 's':
                flags[1] = 1;
                *sarg = optarg;
                break;
            case 'r':
                flags[2] = 1;
                *rarg = optarg;
                break;
            case 'i':
                flags[3] = 1;
                *iarg = optarg;
                break;
            case 'c':
                flags[4] = 1;
                break;
            case 'l':
                flags[5] = 1;
                break;
        }
    }

    // Checks for the required filepath
    // Only reads the first value from argv
    if(optind < argc){
        *filepath = argv[optind];
        // *filepath = (char*)malloc(strlen(argv[optind]));
        // strcpy(*filepath, argv[optind]);
    }else{
        printhelp();
        printf("Invalid option: archiver takes exactly one filepath as parameter\n");
        return 0;
    }

    return 1;
}
