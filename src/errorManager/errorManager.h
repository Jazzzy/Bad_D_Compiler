//
// Created by jazzzy on 10/23/16.
//

#ifndef BDC_ERRORMANAGER_H
#define BDC_ERRORMANAGER_H

typedef enum {
    ERR_UNEXPECTED_EOF,
    ERR_BAD_OPERATOR
} errorType;


void manageNonFatalError(int code, char *message);

void manageNonFatalErrorWithLine(int code, char *message, int line);

void manageFatalError(int code, char *message);

void manageFatalErrorWithLine(int code, char *message, int line);


#endif //BDC_ERRORMANAGER_H
