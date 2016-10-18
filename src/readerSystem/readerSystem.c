//
// Created by jazzzy on 10/18/16.
//

#include "readerSystem.h"
#include <stdlib.h>
#include <sys/stat.h>

//TODO: Check if it is the end of the file and put the \0.
unsigned int readNextBlock(readerSystem *rs, char **pointer) {
    *pointer = (char *) malloc(sizeof(char) * ((*rs).blockSize + 1));
    *pointer[(*rs).blockSize] = '\0';                           //Bruno said that we need a +1 here.
    unsigned int charRead= (unsigned int) fread(*pointer, sizeof(char), rs->blockSize, rs->file);

    return charRead;
}

void initReaderSystem(readerSystem **rs, char *filename) {
    *rs = (readerSystem *) malloc(sizeof(readerSystem));    //Reserve size for the struct that defines the reader system
    (*rs)->file = fopen(filename, "r");                  //We open the file itself
    if ((*rs)->file == NULL) {
        //THROW ERROR
        return;
    }

    struct stat st;                                     //We read the size of the blocks in the filesystem of the file
    stat(filename, &st);
    (*rs)->blockSize = (unsigned int) st.st_blksize;    //TODO: Try smaller blocks

    if (readNextBlock(*rs, &((*rs)->block0)) >= (*rs)->blockSize) {
        readNextBlock(*rs, &((*rs)->block1));
    }

    (*rs)->beg.pointer = (*rs)->block0;
    (*rs)->end.pointer = (*rs)->block0;

    (*rs)->beg.block = 0;
    (*rs)->end.block = 0;

    return;
}


void deleteReaderSystem(readerSystem **rs) {

    return;
}

char getNextChar(readerSystem *rs) {


}

void returnChar(readerSystem *rs) {


}

char *getCurrentLex(readerSystem *rs) {


}