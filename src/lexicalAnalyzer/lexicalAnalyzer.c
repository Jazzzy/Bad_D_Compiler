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
 *  -Should we insert an identifier in the symbol table when we see it more than once?
 *  -Do we need an explicit syntactical analyzer or can it be in the main?
 *
 *
 * */

/**
 * Saves memory for the operators and calls the parser to fill the data.
 *
 * */
void initOperators(listOfOperators **mList, char *pathToOperators) {
    *mList = (listOfOperators *) malloc(sizeof(listOfOperators));
    parseOperators(mList, pathToOperators);
}

/**
 *
 * Resets the list of operators to say that all of them are possible again.
 *
 * */
void resetListOfOperators(listOfOperators **mList) {
    int i;
    for (i = 0; i < (*mList)->length; i++) {
        (*mList)->list[i].isPossible = 1;
    }
}

/**
 * Initializes the lexical analyzer and the operators
 *
 * rs and st need to be init already.
 *
 * */
void initLexicalAnalyzer(lexicalAnalyzer **la, readerSystem *rs, symbolTable *st, char *pathToDefine, char *pathToOperators) {
    *la = (lexicalAnalyzer *) malloc(sizeof(lexicalAnalyzer));
    (*la)->mReaderSystem = rs;
    (*la)->mSymbolTable = st;
    initOperators(&(*la)->mListOfOperators, pathToOperators);
}

/**
 * Deletes the list of operators
 *
 * */
void deleteOperators(listOfOperators **mList) {
    free((*mList)->list);
    free(*mList);
}

/**
 * Deletes the whole lexical analyzer
 *
 * */
void deleteLexicalAnalyzer(lexicalAnalyzer **la) {
    deleteOperators(&(*la)->mListOfOperators);
    free(*la);
}


/**
 * Detects if a char can be part of an operator for a given position.
 *
 * If the char read is valid for that operator we return TRUE
 *
 * If the operator has only X elements and we are checking position X+1 then we return TRUE
 * because the operator has already been read and detected as valid, we need to discard any other options.
 *
 * */
short detectOperator(operator *anOperator, int position, char c) {
    short toReturn;
    toReturn = anOperator->length <= position || (anOperator->str[position] == c);
    return toReturn;
}

