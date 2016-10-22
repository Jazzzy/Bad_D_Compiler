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
    printf("LENGTH: %d\n", length);

    (*mList)->length = length;
    (*mList)->list = (operator *) malloc(sizeof(operator) * (*mList)->length);

    /*for (i = 0; i < (*mList)->length; i++) {        //We make sure we init all the values
        (*mList)->list[i].isPossible = 1;
        int j = 0;
        for (j = 0; j < 4; j++) {
            (*mList)->list[i].str[j] = '\0';
        }
        (*mList)->list[i].length = 0;
        (*mList)->list[i].lexicalComponent = 0;
    }*/
    i = 0;
    while (fgets(line, sizeof(line), file)) {
        int j;
        int opeLength;
        char opeString[4] = {'\0'};
        unsigned int opeLexComp;

        sscanf(line, "%d %[^ ] %d\n", &opeLength, opeString, &opeLexComp);
        (*mList)->list[i].isPossible = 1;
        for (j = 0; j < 4; j++) {
            (*mList)->list[i].str[j] = opeString[j];
        }
        (*mList)->list[i].length = opeLength;
        (*mList)->list[i].lexicalComponent = opeLexComp;
        printf("OPERATOR DATA: LENGTH [%d], STRING [%s], LEXICAL COMPONENT [%d]\n", (*mList)->list[i].length, (*mList)->list[i].str,
               (*mList)->list[i].lexicalComponent);
        i++;
    }
    /* may check feof here to make a difference between eof and io failure -- network
       timeout for instance */

    fclose(file);

    return;
}