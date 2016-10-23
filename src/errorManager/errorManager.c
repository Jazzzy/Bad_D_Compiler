//
// Created by jazzzy on 10/23/16.
//

#include <stdlib.h>
#include <stdio.h>
#include "errorManager.h"
#include "../lexicalAnalyzer/lexicalAnalyzer.h"

extern lexicalAnalyzer *global_la;
extern symbolTable *global_st;
extern readerSystem *global_rs;

void printMessageFor(int code) {
    switch (code) {
        case ERR_UNEXPECTED_EOF:
            printf("Unexpected End of File");
            break;
        case ERR_BAD_OPERATOR:
            printf("Malformed Operator");
            break;

        default:
            printf("No message for that code.");
            break;
    };
}

void manageFatalErrorWithLine(int code, char *message, int line) {

    printf("ERROR CODE [%d]: ", code);
    printMessageFor(code);
    printf("\n\t%s\n\thappened at line %d\n\n", message, line);


    deleteLexicalAnalyzer(&global_la);
    deleteSymbolTable(&global_st);
    deleteReaderSystem(&global_rs);


    exit(EXIT_FAILURE);
}

void manageFatalError(int code, char *message) {

    printf("ERROR CODE [%d]: ", code);
    printMessageFor(code);
    printf("\n\t%s\n\n", message);


    deleteLexicalAnalyzer(&global_la);
    deleteSymbolTable(&global_st);
    deleteReaderSystem(&global_rs);


    exit(EXIT_FAILURE);
}

void manageNonFatalErrorWithLine(int code, char *message, int line) {

    printf("ERROR CODE [%d]: ", code);
    printMessageFor(code);
    printf("\n\t%s\n\thappened at line %d\n\n", message, line);

    return;
}

void manageNonFatalError(int code, char *message) {

    printf("ERROR CODE [%d]: ", code);
    printMessageFor(code);
    printf("\n\t%s\n\n", message);

    return;

}
