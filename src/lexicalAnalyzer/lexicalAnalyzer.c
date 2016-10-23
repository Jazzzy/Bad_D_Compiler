//
// Created by jazzzy on 10/21/16.
//

#include "lexicalAnalyzer.h"
#include <stdlib.h>
#include "../DLang/D_DEFINE_RESERVED_WORDS.h"
#include "../DLang/D_DEFINE_NON_RESERVED_WORDS.h"
#include "../utils/operatorParser.h"
#include "lexicalHelper.h"
#include "../symbolTable/symbolTable.h"
#include "../errorManager/errorManager.h"


/*
 *
 * TODO: QUESTIONS:
 *
 *  -What should we do with number and string literals? Store them?
 *  -What should we do with comments, ignore them or send them?
 *
 * */


void initOperators(listOfOperators **mList, char *pathToOperators) {
    *mList = (listOfOperators *) malloc(sizeof(listOfOperators));
    parseOperators(mList, pathToOperators);
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
    (*la)->mSymbolTable = st;
    initOperators(&(*la)->mListOfOperators, pathToOperators);
    (*la)->currentLine = 1;
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

    //printf("VALID: %c and %c? %d\n", anOperator->str[position], c, toReturn);

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
        manageFatalError(ERR_BAD_OPERATOR,
                         "Possible operators are negative, this should never be possible, how did you get here?\n\tTell me at gladislacebrafeliz@gmail.com :)");
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
    int lexComp;

    for (;;) {

        c = getNextChar(rs);        //We get a character

        readEmptyData(la, &c);

        if (c == EOF)               //If we have found the end of file we end the search.
            return END_OF_FILE;              //Return End Of File here


        //-------------------------------------------------CHECKING FOR COMMENTS--------------------------------------------------

        short checkedForComment = 0;

        if (couldBeComment(c)) {
            c = getNextChar(rs);
            checkedForComment = 1;
            if (isBegOfNestedComment(c)) {
                c = getNextChar(rs);
                readUntilEndOfNestedComment(la, &c);
                char *lex = getCurrentLex(rs);
                returnChar(rs);
#ifdef PRINT_LEXEM
                printf("Detected comment: [%s]\n", lex);
#endif
                free(lex);
                return NESTED_COMMENT;
            } else if (isBegOfBlockComment(c)) {
                c = getNextChar(rs);
                readUntilEndOfBlockComment(la, &c);
                char *lex = getCurrentLex(rs);
                returnChar(rs);
#ifdef PRINT_LEXEM
                printf("Detected comment: [%s]\n", lex);
#endif
                free(lex);
                return BLOCK_COMMENT;
            } else if (isBegOfLineComment(c)) {
                c = getNextChar(rs);
                readUntilEndOfLineComment(la, &c);
                char *lex = getCurrentLex(rs);
                returnChar(rs);
#ifdef PRINT_LEXEM
                printf("Detected comment: [%s]\n", lex);
#endif
                free(lex);
                return BLOCK_COMMENT;
            }
            returnChar(rs);
        }

        //-------------------------------------------------CHECKING FOR OPERATORS--------------------------------------------------

        int chkResult;
        if (checkedForComment) {
            chkResult = checkAllOperators(la->mListOfOperators, 0, '/');              //If the character can be the beginning of an operator
        } else {
            chkResult = checkAllOperators(la->mListOfOperators, 0, c);              //If the character can be the beginning of an operator
        }
        if (chkResult != 0) {
            int position = 1;
            c = getNextChar(rs);
            chkResult = checkAllOperators(la->mListOfOperators, position, c);       //We test if the next char can be the next part of the operator
            while (chkResult != 1) {                                                //And we test while we don't have only one operator
                if (chkResult == 0) {                                               //If we have no operators possible then there is an error

                    manageNonFatalErrorWithLine(ERR_BAD_OPERATOR, "Found an error when parsing an operator", la->currentLine);
                    char *lex = getCurrentLex(rs);
                    returnChar(rs);
#ifdef PRINT_LEXEM
                    printf("Detected lexem: [%s]\n", lex);
#endif
                    free(lex);
                    resetListOfOperators(&(la->mListOfOperators));
                    return OPE_ERROR;

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
            char *lex = getCurrentLex(rs);                                      //Get the lexeme
            returnChar(rs);                                                     //And return the last chat that was used for checking the end of the operator
#ifdef PRINT_LEXEM
            printf("Detected lexem: [%s]\n", lex);                              //Print it
#endif
            free(lex);                                                          //Free it
            resetListOfOperators(&(la->mListOfOperators));                      //Reset the operator list to make them all possible again for future iterations.
            return lexComp;                                                     //And return the lexical component.
        } else {
            resetListOfOperators(&(la->mListOfOperators));                      //We need to reset the list of operators to set them all to possible again.
        }



        //-------------------------------------------------CHECKING FOR STRING LITERALS--------------------------------------------------


        if (couldBeStrLit(c)) {
            c = getNextChar(rs);
            if (c == EOF) {//ERROR CHECKING FOR END OF FILE
                manageFatalErrorWithLine(ERR_UNEXPECTED_EOF, "EOF found inside a string", la->currentLine);
                return -1;
            }
            while (!isEndOfString(c)) {
                if (isScapeCharacter(c)) {
                    c = getNextChar(rs);
                    if (c == EOF) {//ERROR CHECKING FOR END OF FILE
                        manageFatalErrorWithLine(ERR_UNEXPECTED_EOF, "EOF found inside a string", la->currentLine);
                        return -1;
                    }
                }
                c = getNextChar(rs);
                if (c == EOF) {//ERROR CHECKING FOR END OF FILE
                    manageFatalErrorWithLine(ERR_UNEXPECTED_EOF, "EOF found inside a string", la->currentLine);
                    return -1;
                }
            }
            c = getNextChar(rs);
            char *lex = getCurrentLex(rs);                                      //Get the lexeme
            returnChar(rs);
#ifdef PRINT_LEXEM
            printf("Detected lexem: [%s]\n", lex);                              //Print it
#endif
            free(lex);                                                          //Free it
            return STRING_LITERAL;


        }

        //-------------------------------------------------CHECKING FOR ALPHANUMERIC STRINGS--------------------------------------------------


        if (couldBeIdent(c)) {
            while (isPartOfIdent(c)) {
                c = getNextChar(rs);
            }
            char *lex = getCurrentLex(rs);
            returnChar(rs);
#ifdef PRINT_LEXEM
            printf("Detected lexem: [%s]\n", lex);
#endif

            symbolData *sd = searchLex(la->mSymbolTable, lex);
            if (sd == NULL) {                                                   //We have found an identifier that was not on the table
                sd = (symbolData *) malloc(sizeof(symbolData));
                sd->lexicalComponent = IDENTIFIER;
                addLex(&(la->mSymbolTable), lex, sd);

            } else {                                                            //There was already a reserved word or an identifier in the table.
                if (sd->lexicalComponent == IDENTIFIER) {                       //Identical identifier in the symbol table already
                    sd = (symbolData *) malloc(sizeof(symbolData));
                    sd->lexicalComponent = IDENTIFIER;
                    addLex(&(la->mSymbolTable), lex, sd);                       //We add it again, currently we will have repeated elements that are identical.
                } else {                                                        //If it is a reserved word
                    free(lex);                                                  //We need to free the lexeme since we don't need the original string.
                }
                return (sd->lexicalComponent);
            }


            return IDENTIFIER;
        }


        //-------------------------------------------------CHECKING FOR NUMERIC LITERALS--------------------------------------------------


        if (couldBeNumber(c)) {
            char cAux = c;
            c = getNextChar(rs);
            if (isBinFirst(cAux) && isBinPrefix(c)) {                       //We have read a binary number
                c = getNextChar(rs);
                if (c == EOF) {//ERROR CHECKING FOR END OF FILE
                    manageFatalErrorWithLine(ERR_UNEXPECTED_EOF, "EOF found after incomplete definition of a binary number", la->currentLine);
                    return -1;
                }
                while (isPartOfBinary(c)) {
                    c = getNextChar(rs);
                }
                char *lex = getCurrentLex(rs);
                returnChar(rs);
#ifdef PRINT_LEXEM
                printf("Detected lexem: [%s]\n", lex);
#endif
                free(lex);
                return BINARY_LITERAL;

            } else if (!canBeInNumber(c)) {                 //If the number has a length of one
                char *lex = getCurrentLex(rs);
                returnChar(rs);
#ifdef PRINT_LEXEM
                printf("Detected lexem: [%s]\n", lex);
#endif
                free(lex);
                return INTEGER_LITERAL;

            } else if (canBeInNumber(c)) {                  //The number is not binary and is longer than 2 characters
                int type = INTEGER_LITERAL;
                while (isPartOfNumber(c)) {                 //We read an array of numbers
                    c = getNextChar(rs);
                }                                           //We have read the whole string of numbers but we could have a float or scientific notation.
                if (couldBeFloat(c)) {                      //We check if the number has a point.
                    c = getNextChar(rs);                    //If this is a float we need to keep looking for all the numbers after the point
                    while (isPartOfNumber(c)) {
                        c = getNextChar(rs);
                    }
                    type = FLOAT_LITERAL;
                }

                if (couldBeSciNo(c)) {                      //Now we know we have either a float or an integer but we could find some scientific notation.
                    c = getNextChar(rs);
                    if (isPlusMinus(c)) {                   //The e or E can be followed by a plus or minus.
                        c = getNextChar(rs);
                    }
                    if (c == EOF) {//ERROR CHECKING FOR END OF FILE
                        manageFatalErrorWithLine(ERR_UNEXPECTED_EOF, "EOF found after incomplete definition of a number in scientific notation",
                                                 la->currentLine);
                        return -1;
                    }
                    while (isPartOfNumber(c)) {             //And after that we can only have an array of numbers
                        c = getNextChar(rs);
                    }
                }

                char *lex = getCurrentLex(rs);
                returnChar(rs);
#ifdef PRINT_LEXEM
                printf("Detected lexem: [%s]\n", lex);
#endif
                free(lex);
                return type;
            }
        }                                                   //End of number checking

    }
}