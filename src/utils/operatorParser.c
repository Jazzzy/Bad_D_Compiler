//
// Created by jazzzy on 10/22/16.
//

#include "operatorParser.h"
#include <stdlib.h>

void parseOperators(listOfOperators **mList, char *pathToOperators) {

    FILE *file = fopen(pathToOperators, "r");       //Open the file
    char line[256];                                 //Save space for a line, should not be longer than 256

    int length = 0, i;

    fgets(line, sizeof(line), file);
    sscanf(line, "%d\n", &length);
    //printf("LENGTH: %d\n", length);

    (*mList)->length = length;
    (*mList)->list = (operator *) malloc(sizeof(operator) * (*mList)->length);

    i = 0;
    while (fgets(line, sizeof(line), file)) {
        int j;
        int opeLength = 4;
        char opeString[4] = "\0\0\0";
        unsigned int opeLexComp;

        sscanf(line, "%d %[^ ] %d\n", &opeLength, opeString, &opeLexComp);

#ifdef __clang__
        if(opeLength==0) opeLength=4;       //For some reason we can't read the number 4 when compiling with clang
#endif

        (*mList)->list[i].isPossible = 1;
        for (j = 0; j < 4; j++) {
            (*mList)->list[i].str[j] = opeString[j];
        }
        (*mList)->list[i].length = opeLength;
        (*mList)->list[i].lexicalComponent = opeLexComp;
        /*printf("OPERATOR DATA: LENGTH [%d][%d], STRING [%s], LEXICAL COMPONENT [%d]\n", opeLength, (*mList)->list[i].length, (*mList)->list[i].str,
               (*mList)->list[i].lexicalComponent);*/
        i++;
    }


    fclose(file);

    return;
}