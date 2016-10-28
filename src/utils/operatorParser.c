#include "operatorParser.h"
#include "../errorManager/errorManager.h"
#include "../lexicalAnalyzer/lexicalAnalyzer.h"
#include <stdlib.h>

void parseOperators(listOfOperators **mList, char *pathToOperators) {

    FILE *file = fopen(pathToOperators, "r");       //Open the file

    if (file == NULL) {
        char buffer[256];
        sprintf(buffer, "Error opening file: %s\n", pathToOperators);
        manageFatalError(ERR_FILE_ERROR, buffer);
    }

    char line[256];                                 //Save space for a line, should not be longer than 256

    int length = 0, i;

    fgets(line, sizeof(line), file);
    sscanf(line, "%d\n", &length);


    (*mList)->length = length;
    (*mList)->startPosition = 0;
    (*mList)->numPossible = length;
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
        i++;
    }


    fclose(file);

    return;
}