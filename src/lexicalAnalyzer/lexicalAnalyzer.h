//
// Created by jazzzy on 10/21/16.
//

#ifndef BDC_LEXICALANALYZER_H
#define BDC_LEXICALANALYZER_H


#include "../readerSystem/readerSystem.h"
#include "../symbolTable/symbolTable.h"

typedef struct {
    char str[4];
    int length;
    unsigned int lexicalComponent;
    short isPossible;
} operator;

typedef struct {
    operator *list;
    int length;
} listOfOperators;

typedef struct {
    readerSystem *mReaderSystem;
    symbolTable *mSymbolTable;
    listOfOperators *mListOfOperators;
} lexicalAnalyzer;


void initLexicalAnalyzer(lexicalAnalyzer **la, readerSystem *rs, symbolTable *st, char *pathToDefine, char *pathToOperators);

int getNextLexicalComponent(lexicalAnalyzer *la);

void deleteLexicalAnalyzer(lexicalAnalyzer **la);


#endif //BDC_LEXICALANALYZER_H
