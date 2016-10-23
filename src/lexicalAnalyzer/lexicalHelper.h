//
// Created by jazzzy on 10/23/16.
//

#ifndef BDC_LEXICALHELPER_H
#define BDC_LEXICALHELPER_H


#include "lexicalAnalyzer.h"

void readEmptyData(lexicalAnalyzer *la, char *c);

short couldBeIdent(char c);

short isPartOfIdent(char c);


short couldBeNumber(char c);

short isBinPrefix(char c);

short isPartOfBinary(char c);

short isPartOfNumber(char c);

short couldBeFloat(char c);

short couldBeSciNo(char c);

short isPlusMinus(char c);

short canBeInNumber(char c);

short couldBeStrLit(char c);

short isEndOfString(char c);

short isScapeCharacter(char c);

short isBinFirst(char c);

#endif //BDC_LEXICALHELPER_H
