#include "lexicalAnalyzer.h"
#include <stdlib.h>
#include "../DLang/D_DEFINE_NON_RESERVED_WORDS.h"
#include "lexicalHelper.h"
#include "../errorManager/errorManager.h"
#include <string.h>
#include "./flex/lex.yy.c"
#include "../readerSystem/readerSystem.h"

extern lexicalAnalyzer *global_la;


/**
 * Initializes the lexical analyzer and the operators
 *
 * rs and st need to be init already.
 *
 * */
void initLexicalAnalyzer(lexicalAnalyzer **la, readerSystem *rs, symbolTable *st) {
    *la = (lexicalAnalyzer *) malloc(sizeof(lexicalAnalyzer));
    global_la = *la;                                            //We update the pointer to the lexical analyzer in case we need it in error management.
    (*la)->mReaderSystem = rs;
    (*la)->mSymbolTable = st;
    yyin = rs->file;
}

/**
 * Deletes the whole lexical analyzer
 *
 * */
void deleteLexicalAnalyzer(lexicalAnalyzer **la) {
    free(*la);
}


//Define this in order to send the lexeme to the syntactical analyzer
#define SEND_DEBUG_DATA


/**
 * Returns the next lexical component found by the lexical analyzer.
 *
 * */
lexemeComponentPackage getNextLexicalComponent(lexicalAnalyzer *la) {

    lexemeComponentPackage lcp;
    lcp.lexeme = NULL;


    int component = yylex();
    if (component == DOCUMENTATION_COMMENT || component == STRING_LITERAL) {
        lcp.lexeme = (char *) malloc((strlen(array_buf) + 1) * sizeof(char));
        strcpy(lcp.lexeme, array_buf);
        lcp.lexicalComponent = component;
        fillComponentStr(lcp.strLexicalComponent,component);

    } else {
        lcp.lexeme = (char *) malloc((strlen(yytext) + 1) * sizeof(char));
        strcpy(lcp.lexeme, yytext);
        lcp.lexicalComponent = component;
        fillComponentStr(lcp.strLexicalComponent,component);
    }

    return lcp;

}

/*
void main(int argc, char **argv) {

    ++argv, --argc;
    if (argc > 0)
        yyin = fopen(argv[0], "r");
    else
        yyin = stdin;

    int component;

    while ((
                   component = yylex()
           ) != END_OF_FILE) {
        if (component == DOCUMENTATION_COMMENT || component == STRING_LITERAL) {
            printf("Component %4d:\t%s\n", component, array_buf);
        } else {
            printf("Component %4d:\t%s\n", component, yytext);
        }
    }

    printf("Lineas: %d, Caracteres: %d\n", numLine, numCharacter);
}

*/