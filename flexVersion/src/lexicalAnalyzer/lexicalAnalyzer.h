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
    readerSystem *mReaderSystem;
    symbolTable *mSymbolTable;
} lexicalAnalyzer;


void initLexicalAnalyzer(lexicalAnalyzer **la, readerSystem *rs, symbolTable *st);

lexemeComponentPackage getNextLexicalComponent(lexicalAnalyzer *la);

void deleteLexicalAnalyzer(lexicalAnalyzer **la);


#endif //BDC_LEXICALANALYZER_H
