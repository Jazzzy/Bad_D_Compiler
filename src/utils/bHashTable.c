//
// Created by jazzzy on 10/15/16.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bHashTable.h"

//hashElement *table[TABLESIZE]; //If we wanted an static table


#define FNV1_32_INIT ((u_int32_t)0x811c9dc5)


u_int32_t fnv_32a_str(char *str, u_int32_t hval) { //This is the hash function
    unsigned char *s = (unsigned char *) str;   //We get the string as unsigned chars for the algorithm
    while (*s) {
        hval ^= (u_int32_t) *s++;                 //XOR the hash with the current octet (a char is an octet)
        //We multiply by the 32 bit FNV magic prime mod 2^32
        //Explanation of this in: http://isthe.com/chongo/tech/comp/fnv/
        hval += (hval << 1) + (hval << 4) + (hval << 7) + (hval << 8) + (hval << 24);
    }
    return hval;
}

unsigned int badHash(char *key) { //This controls the current hash value used by the hash function and makes the call to it
    return (fnv_32a_str(key, FNV1_32_INIT)) % TABLESIZE;
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


int initHashTable(hashTable *table) {
    *table = (hashTable) malloc(sizeof(hashElement *) * TABLESIZE);
    return HASH_SUCCESS;
}


int deleteHastTable(hashTable *table) {

    int i;
    hashElement *currentElement, *elementToDelete;
    printf("\n\nDELETING TABLE DATA ...\n\n");

    for (i = 0; i < TABLESIZE; i++) {
        if ((*table)[i] != NULL) {
            elementToDelete = (*table)[i];
            currentElement = elementToDelete->next;
            printf("Deleting element in %d: [%s]\n", i, elementToDelete->key);
            free(elementToDelete);
            while (currentElement != NULL) {
                elementToDelete = currentElement;
                currentElement = elementToDelete->next;
                printf("Deleting element in %d: [%s]\n", i, elementToDelete->key);
                free(elementToDelete);
            }
        }
    }
    free(*table);
    printf("\nDONE DELETING\n\n");

}

hashElement *addElement(hashTable *table, char *key, void *data) {

    hashElement *newElement, *oldElement;
    newElement = (hashElement *) malloc(sizeof(hashElement));
    int i;

    for (i = 0; i < strlen(key); i++) {                 //We could copy the key into our struct
        newElement->key[i] = key[i];
    }

    newElement->data = data;

    unsigned int hashNumber = badHash(key);

    if ((*table)[hashNumber] == NULL) {
        (*table)[hashNumber] = newElement;
        newElement->next = NULL;
    } else {
                                                        //We insert at the beginning of the list
        oldElement = (*table)[hashNumber];
        (*table)[hashNumber]=newElement;
        newElement->next=oldElement;


    }
    return newElement;

}

hashElement *getElement(hashTable table, char *key) {

    hashElement *currentElement;
    unsigned int hashNumber = badHash(key);

    if (table[hashNumber] == NULL) return NULL;

    if (strcmp((table)[hashNumber]->key, key) == 0) {
        return (table)[hashNumber];
    } else {
        currentElement = (table)[hashNumber];
        while (currentElement->next != NULL) {
            currentElement = currentElement->next;
            if (strcmp(currentElement->key, key) == 0) {
                return currentElement;
            }
        }
        return NULL;
    }
}

int deleteElement(hashTable *table, char *key) {

    hashElement *currentElement;
    unsigned int hashNumber = badHash(key);

    if (table[hashNumber] == NULL) return HASH_ERROR;   //We cannot detete an element that is not in the table

    if (strcmp((*table)[hashNumber]->key, key) == 0) {     //If we find the element in the first position
        currentElement = (*table)[hashNumber];
        (*table)[hashNumber] = currentElement->next;       //Its child will be now in the first position
        free(currentElement);                           //And we delete the element
        return HASH_SUCCESS;
    } else {                                                //If the element seems to be in the list
        hashElement *parentElement = (*table)[hashNumber];     //The parent element is the first one in the list
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

void printStatus(hashTable table) {

    int maxLength, totalElements, usedSlots, nodesInCurList, i;
    double average;
    hashElement *currentElement;

    maxLength = totalElements = usedSlots = nodesInCurList = 0;
    average = 0.0f;

    printf("\n\nPRINTING TABLE status:\n\n");

    for (i = 0; i < TABLESIZE; i++) {
        if (table[i] != NULL) {
            usedSlots++;
            nodesInCurList = 0;
            currentElement = table[i];
            while (currentElement != NULL) {
                nodesInCurList++;
                currentElement = currentElement->next;
            }
            totalElements += nodesInCurList;
            if (nodesInCurList > maxLength) {
                maxLength = nodesInCurList;
            }

        }
    }

    if (usedSlots != 0)
        average = (double) totalElements / (double) usedSlots;


    printf("\tTotal elements on the list:\t\t%d\n", totalElements);
    printf("\tUsed slots on the table:\t\t%d\n", usedSlots);
    printf("\tMaximum elements on one slot:\t%d\n", maxLength);
    printf("\tAverage length of lists:\t\t%f\n", average);

}

void printData(hashTable table) {

    int i;
    hashElement *currentElement;
    printf("\n\nPRINTING TABLE DATA:\n\n");

    for (i = 0; i < TABLESIZE; i++) {
        if (table[i] != NULL) {
            currentElement = table[i];
            printf("\tPOSITION %d:\t=>\t[%s]", i, currentElement->key);
            currentElement = currentElement->next;
            while (currentElement != NULL) {
                printf("->[%s]", currentElement->key);
                currentElement = currentElement->next;
            }
            printf("\n");
        }
    }

}