#include "symbolTable/symbolTable.h"
#include "readerSystem/readerSystem.h"
#include "lexicalAnalyzer/lexicalAnalyzer.h"
#include "DLang/D_DEFINE_NON_RESERVED_WORDS.h"
#include <stdlib.h>


#ifdef TESTING

void testReaderSystem() {

    readerSystem *rs;

    char *filename = "/home/jazzzy/GitProjects/Bad_D_Compiler/files/regression.d";

    initReaderSystem(&rs, filename);

    int i = 0;
    char c;
    do {
        c = getNextChar(rs);        //We get a character
        if (c != EOF)
            printf("%c", c);            //Do the processing, printing in this case

        /*
        if ((i++) % 2 == 0) {
            returnChar(rs);         //We can check that returnChar works everywhere, we print everything twice.
        }
        */

        //This checks that we can get lexems
        //Uncomment this to see the lexems retrieved.
        /*
        if (c == EOF || (i % 10 == 0 && i != 0)) {      //Lets suppose we get a lexem
            returnChar(rs);                             //Then we return the last character
            char *lex = getCurrentLex(rs);              //And get the current lexem
            printf("\nCurrent lexem: [%s], size: [%lu]\n", lex, strlen(lex));
            free(lex);
        }
        */
        i++;
    } while ((c) != EOF);

    deleteReaderSystem(&rs);
    return;

}


void testSymbolTable() {
    symbolTable *mSymbolTable;
    initSymbolTable(&mSymbolTable);
    char c;
    for (c = 'a'; c < 'z'; c++) {
        char *key = (char *) malloc(sizeof(char) * 6);
        int i = 0;
        for (i = 0; i < 5; i++) {
            key[i] = c;
        }
        key[5] = '\0';
        symbolData *symbolData1 = (symbolData *) malloc(sizeof(symbolData));
        symbolData1->lexicalComponent = 1;
        addLex(&mSymbolTable, key, symbolData1);
        printf("Lexical component retrieved for %s: %d\n", key, searchLex(mSymbolTable, key)->lexicalComponent);
    }

    printSymbolTable(mSymbolTable);
    deleteSymbolTable(&mSymbolTable);

    return;
}

void testHashTable() {
    hashTable *mHashTable = NULL;
    initHashTable(&mHashTable);

    char *aux = NULL;


    char *key = (char *) malloc(sizeof(char) * 6);
    int i = 0;
    for (i = 0; i < 6; i++) {
        key[i] = 'a';
    }
    key[5] = '\0';

    char *key2 = (char *) malloc(sizeof(char) * 6);
    for (i = 0; i < 6; i++) {
        key2[i] = 'a';
    }
    key2[5] = '\0';

    char *key3 = (char *) malloc(sizeof(char) * 6);
    for (i = 0; i < 6; i++) {
        key3[i] = 'b';
    }
    key3[5] = '\0';
    char *key4 = (char *) malloc(sizeof(char) * 6);
    for (i = 0; i < 6; i++) {
        key4[i] = 'b';
    }
    key4[5] = '\0';

    addElement(mHashTable, key, aux);
    addElement(mHashTable, key2, aux);
    addElement(mHashTable, key3, aux);

    if (getElement(mHashTable, key2)->data != NULL)
        printf("Element has this data: %s\n", (char *) getElement(mHashTable, key2)->data);

    addElement(mHashTable, key4, aux);

    if (getElement(mHashTable, key2)->data != NULL)
        printf("Element has this data: %s\n", (char *) getElement(mHashTable, key2)->data);

    printState(*mHashTable);
    printData(*mHashTable);

    deleteHastTable(&mHashTable);
    //---------------------------------------------------------------*/

    return;
}

#endif


lexicalAnalyzer *global_la = NULL;
symbolTable *global_st = NULL;
readerSystem *global_rs = NULL;

/*
 * We can compile this with :


gcc main.c utils/bHashTable.c utils/bHashTable.h symbolTable/symbolTable.c symbolTable/symbolTable.h readerSystem/readerSystem.c readerSystem/readerSystem.h lexicalAnalyzer/lexicalAnalyzer.c lexicalAnalyzer/lexicalAnalyzer.h DLang/D_DEFINE_RESERVED_WORDS.h utils/operatorParser.c utils/operatorParser.h utils/defineParser.c utils/defineParser.h lexicalAnalyzer/lexicalHelper.c lexicalAnalyzer/lexicalHelper.h DLang/D_DEFINE_NON_RESERVED_WORDS.h errorManager/errorManager.c errorManager/errorManager.h -o ../bin/bdc

 *
 */


int main(int argc, char **argv) {

    //TODO: Get this paths from the list of arguments

    char *filename = "/home/jazzzy/GitProjects/Bad_D_Compiler/files/regression.d";
    char *pathToDefine = "/home/jazzzy/GitProjects/Bad_D_Compiler/src/DLang/D_DEFINE_RESERVED_WORDS.h";
    char *pathToOperators = "/home/jazzzy/GitProjects/Bad_D_Compiler/src/DLang/d.ope";

    if (argc < 4) {
        printf("Not Enough arguments, We need:\n\tA file to compile\n\tA file with the reserved words\n\tA File that defines the operators\n\n");
        printf("We will provide our own files just for this time :) , but this may not work. :( \n\n");
    } else {
        filename = argv[1];
        pathToDefine = argv[2];
        pathToOperators = argv[3];
    }


    //Init the reader system
    readerSystem *rs = NULL;

    initReaderSystem(&rs, filename);
    global_rs = rs;

    //Init the symbol table
    symbolTable *st = NULL;
    initSymbolTable(&st, pathToDefine);
    global_st = st;

    //Init the lexical analyzer
    lexicalAnalyzer *la = NULL;
    initLexicalAnalyzer(&la, rs, st, pathToDefine, pathToOperators);
    global_la = la;


    int lexComp = getNextLexicalComponent(la);
    while (lexComp != END_OF_FILE) {
        printf("Lexixal component detected: [%d]\n\n", lexComp);
        lexComp = getNextLexicalComponent(la);
    }

    printSymbolTable(st);

    deleteLexicalAnalyzer(&la);
    deleteSymbolTable(&st);
    deleteReaderSystem(&rs);

}
