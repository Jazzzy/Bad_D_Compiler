#ifndef BDC_READERSYSTEM_H
#define BDC_READERSYSTEM_H

#include <stdio.h>

typedef struct {
    char *pointer;
    unsigned short block;
} sentinelPointer;

typedef struct {
    char *pointer;
} blockOfData;

typedef struct {
    unsigned int blockSize;
    FILE *file;
    sentinelPointer beg;
    sentinelPointer end;
    blockOfData block0;
    blockOfData block1;
    unsigned int currentLine;
    unsigned int currentPosition;
    unsigned int lengthOfCurrentLex;
} readerSystem;


void initReaderSystem(readerSystem **rs, char *filename);

void deleteReaderSystem(readerSystem **rs);

char getNextChar(readerSystem *rs);

void returnChar(readerSystem *rs);

char *getCurrentLex(readerSystem *rs);

void advanceBeginning(readerSystem *rs);

#endif //BDC_READERSYSTEM_H
