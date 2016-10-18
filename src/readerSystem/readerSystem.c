//
// Created by jazzzy on 10/18/16.
//

#include "readerSystem.h"
#include <stdlib.h>
#include <sys/stat.h>

//TODO: Check if it is the end of the file and put the EOF.
unsigned int readNextBlock(readerSystem *rs, blockOfData *block) {
    block->pointer = (char *) malloc(sizeof(char) * ((*rs).blockSize + 1)); //Save memory for the block and the EOF

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

    struct stat st;                                     //We read the size of the blocks in the filesystem of the file
    stat(filename, &st);
    (*rs)->blockSize = (unsigned int) st.st_blksize;    //TODO: Try smaller blocks

    if (readNextBlock(*rs, &((*rs)->block0)) >= (*rs)->blockSize) { //If the first block is full
        //readNextBlock(*rs, &((*rs)->block1));                       //Then we fill the next one
    }

    (*rs)->beg.pointer = (*rs)->block0.pointer;     //Both pointers start at the beginning of the first block.
    (*rs)->end.pointer = (*rs)->block0.pointer;

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

    char retChar = *(rs->end.pointer);

    if (retChar == EOF) {       //We have found an EOF, could be in the end of the block or the file
        if ((rs->end.pointer == (rs->block0.pointer + rs->blockSize * sizeof(char))) ||
            (rs->end.pointer == (rs->block1.pointer + rs->blockSize * sizeof(char)))) {
            //We are on one of our inserted EOF
            if(rs->end.block==1){   //If we are on the block 1
                readNextBlock(rs, &(rs->block0));
                rs->end.block=0;
            }else if(rs->end.block==0){
                readNextBlock(rs, &(rs->block0));
                rs->end.block=0;
            }

        }else{ //We have found the true EOF
            return EOF;
        }

    }

    (rs->end.pointer += sizeof(char)); //RSA 9
    return retChar;

}

void returnChar(readerSystem *rs) {
    (rs->end.pointer -= sizeof(char));
}

char *getCurrentLex(readerSystem *rs) {


}