//
// Created by jazzzy on 10/15/16.
//

#include <stdlib.h>
#include <string.h>
#include "bHashTable.h"


hashElement *table[HASHSIZE];

unsigned int badHash(char *key) {


}

/*
 * The key gets copied because we dont want to run the risk of some other part of the code
 * deleting the key data.
 *
 * That said, the data itself is not copied since this could come at a higher cost and we
 * assume that we are not deleting the data we need since this hash table will be used just to
 * locate the data fast.
 *
 * */
hashElement *addElement(char *key, void *data) {

    hashElement *newElement, *oldElement;
    newElement = (hashElement *) malloc(sizeof(hashElement));
    int i;

    for (i = 0; i < strlen(key); i++) {                 //We could copy the key into out struct
        newElement->key[i] = key[i];
    }

    newElement->data = data;

    unsigned int hashNumber = badHash(key);

    if (table[hashNumber] == NULL) {
        table[hashNumber] = newElement;
        newElement->next = NULL;
    } else {
        oldElement = table[hashNumber];
        while (oldElement->next != NULL) {
            oldElement = oldElement->next;
        }
        oldElement->next = newElement;
        newElement->next = NULL;
    }
    return newElement;

}

hashElement *getElement(char *key) {

    hashElement *currentElement;
    unsigned int hashNumber = badHash(key);

    if (table[hashNumber] == NULL) return NULL;

    if (strcmp(table[hashNumber]->key, key) == 0) {
        return table[hashNumber];
    } else {
        currentElement = table[hashNumber];
        while (currentElement->next != NULL) {
            currentElement = currentElement->next;
            if (strcmp(currentElement->key, key) == 0) {
                return currentElement;
            }
        }
        return NULL;
    }
}

int deleteElement(char *key) {

    hashElement *currentElement;
    unsigned int hashNumber = badHash(key);

    if (table[hashNumber] == NULL) return HASH_ERROR;   //We cannot detete an element that is not in the table

    if (strcmp(table[hashNumber]->key, key) == 0) {     //If we find the element in the first position
        currentElement = table[hashNumber];
        table[hashNumber] = currentElement->next;       //Its child will be now in the first position
        free(currentElement);                           //And we delete the element
        return HASH_SUCCESS;
    } else {                                                //If the element seems to be in the list
        hashElement *parentElement = table[hashNumber];     //The parent element is the first one in the list
        currentElement = parentElement->next;               //And the current element is its child
        while (currentElement != NULL) {                    //While we are not in the last element
            if (strcmp(currentElement->key, key) == 0) {    //If we have found the element
                parentElement->next = currentElement->next; //The parent's child will now be the son's child
                free(currentElement);                       //And we delete the element
                return HASH_SUCCESS;
            }                                               //if we have not found the element in this iteration
            parentElement = currentElement;                 //The parent now is the child of the old parent (current)
            currentElement = currentElement->next;          //The new current element is the son of the current element
        }
        return HASH_ERROR;                  //If we can't find the element at all in the list then we cant delete it
    }
}

void printStatus() {


}

void printData() {


}