//
// Created by jazzzy on 10/21/16.
//

#include "lexicalAnalyzer.h"
#include <stdlib.h>


void initOperators(listOfOperators **mList) {
    int i = 0;
    *mList = (listOfOperators *) malloc(sizeof(listOfOperators));
    (*mList)->length = 4;
    (*mList)->list = (operator *) malloc(sizeof(operator) * (*mList)->length);

    for (i = 0; i < (*mList)->length; i++) {
        (*mList)->list[i].isPossible = 1;
    }
    (*mList)->list[0].length = 2;
    (*mList)->list[0].str[0] = '!';
    (*mList)->list[0].str[1] = '=';
    (*mList)->list[0].lexicalComponent = 400;


    (*mList)->list[1].length = 2;
    (*mList)->list[1].str[0] = '=';
    (*mList)->list[1].str[1] = '=';
    (*mList)->list[1].lexicalComponent = 500;


    (*mList)->list[2].length = 3;
    (*mList)->list[2].str[0] = '=';
    (*mList)->list[2].str[1] = '=';
    (*mList)->list[2].str[2] = '=';
    (*mList)->list[2].lexicalComponent = 600;

    (*mList)->list[3].length = 1;
    (*mList)->list[3].str[0] = '=';
    (*mList)->list[3].lexicalComponent = 900;

}

void resetListOfOperators(listOfOperators **mList) {
    int i;
    for (i = 0; i < (*mList)->length; i++) {
        (*mList)->list[i].isPossible = 1;
    }
}


void initLexicalAnalyzer(lexicalAnalyzer **la, readerSystem *rs, symbolTable *st) {
    *la = (lexicalAnalyzer *) malloc(sizeof(lexicalAnalyzer));
    (*la)->mReaderSystem = rs;
    (*la)->mSymbolTable = st;
    initOperators(&(*la)->mListOfOperators);
}

void deleteOperators(listOfOperators **mList) {
    free((*mList)->list);
    free(*mList);
}

void deleteLexicalAnalyzer(lexicalAnalyzer **la) {
    deleteOperators(&(*la)->mListOfOperators);
    free(*la);
}


short detectOperator(operator *anOperator, int position, char c) {
    /*
     * If the char read is valid for that operator we return TRUE
     *
     * If the operator has only X elements and we are checking position X+1 then we return TRUE
     * because the operator has already been read and detected as valid, we need to discard any other options.
     *
     * */

    //printf("CHAR [%c], OPERATOR CHAR [%c], LENGTH [%d], POSITION [%d]\n", c, anOperator->str[position], anOperator->length, position);

    short toReturn;
    toReturn = anOperator->length <= position || (anOperator->str[position] == c);
    //printf("RETURNING: %d\n\n", toReturn);
    return toReturn;
}

short checkAllOperators(listOfOperators *mList, int position, char c) {
    int i;
    int numOfPosOp = 0;         //Number of possible operators found
    int maxLength = 0;

    for (i = 0; i < (*mList).length; i++) {
        if ((*mList).list[i].isPossible == 1) {             //If that operator is still possible
            if (detectOperator(&(*mList).list[i], position, c) == 1) {   //If that operator is still valid

                if ((*mList).list[i].length >= position) {
                    maxLength = (*mList).list[i].length;    //This is the longest operator
                    //allRead = ((*mList).list[i].length<position);
                    numOfPosOp++;
                } else if ((*mList).list[i].length < position) {
                    (*mList).list[i].isPossible = 0;        //We have found a longer operator
                }


            } else {
                (*mList).list[i].isPossible = 0;
            }
        }
    }

    if (numOfPosOp == 0) {
        //printf("We don't have any operator that is possible with this data.\n");
        return 0;
    } else if (numOfPosOp == 1) {
        //printf("We have found one operator possible\n");
        return 1;
    } else if (numOfPosOp > 1) {
        //printf("Various operators are possible.\n");
        return 2;
    } else {
        printf("ERROR\n");
        return -1;
    }
}

short couldBeOperator(listOfOperators *mList, char c) {     //Return TRUE if the character found is the beginning of an operator.
    return (checkAllOperators(mList, 0, c) != 0);
}

int getOnlyPossibleOperator(listOfOperators *mList) {
    int i;
    for (i = 0; i < (*mList).length; i++) {
        if ((*mList).list[i].isPossible == 1) {
            //printf("Detected: %s", (*mList).list[i].str);
            return (*mList).list[i].lexicalComponent;
        }
    }
    return -1;
}

int readAllFromOperator(listOfOperators *mList, int position, char c) {

    int i;

    for (i = 0; i < (*mList).length; i++) {
        if ((*mList).list[i].isPossible == 1) {             //If that operator is still possible
            if (detectOperator(&(*mList).list[i], position, c) == 1) {   //For the only valid operator
                return ((*mList).list[i].str[position] == c);
            }
        }
    }
    return -1;
}

int getNextLexicalComponent(lexicalAnalyzer *la) {

    readerSystem *rs = la->mReaderSystem;

    //int i = 0;
    char c = 0;
    int lexComp = -1;

    do {
        c = getNextChar(rs);        //We get a character
        if (c == EOF)               //If we have found the end of file we end the search.
            return -1;              //Return End Of File here


        //printf("%c", c);
        int chkResult = checkAllOperators(la->mListOfOperators, 0, c);
        if (chkResult != 0) {
            if (chkResult == 1) {       //We already know the operator with just one character
                int position = 1;
                c = getNextChar(rs);
                while (readAllFromOperator(la->mListOfOperators, position, c)) {        //But we need to read all of it's characters.
                    position++;
                    c = getNextChar(rs);
                }
            } else {                    //We need to look for more characters to know the operator
                int position = 1;
                c = getNextChar(rs);
                //printf("\t\t%c\n", c);
                chkResult = checkAllOperators(la->mListOfOperators, position, c);
                while (chkResult != 1) {
                    if (chkResult == 0) {
                        printf("ERROR PARSING ARGUMENT");
                        return -1;
                    } else if (chkResult == 2) {
                        c = getNextChar(rs);
                        printf("\t\t%c\n", c);
                        position++;
                        chkResult = checkAllOperators(la->mListOfOperators, position, c);
                    }

                }

            }
            lexComp = getOnlyPossibleOperator(la->mListOfOperators);
            char *lex = getCurrentLex(rs);
            printf("Detected lexem: [%s]\n", lex);
            free(lex);
            returnChar(rs);
            resetListOfOperators(&(la->mListOfOperators));
            return lexComp;

        } else {
            resetListOfOperators(&(la->mListOfOperators));
        }

        //printf("Iteration %d", i++);

    } while (1);


}