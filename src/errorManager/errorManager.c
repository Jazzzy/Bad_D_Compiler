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

/**
 * We print the apropriate message for each error code.
 *
 * */
void printMessageFor(int code) {
    switch (code) {
        case ERR_UNEXPECTED_EOF:
            printf("Unexpected End of File");
            break;
        case ERR_BAD_OPERATOR:
            printf("Malformed Operator");
            break;
        case ERR_FILE_ERROR:
            printf("Error managing external file");
            break;
        case ERR_BAD_BINARY:
            printf("Malformed binary number");
            break;

        default:
            printf("No message for that code.");
            break;
    };
}

/**
 * We manage a fatal error showing the line and position given to us.
 *
 * It deletes and frees all memory currently used by the main modules of the program.
 *
 * */
void manageFatalErrorWithLine(int code, char *message, int line, int position) {

    printf("ERROR CODE [%d]: ", code);
    printMessageFor(code);
    printf("\n\t%s\n\thappened at line %d, in position %d\n\n", message, line, position);


    if (global_la != NULL)
        deleteLexicalAnalyzer(&global_la);
    if (global_st != NULL)
        deleteSymbolTable(&global_st);
    if (global_rs != NULL)
        deleteReaderSystem(&global_rs);


    exit(EXIT_FAILURE);
}


/**
 * We manage a fatal error without showing the line and position given to us.
 *
 * It deletes and frees all memory currently used by the main modules of the program.
 *
 * */
void manageFatalError(int code, char *message) {

    printf("ERROR CODE [%d]: ", code);
    printMessageFor(code);
    printf("\n\t%s\n\n", message);


    if (global_la != NULL)
        deleteLexicalAnalyzer(&global_la);
    if (global_st != NULL)
        deleteSymbolTable(&global_st);
    if (global_rs != NULL)
        deleteReaderSystem(&global_rs);


    exit(EXIT_FAILURE);
}


/**
 * We manage a non fatal error showing the line and position given to us.
 *
 * This allows the program to go on.
 * */
void manageNonFatalErrorWithLine(int code, char *message, int line, int position) {

    printf("ERROR CODE [%d]: ", code);
    printMessageFor(code);
    printf("\n\t%s\n\thappened at line %d, in position %d\n\n", message, line, position);

    return;
}


/**
 * We manage a non fatal error without showing the line and position given to us.
 *
 * This allows the program to go on.
 * */
void manageNonFatalError(int code, char *message) {

    printf("ERROR CODE [%d]: ", code);
    printMessageFor(code);
    printf("\n\t%s\n\n", message);

    return;

}
