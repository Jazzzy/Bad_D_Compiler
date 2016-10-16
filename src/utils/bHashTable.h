
#ifndef BDC_BHASHTABLE_H
#define BDC_BHASHTABLE_H

typedef enum {HASH_SUCCESS,HASH_ERROR} returnType;


#define MAXKEYLENGTH 100
#define TABLESIZE 10007

typedef struct hashElement hashElement;

struct hashElement {
    char key[MAXKEYLENGTH];
    void *data;
    hashElement *next;
};

typedef struct hashElement **hashTable;


int initHashTable(hashTable *table);

int deleteHastTable(hashTable *table);

unsigned int badHash(char *key);

hashElement *addElement(hashTable *table,char *key, void *data); //TODO: Make sure if we should allow duplicates or not

hashElement *getElement(hashTable table,char *key);

int deleteElement(hashTable *table,char *key);

void printStatus(hashTable table);

void printData(hashTable table);







#endif //BDC_BHASHTABLE_H
