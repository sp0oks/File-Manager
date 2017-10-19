#include "register.h"

/*
 * Invalid register will have a starting byte 7F (ASCII Del), the relative address of the next invalid register (in register units, not bytes);
 */

int searchreg(reg* regout, char* searchstr, char* filepath){
    FILE* fp = fopen(filepath, "rb");
    char block_temp[BLKSIZE]; // Buffer to store raw blocks
    char reg_temp[REGSIZE]; // Buffer to store raw registers
    char buffer[128]; // Buffer to format character arrays into strings

    if(!fp)
        return 0;
    else{
        while(fread(block_temp, BLKSIZE, 1, fp)){
          // Checks if search term matches any register in the block and if it is valid (not removed)
          if (!strncmp(block_temp, "#!", 2)){
            int OFFBLK = 2; // Offset variable to jump between registers
            int found = 0;
            while (OFFBLK < BLKSIZE && !found){
              if (strncmp(block_temp+OFFBLK,"\x7F",1) && strncmp(block_temp+OFFBLK,"\0", 1)){
                int OFFREG = 0; // Offset variable to jump between register fields
                // Copy REGSIZE number of bits from temporary block buffer to temporary register buffer
                memcpy(reg_temp, block_temp+OFFBLK, REGSIZE);
                // Clear comparing buffer and copy character array into it to test against the search string
                // Testing key field
                memset(buffer, 0, sizeof(buffer));
                memcpy(buffer, reg_temp+OFFREG, KEYSIZE);
                found = (!strncmp(buffer, searchstr, strlen(searchstr))) ? 1 : 0;
                OFFREG += KEYSIZE;
                // Testing title field
                memset(buffer, 0, sizeof(buffer));
                memcpy(buffer, reg_temp+OFFREG, TITLESIZE);
                found = (found == 1) ? 1 : teststr(buffer,searchstr," ");
                OFFREG += TITLESIZE;
                // Testing author field
                memset(buffer, 0, sizeof(buffer));
                memcpy(buffer, reg_temp+OFFREG, AUTHORSIZE);
                found = (found == 1) ? 1 : teststr(buffer,searchstr," ");
                OFFREG += AUTHORSIZE;
                // Testing link field
                memset(buffer, 0, sizeof(buffer));
                memcpy(buffer, reg_temp+OFFREG, LINKSIZE);
                found = (found == 1) ? 1 : teststr(buffer,searchstr,".");
              }
              // If not found, move to next register
              OFFBLK += REGSIZE;
              if(found){
                // If search match is successful, copy register to output
                int OFFREG = 0;
                memcpy(regout->key, reg_temp+OFFREG, KEYSIZE);
                OFFREG += KEYSIZE;
                memcpy(regout->title, reg_temp+OFFREG, TITLESIZE);
                OFFREG += TITLESIZE;
                memcpy(regout->author, reg_temp+OFFREG, AUTHORSIZE);
                OFFREG += AUTHORSIZE;
                memcpy(regout->link, reg_temp+OFFREG, LINKSIZE);

                fclose(fp);
                return 1;
              }
            }
            // If still not found, read next block
          }
          else{
              printf("Block consistence bit unconfirmed, file might have been corrupted.\n");
          }
        }
        fclose(fp);
        return 0;
    }
}

int teststr(char* source, char* searchstr, const char* delim){
  char* token; // Variable to store string tokens that will be used for testing against search string
  token = strtok(source, delim);
  while(token != NULL){
    if(!strncmp(token, searchstr, strlen(searchstr))) return 1;
    token = strtok(NULL, delim);
  }
    return 0;
}

int insertreg(reg regin, char* filepath){
    FILE* fp = fopen(filepath, "r+b");
    size_t iterator; // Iterator to check read line's size
    char block_temp[BLKSIZE];
    int BLKCOUNT = 0; // Offset that will be used to find the block's position
    int offset = 0;

    if(!fp)
      return 0;
    else{
      // Checks if there are available slots in the existing blocks
      while((iterator = fread(block_temp, BLKSIZE, 1, fp)) != 0){
        if(!strncmp(block_temp, "#!", 2)){
            // Starting block offset to reach the register index
            offset = 2;

            // Checks if there are available slots in the block's index
            while(strncmp(block_temp+offset, "\x7F", 1) && strncmp(block_temp+offset, "\0", 1) && offset <= (BLKSIZE - REGSIZE))
              offset += REGSIZE;

            if(offset <= (BLKSIZE-REGSIZE)){
              strncpy(block_temp+offset, regin.key, KEYSIZE);
              offset += KEYSIZE;
              strncpy(block_temp+offset, regin.title, TITLESIZE);
              offset += TITLESIZE;
              strncpy(block_temp+offset, regin.author, AUTHORSIZE);
              offset += AUTHORSIZE;
              strncpy(block_temp+offset, regin.link, LINKSIZE);
              offset += LINKSIZE;

              fseek(fp, BLKCOUNT*BLKSIZE, SEEK_SET);
              fwrite(block_temp, BLKSIZE, 1, fp);

              fclose(fp);
              return 1;
            }
            BLKCOUNT++;
        }
        else{
          printf("Block consistence bit unconfirmed, file might have been corrupted.\n");
          return 0;
        }
      }
      // If read size is null, last block is full and a new one must be made
      if(!iterator){
        block* new_blk = newblock();
        // Since it's a new block, copy the input register to the first position
        strncpy(new_blk->reg_index[0].key, regin.key, KEYSIZE);
        strncpy(new_blk->reg_index[0].title, regin.title, TITLESIZE);
        strncpy(new_blk->reg_index[0].author, regin.author, AUTHORSIZE);
        strncpy(new_blk->reg_index[0].link, regin.link, LINKSIZE);
        // Position pointer and write data to file
        fseek(fp, BLKCOUNT*BLKSIZE, SEEK_SET);
        fwrite(new_blk, BLKSIZE, 1, fp);

        free(new_blk);
        fclose(fp);
        return 1;
      }
    }
	  return 0;
}

