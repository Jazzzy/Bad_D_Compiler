#include "lexicalHelper.h"
#include "../errorManager/errorManager.h"
#include <ctype.h>

short notRelevantChar(char c) {
    return (isspace(c) || c == EOF);
}

/**
 * Reads what is considered not relevant data in this program
 * and updates the beg pointer in the reader system.
 *
 * */
void readEmptyData(lexicalAnalyzer *la, char *c) {

    while (notRelevantChar(*c)) {
        /*switch (*c) {                                           //Use this in case we want to consider data like tabs or newlines.
            default:
                break;
        }*/
        if (*c == EOF) {
            return;
        }

        advanceBeginning(la->mReaderSystem);                    //We advance one position in the beg pointer of the reader system
        *c = getNextChar(la->mReaderSystem);                    //And we get the next character
    }


}


/**
 *
 * Reads data until the end of a nested comment, calling itself when another nested comment is found
 *
 * */
short readUntilEndOfNestedComment(lexicalAnalyzer *la, char *c) {


    for (;;) {

        short readAnotherOne = 1;

        if (*c == EOF) {//ERROR CHECKING FOR END OF FILE
            manageFatalErrorWithLine(ERR_UNEXPECTED_EOF, "EOF found inside a nested comment", la->mReaderSystem->currentLine,
                                     la->mReaderSystem->currentPosition);
            return -1;
        }

        if (couldBeComment(*c)) {                       //We could have found the beginning of another nested comment
            *c = getNextChar(la->mReaderSystem);        //We get the next char
            readAnotherOne = 0;
            if (isBegOfNestedComment(*c)) {             //And test for the beginning of another nested comment
                readUntilEndOfNestedComment(la, c);     //If it is we call this function again to search for the end
            }
        }

        if (*c == '+') {
            *c = getNextChar(la->mReaderSystem);        //We get the next char
            readAnotherOne = 0;
            if (*c == '/') {                            //And test for the end of the nested comment
                *c = getNextChar(la->mReaderSystem);
                return 1;
            }
        }

        if (readAnotherOne)
            *c = getNextChar(la->mReaderSystem);
    }

}

/**
 *
 * Reads data until the end of a block comment
 *
 * */
short readUntilEndOfBlockComment(lexicalAnalyzer *la, char *c) {

    for (;;) {

        short readAnotherOne = 1;

        if (*c == EOF) {//ERROR CHECKING FOR END OF FILE
            manageFatalErrorWithLine(ERR_UNEXPECTED_EOF, "EOF found inside a block comment", la->mReaderSystem->currentLine,
                                     la->mReaderSystem->currentPosition);
            return -1;
        }

        if (*c == '*') {
            *c = getNextChar(la->mReaderSystem);        //We get the next char
            readAnotherOne = 0;
            if (*c == '/') {                            //And test for the end of the nested comment
                *c = getNextChar(la->mReaderSystem);
                return 1;
            }
        }

        if (readAnotherOne)
            *c = getNextChar(la->mReaderSystem);

    }

}

/**
 * Reads data until the end of a line comment
 *
 * */
short readUntilEndOfLineComment(lexicalAnalyzer *la, char *c) {

    for (;;) {
        if (*c == EOF) {
            return 1;
        }
        if (*c == '\n') {
            return 1;
        }
        *c = getNextChar(la->mReaderSystem);
    }
}

short couldBeIdent(char c) {
    return (isalpha(c) || c == '_');
}

short isPartOfIdent(char c) {
    return (isalnum(c) || c == '_');
}

short couldBeNumber(char c) {
    return (isdigit(c));
}

short isBinPrefix(char c) {
    return (c == 'B' || c == 'b');
}

short isPartOfBinary(char c) {

    return (c == '0' || c == '1' || c == '_');
}

short isPartOfNumber(char c) {
    return (isdigit(c) || c == '_');
}

short couldBeFloat(char c) {
    return (c == '.');
}

short couldBeSciNo(char c) {
    return (c == 'e' || c == 'E');
}

short isPlusMinus(char c) {
    return (c == '+' || c == '-');
}

short canBeInNumber(char c) {
    return (isPartOfNumber(c) || couldBeFloat(c) || couldBeSciNo(c));

}

short couldBeStrLit(char c) {
    return (c == '\"');

}

short isEndOfString(char c) {
    return (c == '\"');
}

short isScapeCharacter(char c) {
    return (c == '\\');
}

short isBinFirst(char c) {
    return (c == '0');
}

short couldBeComment(char c) {
    return (c == '/');
}

short isBegOfNestedComment(char c) {
    return (c == '+');
}

short isBegOfBlockComment(char c) {
    return (c == '*');
}

short isBegOfLineComment(char c) {
    return (c == '/');
}

short isNewLine(char c){
    return (c == '\n');
}
