#include "utils/bHashTable.h"
#include <stdlib.h>
#include <stdio.h>

int main() {



    //---------------------Testing hashTable-------------------------
    hashTable mHashTable = NULL;
    initHashTable(&mHashTable);
    char *key = "HELLO!";
    char *aux = "FIRST";
    char *aux2 = "SECOND";
    addElement(&mHashTable, key, aux);
    addElement(&mHashTable, key, aux2);


    char *key2 = "HELLO2";
    addElement(&mHashTable, key2, aux);

    printf("Element has this data: %s\n",(char*)getElement(mHashTable,key2)->data);

    addElement(&mHashTable, key2, aux2);

    printf("Element has this data: %s\n",(char*)getElement(mHashTable,key2)->data);

    printStatus(mHashTable);
    printData(mHashTable);
    deleteHastTable(&mHashTable);
    //---------------------------------------------------------------






    return 0;
}