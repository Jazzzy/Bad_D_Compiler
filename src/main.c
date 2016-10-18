#include "symbolTable/symbolTable.h"
#include "readerSystem/readerSystem.h"
#include <stdlib.h>
#include <string.h>

int main() {


    readerSystem *rs;

    char *filename = "/home/jazzzy/GitProjects/Bad_D_Compiler/src/main.c";

    initReaderSystem(&rs, filename);

    int i = 0;

    char c;
    (c) = getNextChar(rs);
    while ((c) != EOF) {
        printf("%c", c);          //This prints every character
        c = getNextChar(rs);
        /*if ((i++) % 2 == 0) {
            returnChar(rs);         //Just to check that returnChar works everywhere, we print everything twice.
        }
        */

        //This checks that we can get lexems

        if (c == EOF || (i % 10 == 0 && i != 0)) {
            char *lex = getCurrentLex(rs);
            printf("\nCurrent lexem: [%s], size: [%lu]\n", lex, strlen(lex));
            free(lex);
        }


        i++;
    }


    deleteReaderSystem(&rs);

/*
    symbolTable *mSymbolTable;
    initSymbolTable(&mSymbolTable);

    char *key = "HELLO!";
    symbolData *symbolData1 = (symbolData *) malloc(sizeof(symbolData));
    symbolData1->lexicalComponent = 1;

    addLex(&mSymbolTable, key, symbolData1);
    printf("Lexical component retrieved for %s: %d\n", key, searchLex(mSymbolTable, key)->lexicalComponent);

    printSymbolTable(mSymbolTable);

    deleteSymbolTable(&mSymbolTable);
*/

    /*//---------------------Testing hashTable-------------------------
    hashTable mHashTable = NULL;
    initHashTable(&mHashTable);
    char *key = "HELLO!";
    char *aux = "FIRST";
    char *aux2 = "SECOND";
    addElement(&mHashTable, key, aux);
    addElement(&mHashTable, key, aux2);


    char *key2 = "HELLO2";
    addElement(&mHashTable, key2, aux);

    printf("Element has this data: %s\n",(char*)getElement(&mHashTable,key2)->data);

    addElement(&mHashTable, key2, aux2);

    printf("Element has this data: %s\n",(char*)getElement(&mHashTable,key2)->data);

    printStatus(mHashTable);
    printData(mHashTable);
    deleteHastTable(&mHashTable);
    //---------------------------------------------------------------*/






    return 0;
}