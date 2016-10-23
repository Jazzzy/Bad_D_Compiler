//
// Created by jazzzy on 10/23/16.
//

#include "lexicalHelper.h"
#include "lexicalAnalyzer.h"
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