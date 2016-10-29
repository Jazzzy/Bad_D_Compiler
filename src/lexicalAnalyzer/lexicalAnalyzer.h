#ifndef BDC_LEXICALANALYZER_H
#define BDC_LEXICALANALYZER_H


#include "../readerSystem/readerSystem.h"
#include "../symbolTable/symbolTable.h"


typedef struct {
    int lexicalComponent;
    char *lexeme;
    char strLexicalComponent[50];   //Added this for visibility while debugging
} lexemeComponentPackage;


typedef struct {
    char str[4];
    int length;
    unsigned int lexicalComponent;
    short isPossible;
} operator;

typedef struct {
    operator *list;
    int length;
    int startPosition;
    int numPossible;
} listOfOperators;

typedef struct {
    readerSystem *mReaderSystem;
    symbolTable *mSymbolTable;
    listOfOperators *mListOfOperators;
} lexicalAnalyzer;


void initLexicalAnalyzer(lexicalAnalyzer **la, readerSystem *rs, symbolTable *st, char *pathToOperators);

lexemeComponentPackage getNextLexicalComponent(lexicalAnalyzer *la);

void deleteLexicalAnalyzer(lexicalAnalyzer **la);


#endif //BDC_LEXICALANALYZER_H
