//
// Created by jazzzy on 10/18/16.
//

#include "readerSystem.h"
#include <stdlib.h>
#include <sys/stat.h>

//TODO: Check if it is the end of the file and put the EOF.
unsigned int readNextBlock(readerSystem *rs, blockOfData *block) {

    //We read a block of data from the file
    unsigned int charsRead = (unsigned int) fread(block->pointer, sizeof(char), rs->blockSize, rs->file);

    block->pointer[charsRead] = EOF;            //We put EOF at the end of the string read. (End of the block normally)

/*    if (charsRead < (*rs).blockSize) {          //If we are in the final block
        block->final = 1;                       //We store the block as final
    }else{
        block->final = 0;                       //Otherwise is not final
    }*/

    return charsRead;
}

void initReaderSystem(readerSystem **rs, char *filename) {
    *rs = (readerSystem *) malloc(sizeof(readerSystem));//Reserve size for the struct that defines the reader system
    (*rs)->file = fopen(filename, "r");                 //We open the file itself
    if ((*rs)->file == NULL) {                          //We check if we have the file opened.
        //THROW ERROR
        return;
    }

#ifdef UNIX
    struct stat st;                                     //We read the size of the blocks in the filesystem of the file
    stat(filename, &st);
    //(*rs)->blockSize = (unsigned int) st.st_blksize;
#else
    (*rs)->blockSize = (unsigned int) 16;
#endif


    (*rs)->block0.pointer = (char *) malloc(sizeof(char) * ((*rs)->blockSize + 1)); //Save memory for the blocks and the EOFs
    (*rs)->block1.pointer = (char *) malloc(sizeof(char) * ((*rs)->blockSize + 1));

    if (readNextBlock(*rs, &((*rs)->block0)) >= (*rs)->blockSize) { //If the first block is full
        //readNextBlock(*rs, &((*rs)->block1));                       //Then we could fill the next one
    }

    (*rs)->beg.pointer = ((*rs)->block0.pointer);
    (*rs)->end.pointer = ((*rs)->block0.pointer) - sizeof(char);    //End pointer starts before the beginning of the first block.

    (*rs)->beg.block = 0;                           //And both are in the first block
    (*rs)->end.block = 0;

    return;
}


void deleteBlock(blockOfData *block) {

    if (block == NULL) {
        return;
    }

    free((block)->pointer);

}

void deleteReaderSystem(readerSystem **rs) {

    deleteBlock(&((*rs)->block0));
    deleteBlock(&((*rs)->block1));
    if (fclose((*rs)->file) != 0) {
        //THROW ERROR
        return;
    }
    free(*rs);

    return;
}

char getNextChar(readerSystem *rs) {

    rs->end.pointer += sizeof(char);    //We advance a position in the end pointer

    char retChar = *(rs->end.pointer);  //We get the character to return

    if (retChar == EOF) {               //We have found an EOF, could be in the end of the block or the file
        if ((rs->end.pointer == (rs->block0.pointer + rs->blockSize * sizeof(char))) ||
            (rs->end.pointer == (rs->block1.pointer + rs->blockSize * sizeof(char)))) {     //We are on the end of the block
            //We are on one of our inserted EOF
            if (rs->end.block == 1) {               //If we are on the block 1
                readNextBlock(rs, &(rs->block0));   //We read the next block on block 0
                rs->end.block = 0;                  //We mark the block of the end pointer to be 0
                rs->end.pointer = rs->block0.pointer;   //We set the pointer to the beginning of the block
                retChar = *(rs->end.pointer);           //We read the character at the beginning of the block
            } else if (rs->end.block == 0) {        //If we are on block 0
                readNextBlock(rs, &(rs->block1));   //Same as above but with block 1
                rs->end.block = 1;
                rs->end.pointer = rs->block1.pointer;
                retChar = *(rs->end.pointer);
            }

        } else { //We have found the true EOF
            return EOF;
        }

    }

    return retChar;

}

/*
 * This function can trigger errors if called twice or more without any call to
 * getNextChar because we could be accessing to unknown positions in memory.
 * */
void returnChar(readerSystem *rs) {
    (rs->end.pointer -= sizeof(char));  //To go to the character before we can just substract one position in the array

    /*
     * This makes sense because if we are in the middle of a block then there is no problem.
     *
     * If we are at the first character in a block then we would point to an unknown position but since we advance before
     * reading the position we should have no problem with that either.
     *
     * */
}

char *getCurrentLex(readerSystem *rs) {

    char *lex = NULL;
    unsigned long size;
    int i;

    if (rs->beg.block == rs->end.block) {
        size = (rs->end.pointer - rs->beg.pointer) / sizeof(char);

        lex = (char *) malloc((size + 1) * sizeof(char));
        for (i = 0; i < size; ++i) {
            lex[i] = rs->beg.pointer[i];
        }
        lex[size] = '\0';
    } else {
        if (rs->beg.block == 0) {
            unsigned long size1 = (rs->block0.pointer + rs->blockSize - rs->beg.pointer) / sizeof(char);
            long size2 = (rs->end.pointer - rs->block1.pointer) / sizeof(char);
            if (size2 < 0) size2 = 0;
            size = size1 + size2;
            lex = (char *) malloc((size + 1) * sizeof(char));
            int j = 0;
            for (i = 0; i < size1; ++i) {
                lex[j] = rs->beg.pointer[i];
                j++;
            }
            for (i = 0; i < size2; ++i) {
                lex[j] = rs->block1.pointer[i];
                j++;
            }
            lex[size] = '\0';
        } else if (rs->beg.block == 1) {
            unsigned long size1 = (rs->block1.pointer + rs->blockSize - rs->beg.pointer) / sizeof(char);
            long size2 = (rs->end.pointer - rs->block0.pointer) / sizeof(char);

            size = size1 + size2;
            lex = (char *) malloc((size + 1) * sizeof(char));
            int j = 0;
            for (i = 0; i < size1; ++i) {
                lex[j] = rs->beg.pointer[i];
                j++;
            }
            for (i = 0; i < size2; ++i) {
                lex[j] = rs->block0.pointer[i];
                j++;
            }
            lex[size] = '\0';
        }
    }
    rs->beg.block = rs->end.block;
    rs->beg.pointer = rs->end.pointer;

    return lex;
}