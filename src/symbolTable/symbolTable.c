//
// Created by jazzzy on 10/16/16.
//
#include <stdlib.h>
#include "symbolTable.h"


void initSymbolTable(symbolTable *oSymbolTable) {
    oSymbolTable = (symbolTable *) malloc(sizeof(symbolTable));
    initHashTable(&(oSymbolTable->identifiers));
    initHashTable(&(oSymbolTable->reserved));
}

void deleteSymbolTable(symbolTable *oSymbolTable) {
    deleteHastTable(&(oSymbolTable->identifiers));
    deleteHastTable(&(oSymbolTable->reserved));
    free(oSymbolTable);
}