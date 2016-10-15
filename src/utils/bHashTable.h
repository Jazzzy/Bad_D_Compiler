
#ifndef BDC_BHASHTABLE_H
#define BDC_BHASHTABLE_H

typedef enum {HASH_SUCCESS,HASH_ERROR};


#define MAXKEYLENGTH 100
#define HASHSIZE 10007

typedef struct hashElement hashElement;

struct hashElement {
    char key[MAXKEYLENGTH];
    void *data;
    hashElement *next;
};

unsigned int badHash(char *key);

hashElement *addElement(char *key, void *data);

hashElement *getElement(char *key);

int deleteElement(char *key);

void printStatus();

void printData();







#endif //BDC_BHASHTABLE_H
