//
// Created by jazzzy on 10/16/16.
//
#include <stdlib.h>
#include <stdio.h>
#include "symbolTable.h"
#include "../utils/defineParser.h"


void initSymbolTable(symbolTable **oSymbolTable, char *pathToDefineFile) {
    *oSymbolTable = (symbolTable *) malloc(
            sizeof(symbolTable));     //We reserve memory for the struct of the symbol table
    initHashTable(&((*oSymbolTable)->identifiers));                 //And for both hash tables we use
    initHashTable(&((*oSymbolTable)->reserved));


    parseReservedWords(oSymbolTable, pathToDefineFile);

}

void deleteSymbolTable(symbolTable **oSymbolTable) {
    deleteHastTable(&((*oSymbolTable)->identifiers));               //First we delete the hash tables
    deleteHastTable(&((*oSymbolTable)->reserved));
    free(*oSymbolTable);                                            //And then the struct of the symbol table itself
}

symbolData *searchLex(symbolTable *oSymbolTable, char *lex) {
    hashElement *mElement = getElement((oSymbolTable->reserved), lex);  //We look first in the reserved words table
    if (mElement == NULL) {                                             //If the lexem is not a reserved word
        mElement = getElement((oSymbolTable->identifiers), lex);        //Then we look in the identifiers table

    }
    if (mElement == NULL) return NULL;
    return (symbolData *) mElement->data;                               //And return the result which can be NULL
}

int addLex(symbolTable **oSymbolTable, char *lex, symbolData *data) {                   //This adds a lexem to the table of identifiers
    return (addElement(((*oSymbolTable)->identifiers), lex, (void *) data) != NULL);    //We add the lexem to the identifiers table
}

int addReservedWork(symbolTable **oSymbolTable, char *lex, symbolData *data) {      //This adds a word to the table of reserved words
    return (addElement(((*oSymbolTable)->reserved), lex, (void *) data) != NULL);   //We add the word to the reserved words table
}

void printSymbolTable(symbolTable *oSymbolTable) {
    printf("\n\n----------------PRINTING STATUS AND DATA OF THE RESERVED WORDS TABLE----------------\n\n");
    printState(*oSymbolTable->reserved);
    printData(*oSymbolTable->reserved);

    printf("\n\n----------------PRINTING STATUS AND DATA OF THE IDENTIFIERS TABLE----------------\n\n");
    printState(*oSymbolTable->identifiers);
    printData(*oSymbolTable->identifiers);

}