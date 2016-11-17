#include "symbolTable/symbolTable.h"
#include "readerSystem/readerSystem.h"
#include "lexicalAnalyzer/lexicalAnalyzer.h"
#include "DLang/D_DEFINE_NON_RESERVED_WORDS.h"
#include "utils/colours.h"
#include <stdlib.h>


lexicalAnalyzer *global_la = NULL;
symbolTable *global_st = NULL;
readerSystem *global_rs = NULL;


int main(int argc, char **argv) {

    char *filename = "./arguments/regression.d";
    char *pathToDefine = "./arguments/D_DEFINE_RESERVED_WORDS.h";


    if (argc < 3) {
        printf("Not Enough arguments, We need:\n\tA file to compile\n\tA file with the reserved words\n\n");
        printf("We will provide our own files just for this time :) , but this may not work. :( \n\n");
    } else {
        filename = argv[1];
        pathToDefine = argv[2];
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
    initLexicalAnalyzer(&la, rs, st);
    global_la = la;


    //This section represents what the syntactical analyzer would do
    lexemeComponentPackage lcp = getNextLexicalComponent(la);
    while (lcp.lexicalComponent != END_OF_FILE) {
        if (lcp.lexeme != NULL) {
            printf("[ " BLU "%s" RESET " ]\n\t[" GRN "%d" RESET "]\t=>\t[" CYN "%s" RESET "]\n\n", lcp.strLexicalComponent, lcp.lexicalComponent, lcp.lexeme);
        } else {
            printf("[%d]\n", lcp.lexicalComponent);
        }
        free(lcp.lexeme);
        lcp = getNextLexicalComponent(la);
    }
    free(lcp.lexeme);

    //We print the final state of the symbol table
    printSymbolTable(st);


    //And we delete everything before ending the program
    deleteLexicalAnalyzer(&la);
    deleteSymbolTable(&st);
    deleteReaderSystem(&rs);

    return EXIT_SUCCESS;

}
