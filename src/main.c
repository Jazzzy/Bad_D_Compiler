#include "utils/bHashTable.h"
#include <stdlib.h>

int main() {



    //---------------------Testing hashTable-------------------------
    hashTable mHashTable = NULL;
    initHashTable(&mHashTable);
    char *key = "HELLO!";
    void *aux = NULL;
    addElement(&mHashTable, key, aux);
    addElement(&mHashTable, key, aux);
    char *key2 = "HELLO2";
    addElement(&mHashTable, key2, aux);
    addElement(&mHashTable, key2, aux);
    printStatus(mHashTable);
    printData(mHashTable);
    deleteHastTable(&mHashTable);
    //---------------------------------------------------------------






    return 0;
}