int listreg(char* filepath){
    FILE* fp = fopen(filepath, "rb");
    char block_temp[BLKSIZE];
    reg regout[BLKSIZE/REGSIZE];
    memset(regout, 0, sizeof(regout));
    int empty = 1;

    if(!fp)
      return 0;
    else{
      while((fread(block_temp, BLKSIZE, 1, fp)) != 0){
        if(!strncmp(block_temp, "#!", 2)){
          int i;
          int offset = 2;
          for(i = 0; i < BLKSIZE/REGSIZE; i++){
            if(strncmp(block_temp+offset, "\x7F", 1) && strncmp(block_temp+offset, "\0", 1)){
              empty = 0;
              strncpy(regout[i].key, block_temp+offset, KEYSIZE);
              offset += KEYSIZE;
              strncpy(regout[i].title, block_temp+offset, TITLESIZE);
              offset += TITLESIZE;
              strncpy(regout[i].author, block_temp+offset, AUTHORSIZE);
              offset += AUTHORSIZE;
              strncpy(regout[i].link, block_temp+offset, LINKSIZE);
              offset += LINKSIZE;
              writereg(regout[i]);
            }
          }
        }
        else{
          printf("Block consistence bit unconfirmed, file might have been corrupted.\n");
          return 0;
        }
      }
      if(empty)
        printf("File is empty.\n");
      fclose(fp);
      return 1;
    }
}

int removereg(char* searchstr, char* filepath){
  FILE* fp = fopen(filepath, "r+b");
  char block_temp[BLKSIZE]; // Buffer to store raw blocks
  char reg_temp[REGSIZE]; // Buffer to store raw registers
  char buffer[128]; // Buffer to format character arrays into strings
  int BLKCOUNT = 0;

  if(!fp)
      return 0;
  else{
      while(fread(block_temp, BLKSIZE, 1, fp)){
      // Checks if search term matches any register in the block and if it is valid (not removed)
        if (!strncmp(block_temp, "#!", 2)){
          int OFFBLK = 2; // Offset variable to jump between registers
          int found = 0;
          while (OFFBLK < BLKSIZE && !found){
            if (strncmp(block_temp+OFFBLK,"\x7F",1) && strncmp(block_temp+OFFBLK,"\0", 1)){
              int OFFREG = 0; // Offset variable to jump between register fields
              // Copy REGSIZE number of bits from temporary block buffer to temporary register buffer
              memcpy(reg_temp, block_temp+OFFBLK, REGSIZE);
              // Clear comparing buffer and copy character array into it to test against the search string
              // Testing key field
              memset(buffer, 0, sizeof(buffer));
              memcpy(buffer, reg_temp+OFFREG, KEYSIZE);
              found = (!strncmp(buffer, searchstr, strlen(searchstr))) ? 1 : 0;
              OFFREG += KEYSIZE;
              // Testing title field
              memset(buffer, 0, sizeof(buffer));
              memcpy(buffer, reg_temp+OFFREG, TITLESIZE);
              found = (found == 1) ? 1 : teststr(buffer,searchstr," ");
              OFFREG += TITLESIZE;
              // Testing author field
              memset(buffer, 0, sizeof(buffer));
              memcpy(buffer, reg_temp+OFFREG, AUTHORSIZE);
              found = (found == 1) ? 1 : teststr(buffer,searchstr," ");
              OFFREG += AUTHORSIZE;
              // Testing link field
              memset(buffer, 0, sizeof(buffer));
              memcpy(buffer, reg_temp+OFFREG, LINKSIZE);
              found = (found == 1) ? 1 : teststr(buffer,searchstr,".");
            }
            if(found){
              /* If search match is successful, change register key to \x7F (Mark as invalid)
                and write to block */
              memset(buffer, 0, sizeof(buffer));
              memcpy(buffer, reg_temp, KEYSIZE);
              memcpy(reg_temp, "\x7F\0\0\0", KEYSIZE);

              fseek(fp, BLKCOUNT*BLKSIZE + OFFBLK, SEEK_SET);
              fwrite(reg_temp, REGSIZE, 1, fp);

              fclose(fp);
              return 1;
            }
            // If not found, move to next register
            OFFBLK += REGSIZE;
          }
        // If still not found, read next block
        BLKCOUNT++;
        }
        else{
          printf("Block consistence bit unconfirmed, file might have been corrupted.\n");
        }
      }
      fclose(fp);
      return 0;
    }
}


