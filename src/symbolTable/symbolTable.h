//
// Created by jazzzy on 10/16/16.
//

#include "../utils/bHashTable.h"

#ifndef BDC_SYMBOLTABLE_H
#define BDC_SYMBOLTABLE_H

typedef struct {
    hashTable reserved;
    hashTable identifiers;
}symbolTable;

typedef struct {            //More data could be added here, this will be the void *data inserted into the hast tables
    int lexicalComponent;
}symbolData;


void initSymbolTable(symbolTable *oSymbolTable);

void deleteSymbolTable(symbolTable *oSymbolTable);

//TODO: Insert here the functions of the symbol table


#endif //BDC_SYMBOLTABLE_H
