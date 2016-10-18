//
// Created by jazzzy on 10/18/16.
//

#ifndef BDC_READERSYSTEM_H
#define BDC_READERSYSTEM_H

#include <stdio.h>

typedef struct {
    char *pointer;
    unsigned short block;
}sentinelPointer;

typedef struct{
    char* pointer;
    //unsigned short final;
}blockOfData;

typedef struct {
    unsigned int blockSize;
    FILE *file;
    sentinelPointer beg;
    sentinelPointer end;
    blockOfData block0;
    blockOfData block1;
}readerSystem;


void initReaderSystem(readerSystem **rs, char* filename);

void deleteReaderSystem(readerSystem **rs);

char getNextChar(readerSystem *rs);

void returnChar(readerSystem *rs);

char *getCurrentLex(readerSystem *rs);


#endif //BDC_READERSYSTEM_H
