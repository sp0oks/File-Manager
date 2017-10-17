#include "register.h"

/*
 * Invalid register will have a starting byte 7F (ASCII Del), the relative address of the next invalid register (in register units, not bytes);
 */

int searchreg(reg* regout, char* searchstr, char* filepath){
    FILE* fp = fopen(filepath, "rb");
    char blk_temp[BLKSIZE];

    if(!fp)
        return 0;

    else{
        while(fread(blk_temp, BLKSIZE, 1, fp)){
          // Checks if keys match and if the register is valid
         /* if(!strncmp(reg_temp, key, 4) && strncmp(reg_temp, "\x7F", 1)){
              strncpy(regout->key, reg_temp, 4);
              strncpy(regout->title, reg_temp+4, 30);
              strncpy(regout->author, reg_temp+34, 21);
              strncpy(regout->link, reg_temp+55, 30);

              fclose(fp);
              return 1;
          }*/
        }
        fclose(fp);
    }
    return 0;
}

int insertreg(reg regin, char* filepath){
    FILE* fp = fopen(filepath, "r+b");
    size_t iterator;
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
              strncpy(block_temp+offset, regin.key, sizeof(regin.key));
              offset += sizeof(regin.key);
              strncpy(block_temp+offset, regin.title, sizeof(regin.title));
              offset += sizeof(regin.title);
              strncpy(block_temp+offset, regin.author, sizeof(regin.author));
              offset += sizeof(regin.author);
              strncpy(block_temp+offset, regin.link, sizeof(regin.link));
              offset += sizeof(regin.link);

              fseek(fp, BLKCOUNT*BLKSIZE, SEEK_SET);
              fwrite(block_temp, BLKSIZE, 1, fp);

              fclose(fp);
              return 1;
            }
            BLKCOUNT++;
        }
      }

      if(!iterator){
        block* new_blk = newblock();

        strncpy(new_blk->reg_index[0].key, regin.key, sizeof(regin.key));
        strncpy(new_blk->reg_index[0].title, regin.title, sizeof(regin.title));
        strncpy(new_blk->reg_index[0].author, regin.author, sizeof(regin.author));
        strncpy(new_blk->reg_index[0].link, regin.link, sizeof(regin.link));

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
    return 1;
}

int removereg(char* key, char* filepath){
    return 1;
}

void readreg(reg* regout){
    char title_ini[3], author_ini[2], buffer[64];
    unsigned int i, letcount;

    printf("Enter the name of the book's Title:\n");
    fgets(buffer, sizeof(buffer), stdin);
    strncpy(regout->title, buffer, sizeof(regout->title));
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
    strncpy(regout->author, buffer, sizeof(regout->author));
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
    strncpy(regout->link, buffer, sizeof(regout->link));

    /* Register key is a composition of 2 letters from the author's name and
     * 3 letters from the title's name (preferably the initials).
     * If there is a whitespace in the key, it is swapped to '-'.
     */
    strncpy(regout->key, author_ini, 2);
    strncpy(regout->key+2, title_ini, 3);
    for(i = 0; i < sizeof(regout->key); i++){
      if(isspace(regout->key[i]))
        regout->key[i] = '-';
    }
    memset(buffer,0,sizeof(buffer));
    strncpy(buffer, regout->key, sizeof(regout->key));
    printf("The chosen register key is: %s\n", buffer);
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