short checkAllOperators(listOfOperators *mList, int position, char c) {
    int i;
    int numOfPosOp = 0;                                                     //Number of possible operators found
    int maxLength = 0;

    for (i = 0; i < (*mList).length; i++) {
        if ((*mList).list[i].isPossible == 1) {                             //If that operator is still possible
            if (detectOperator(&(*mList).list[i], position, c) == 1) {      //If that operator is still valid

                if ((*mList).list[i].length >= position) {
                    maxLength = (*mList).list[i].length;                    //This is the longest operator
                    numOfPosOp++;
                } else if ((*mList).list[i].length < position) {
                    (*mList).list[i].isPossible = 0;                        //We have found a longer operator
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
        return COMPONENT_ERROR;
    }
}

/**
 * Returns TRUE if the character found is the beginning of an operator.
 *
 * */
short couldBeOperator(listOfOperators *mList, char c) {
    return (checkAllOperators(mList, 0, c) != 0);
}

/**
 * Returns the only operator that is possible at the moment.
 *
 * Or the first one it finds but this is only called if there is only one.
 *
 * */
int getOnlyPossibleOperator(listOfOperators *mList) {
    int i;
    for (i = 0; i < (*mList).length; i++) {
        if ((*mList).list[i].isPossible == 1) {
            return (*mList).list[i].lexicalComponent;
        }
    }
    return -1;
}

/**
 * We read another character from the only operator possible.
 *
 * Returning FALSE when all the string has been read.
 *
 * */
int readAllFromOperator(listOfOperators *mList, int position, char c) {

    int i;

    for (i = 0; i < (*mList).length; i++) {
        if ((*mList).list[i].isPossible == 1) {                             //If that operator is still possible
            if (detectOperator(&(*mList).list[i], position, c) == 1) {      //For the only valid operator
                return ((*mList).list[i].str[position] == c);
            }
        }
    }
    return -1;
}

#define PRINT_LEXEM


/**
 * Returns the next lexical component found by the lexical analyzer.
 *
 * */
int getNextLexicalComponent(lexicalAnalyzer *la) {

    readerSystem *rs = la->mReaderSystem;

    //int i = 0;
    char c = 0;
    int lexComp;

    for (;;) {

        c = getNextChar(rs);                    //We get a character

        readEmptyData(la, &c);                  //After we read non useful data

        if (c == EOF)                           //If we have found the end of file we end the search.
            return END_OF_FILE;                 //Return End Of File here


        //-------------------------------------------------CHECKING FOR COMMENTS--------------------------------------------------

        short checkedForComment = 0;

        if (couldBeComment(c)) {                            //We could be at the beginning of a comment
            c = getNextChar(rs);
            checkedForComment = 1;
            if (isBegOfNestedComment(c)) {                  //We are at the beginning of a nested comment
                c = getNextChar(rs);
                readUntilEndOfNestedComment(la, &c);        //We read until we have finished the nested comment
                char *lex = getCurrentLex(rs);              //We get the string that corresponds to the comment
                returnChar(rs);
#ifdef PRINT_LEXEM
                printf("Detected comment: [%s]\n", lex);    //Print it
#endif
                free(lex);                                  //And here we free it but I'm not sure about what to do with it.
                return NESTED_COMMENT;
            } else if (isBegOfBlockComment(c)) {            //We are at the beginning of a block comment
                c = getNextChar(rs);

                if (isBegOfBlockComment(c)) {               //If we find another '*' then this could be a documentation comment.
                    c = getNextChar(rs);
                    if (couldBeComment(c)) {                //If we find a '/' then the comment is a normal block comment like this "/**/"
                        c = getNextChar(rs);
                        char *lex = getCurrentLex(rs);
                        returnChar(rs);
#ifdef PRINT_LEXEM
                        printf("Detected comment: [%s]\n", lex);
#endif
                        free(lex);
                        return BLOCK_COMMENT;
                    } else {                                //Else it is a documentation comment
                        readUntilEndOfBlockComment(la, &c); //We read until the end of the comment
                        char *lex = getCurrentLex(rs);
                        returnChar(rs);
#ifdef PRINT_LEXEM
                        printf("Detected comment: [%s]\n", lex);
#endif
                        free(lex);
                        return DOCUMENTATION_COMMENT;       //And return a different lexical component.

                    }

                }
                readUntilEndOfBlockComment(la, &c);         //If we end up here is just a normal block comment with something inside it.
                char *lex = getCurrentLex(rs);              //We just to the same as above to get the lexeme, print it and return the lexical component.
                returnChar(rs);
#ifdef PRINT_LEXEM
                printf("Detected comment: [%s]\n", lex);
#endif
                free(lex);
                return BLOCK_COMMENT;
            } else if (isBegOfLineComment(c)) {             //If we have found "//" then this is a line comment
                c = getNextChar(rs);
                readUntilEndOfLineComment(la, &c);          //We read until the next line
                char *lex = getCurrentLex(rs);              //And repeat the steps above to return the correct lexical component.
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

                    manageNonFatalErrorWithLine(ERR_BAD_OPERATOR, "Found an error when parsing an operator", la->mReaderSystem->currentLine,
                                                la->mReaderSystem->currentPosition);
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


        if (couldBeStrLit(c)) {                                                 //If this can be a string literal
            c = getNextChar(rs);
            if (c == EOF) {//ERROR CHECKING FOR END OF FILE
                manageFatalErrorWithLine(ERR_UNEXPECTED_EOF, "EOF found inside a string", la->mReaderSystem->currentLine,
                                         la->mReaderSystem->currentPosition);
                return COMPONENT_ERROR;
            }
            while (!isEndOfString(c)) {                                         //While we don't find the closing character
                if (isScapeCharacter(c)) {                                      //If we find an scape character
                    c = getNextChar(rs);                                        //The we get the next character to that and ignore it
                    if (c == EOF) {//ERROR CHECKING FOR END OF FILE
                        manageFatalErrorWithLine(ERR_UNEXPECTED_EOF, "EOF found inside a string", la->mReaderSystem->currentLine,
                                                 la->mReaderSystem->currentPosition);
                        return COMPONENT_ERROR;
                    }
                }
                c = getNextChar(rs);                                            //We keep getting characters
                if (c == EOF) {//ERROR CHECKING FOR END OF FILE
                    manageFatalErrorWithLine(ERR_UNEXPECTED_EOF, "EOF found inside a string", la->mReaderSystem->currentLine,
                                             la->mReaderSystem->currentPosition);
                    return COMPONENT_ERROR;
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


        if (couldBeIdent(c)) {                                                  //If this can be an alphanumeric string
            while (isPartOfIdent(c)) {                                          //We get all the characters that can still be part of it
                c = getNextChar(rs);
            }
            char *lex = getCurrentLex(rs);                                      //We get the lexeme
            returnChar(rs);
#ifdef PRINT_LEXEM
            printf("Detected lexem: [%s]\n", lex);                              //And print it
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


        if (couldBeNumber(c)) {                                             //If this is the start of a number
            char cAux = c;
            c = getNextChar(rs);
            if (isBinFirst(cAux) && isBinPrefix(c)) {                       //We have read a binary number
                c = getNextChar(rs);
                if (c == EOF) {//ERROR CHECKING FOR END OF FILE
                    manageFatalErrorWithLine(ERR_UNEXPECTED_EOF, "EOF found after incomplete definition of a binary number", la->mReaderSystem->currentLine,
                                             la->mReaderSystem->currentPosition);
                    return COMPONENT_ERROR;
                }
                if (!isPartOfBinary(c)) {                                   //If it does not have a 0 or 1 after the definition then there is an error
                    manageNonFatalErrorWithLine(ERR_BAD_BINARY, "Found incomplete definition of a binary number", la->mReaderSystem->currentLine,
                                                la->mReaderSystem->currentPosition);
                    char *lex = getCurrentLex(rs);
                    returnChar(rs);
#ifdef PRINT_LEXEM
                    printf("Error in: [%s]\n", lex);
#endif
                    free(lex);
                    return COMPONENT_ERROR;
                }
                while (isPartOfBinary(c)) {                                 //We read all 0s and 1s
                    c = getNextChar(rs);
                }
                char *lex = getCurrentLex(rs);                              //And get the lexeme
                returnChar(rs);
#ifdef PRINT_LEXEM
                printf("Detected lexem: [%s]\n", lex);
#endif
                free(lex);
                return BINARY_LITERAL;

            } else if (!canBeInNumber(c)) {                 //If the number has a length of one then it is an integer
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
                                                 la->mReaderSystem->currentLine,
                                                 la->mReaderSystem->currentPosition);
                        return COMPONENT_ERROR;
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
        }//End of number checking

        //Could continue here in order to check more things


    }
}