//
// Created by jazzzy on 10/23/16.
//

#include "lexicalHelper.h"
#include "lexicalAnalyzer.h"
#include "../errorManager/errorManager.h"
#include <ctype.h>
#include <stdio.h>

short notRelevantChar(char c) {
    return (isspace(c) || c == EOF);
}

void readEmptyData(lexicalAnalyzer *la, char *c) {

    while (notRelevantChar(*c)) {
        switch (*c) {
            case '\n':
                la->currentLine++;                              //We update the number of line.
                //printf("\n\nLINE [%d]\n\n", la->currentLine);
                break;
            default:
                break;
        }
        if (*c == EOF) {
            return;
        }

        advanceBeginning(la->mReaderSystem);                    //We advance one position in the beg pointer of the reader system
        *c = getNextChar(la->mReaderSystem);                    //And we get the next character
    }


}

short readUntilEndOfNestedComment(lexicalAnalyzer *la, char *c) {


    for (;;) {

        if (*c == EOF) {//ERROR CHECKING FOR END OF FILE
            manageFatalErrorWithLine(ERR_UNEXPECTED_EOF, "EOF found inside a nested comment", la->currentLine);
            return -1;
        }

        if (*c == '\n') {
            la->currentLine++;
        }

        if (couldBeComment(*c)) {                       //We could have found the beggining of another nested comment
            *c = getNextChar(la->mReaderSystem);        //We get the next char
            if (isBegOfNestedComment(*c)) {             //And test for the beginning of another nested comment
                readUntilEndOfNestedComment(la, c);     //If it is we call this function again to search for the end
            }
        }

        if (*c == '+') {
            *c = getNextChar(la->mReaderSystem);        //We get the next char
            if (*c == '/') {                            //And test for the end of the nested comment
                *c = getNextChar(la->mReaderSystem);
                return 1;
            }

        }

        *c = getNextChar(la->mReaderSystem);
    }

}

short readUntilEndOfBlockComment(lexicalAnalyzer *la, char *c) {


    for (;;) {

        if (*c == EOF) {//ERROR CHECKING FOR END OF FILE
            manageFatalErrorWithLine(ERR_UNEXPECTED_EOF, "EOF found inside a block comment", la->currentLine);
            return -1;
        }

        if (*c == '\n') {
            la->currentLine++;
        }

        if (*c == '*') {
            *c = getNextChar(la->mReaderSystem);        //We get the next char
            if (*c == '/') {                            //And test for the end of the nested comment
                *c = getNextChar(la->mReaderSystem);
                return 1;
            }
        }

        *c = getNextChar(la->mReaderSystem);
    }

}

short readUntilEndOfLineComment(lexicalAnalyzer *la, char *c) {

    for (;;) {

        if (*c == EOF) {
            return 1;
        }

        if (*c == '\n') {
            *c = getNextChar(la->mReaderSystem);
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

