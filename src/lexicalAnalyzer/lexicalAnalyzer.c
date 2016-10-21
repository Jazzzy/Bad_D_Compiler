//
// Created by jazzzy on 10/21/16.
//

#include "lexicalAnalyzer.h"
#include <stdlib.h>


void initOperators(listOfOperators **mList, char *pathToOperators) {


    int i = 0;
    *mList = (listOfOperators *) malloc(sizeof(listOfOperators));
    (*mList)->length = 4;                           //TODO: Read here the length from the file.
    (*mList)->list = (operator *) malloc(sizeof(operator) * (*mList)->length);

    for (i = 0; i < (*mList)->length; i++) {        //We make sure we init all the values
        (*mList)->list[i].isPossible = 1;
        int j = 0;
        for (j = 0; j < 4; j++) {
            (*mList)->list[i].str[j] = '\0';
        }
        (*mList)->list[i].length = 0;
        (*mList)->list[i].lexicalComponent = 0;
    }

                                                    //TODO: Read here the values from the file.
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


void initLexicalAnalyzer(lexicalAnalyzer **la, readerSystem *rs, symbolTable *st, char *pathToDefine, char *pathToOperators) {
    *la = (lexicalAnalyzer *) malloc(sizeof(lexicalAnalyzer));
    (*la)->mReaderSystem = rs;
    (*la)->mSymbolTable = st;                                       //TODO: Fill the symbolTable with the define file



    initOperators(&(*la)->mListOfOperators, pathToOperators);        //TODO: Fill the operator array with the operators file
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





        //Checking for operators
        int chkResult = checkAllOperators(la->mListOfOperators, 0, c);              //If the character can be the beginning of an operator
        if (chkResult != 0) {
            int position = 1;
            c = getNextChar(rs);                                                    //We take the next character to continue testing
            chkResult = checkAllOperators(la->mListOfOperators, position, c);       //We test if the next char can be the next part of the operator
            while (chkResult != 1) {                                                //And we test while we don't have only one operator
                if (chkResult == 0) {                                               //If we have no operators possible then there is an error
                    printf("ERROR PARSING ARGUMENT");
                    return -1;
                } else if (chkResult == 2) {                                    //If we have more than two operators possible we need to check more characters
                    c = getNextChar(rs);
                    position++;
                    chkResult = checkAllOperators(la->mListOfOperators, position, c);
                }
            }
            //Here we already know the operator since it's only one option left

            //But we need to read all of it's characters.
            while (readAllFromOperator(la->mListOfOperators, position, c)) {    //So we keep reading and checking
                position++;
                c = getNextChar(rs);                                            //And getting the remaining characters
            }
            lexComp = getOnlyPossibleOperator(la->mListOfOperators);            //We get the lexical component of the only operator possible
            char *lex = getCurrentLex(rs);                                      //Get the lexem
            returnChar(rs);                                                     //And return the last chat that was used for checking the end of the operator
            printf("Detected lexem: [%s]\n", lex);                              //Print it
            free(lex);                                                          //Free it
            resetListOfOperators(&(la->mListOfOperators));                      //Reset the operator list to make them all possible again for future iterations.
            return lexComp;                                                     //And return the lexical component.
        } else {
            resetListOfOperators(&(la->mListOfOperators));                      //We need to reset the list of operators to set them all to possible again.





        }

        //printf("Iteration %d", i++);

    } while (1);


}