void readreg(reg* regout){
    char title_ini[3], author_ini[2], buffer[64];
    unsigned int i, letcount;

    printf("Enter the name of the book's Title:\n");
    fgets(buffer, sizeof(buffer), stdin);
    strncpy(regout->title, buffer, TITLESIZE);
    letcount = 0;
    for(i = 0; i < strlen(buffer); i++){
      if ((isupper(buffer[i])) && (letcount < sizeof(title_ini))){
        strncpy(title_ini+letcount, buffer+i, sizeof(char));
        letcount++;
      }
    }
    if (letcount < sizeof(title_ini))
        strncpy(title_ini+letcount, buffer+letcount, sizeof(title_ini) - letcount);

    printf("Enter the name of the book's Author:\n");
    fgets(buffer, sizeof(buffer), stdin);
    strncpy(regout->author, buffer, AUTHORSIZE);
    letcount = 0;
    for(i = 0; i < strlen(buffer); i++){
      if ((isupper(buffer[i])) && (letcount < sizeof(author_ini))){
        strncpy(author_ini+letcount, buffer+i, sizeof(char));
        letcount++;
      }
    }
    if (letcount < sizeof(author_ini))
        strncpy(author_ini+letcount, buffer+letcount, sizeof(author_ini) - letcount);

    printf("Enter the book's Download Link:\n");
    fgets(buffer, sizeof(buffer), stdin);
    while(strlen(buffer) > 30){
      printf("Warning: link size over 30 characters, resulting link will be truncated.\n");
      printf("Please shorten the link and retry:\n");
      fgets(buffer, sizeof(buffer), stdin);
    }
    strncpy(regout->link, buffer, LINKSIZE);

    /* Register key is a composition of 2 letters from the author's name and
     * 3 letters from the title's name (preferably the initials).
     * If there is a whitespace in the key, it is swapped to '-'.
     */
    strncpy(regout->key, author_ini, 2);
    strncpy(regout->key+2, title_ini, 3);
    for(i = 0; i < KEYSIZE; i++){
      if(isspace(regout->key[i]))
        regout->key[i] = '-';
    }
    memset(buffer,0,sizeof(buffer));
    strncpy(buffer, regout->key, KEYSIZE);
    printf("The chosen register key is: %s\n", buffer);
}

void writereg(reg regout){
  // Writes the contents of a register as formatted strings to stdout
  // Format: [ K:<key> | T:<title> | A:<author> | L: <link> ]
  char buffer[64];
  unsigned int i;
  memset(buffer, 0, sizeof(buffer));
  strncpy(buffer, regout.key, KEYSIZE);
  printf("[ K: ");
  for (i = 0; i < strlen(buffer); i++) putchar(buffer[i]);
  memset(buffer, 0, sizeof(buffer));
  strncpy(buffer, regout.title, TITLESIZE);
  printf(" | T: ");
  for (i = 0; i < strlen(buffer)-1; i++) putchar(buffer[i]);
  memset(buffer, 0, sizeof(buffer));
  strncpy(buffer, regout.author, AUTHORSIZE);
  printf(" | A: ");
  for (i = 0; i < strlen(buffer)-1; i++) putchar(buffer[i]);
  memset(buffer, 0, sizeof(buffer));
  strncpy(buffer, regout.link, LINKSIZE);
  printf(" | T: ");
  for (i = 0; i < strlen(buffer)-1; i++) putchar(buffer[i]);
  printf(" ]\n");
}

block* newblock(){
    block* tmp = (block*)malloc(sizeof(block));
    memset(tmp,0,BLKSIZE);
    strncpy(tmp->header, "#!", 2);
    return tmp;
}

int createfile(char* filepath){
    FILE* fp = fopen(filepath, "wb");
    if(!fp)
        return 0;
    else{
        block* blk = newblock();
        fwrite(blk,BLKSIZE,1,fp);
        free(blk);
        fclose(fp);
    }
    return 1;
}

int compactfile(char* filepath){
    return 1;
}
