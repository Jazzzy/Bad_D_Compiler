//
// Created by jazzzy on 10/15/16.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bHashTable.h"

//---------------------HASH FUNCTION-------------------------
#define FNV1_32_INIT ((u_int32_t)0x811c9dc5)

u_int32_t fnv_32a_str(char *str, u_int32_t hval) {      //This is the hash function
    unsigned char *s = (unsigned char *) str;           //We get the string as unsigned chars for the algorithm
    while (*s) {
        hval ^= (u_int32_t) *s++;                       //XOR the hash with the current octet (a char is an octet)
        //We multiply by the 32 bit FNV magic prime mod 2^32
        //Explanation of this in: http://isthe.com/chongo/tech/comp/fnv/
        hval += (hval << 1) + (hval << 4) + (hval << 7) + (hval << 8) + (hval << 24);
        //This above is basically a prime number with the necessary number of bits (32 in this case)
    }
    return hval;
}

unsigned int
badHash(char *key) { //This controls the current hash value used by the hash function and makes the call to it
    return (fnv_32a_str(key, FNV1_32_INIT)) % TABLESIZE;
}
//-----------------------------------------------------------





//--------------------TABLE LOGIC----------------------------
/*
 * The key gets copied because we don't want to run the risk of some other part of the code
 * deleting the key data.
 *
 * That said, the data itself is not copied since this could come at a higher cost and we
 * assume that we are not deleting the data we need since this hash table will be used just to
 * locate the data fast.
 *
 * Another NOTE: We are using strcmp to compare lexems, we have tested our own implementation of strcmp but
 * it is not nearly as fast as the original strcmp because compilers do a ton of smart optimizations to functions like this.
 *
 * */


int initHashTable(hashTable **table) {
    *table = (hashTable *) malloc(sizeof(hashTable *));                 //Reserve memory for the pointer to our table
    **table = (hashTable) malloc(
            sizeof(hashElement *) * TABLESIZE);    //Reserve memory for the array of pointers to the elements
    return HASH_SUCCESS;
}


int deleteHastTable(hashTable **table) {

    if (table == NULL || *table == NULL || **table == NULL)
        return HASH_ERROR;                              //If the pointer to the table, the table, or the elements are NULL then we cannot delete.

    int i;
    hashElement *currentElement, *elementToDelete;
    printf("\n\nDELETING TABLE DATA ...\n\n");

    for (i = 0; i < TABLESIZE; i++) {                   //We look in all the elements in the table
        if ((**table)[i] != NULL) {                     //If the element is not null
            elementToDelete = (**table)[i];             //We need to delete it
            currentElement = elementToDelete->next;     //And we save a reference to the next one in the list to delete
            printf("Deleting element in %d: [%s]\n", i, elementToDelete->key);
            if (elementToDelete->data != NULL)
                free(elementToDelete->data);            //We have chosen to delete the elements data when we delete the element itself if it's not set to NULL
            free(elementToDelete->key);
            free(elementToDelete);
            while (currentElement !=
                   NULL) {            //We iterate through all the elements in the list for that position to delete them all
                elementToDelete = currentElement;
                currentElement = elementToDelete->next;
                printf("Deleting element in %d: [%s]\n", i, elementToDelete->key);
                if (elementToDelete->data != NULL)
                    free(elementToDelete->data);
                free(elementToDelete->key);
                free(elementToDelete);
            }
        }
    }
    free(**table);                                      //We free the array of pointers to the elements
    free(*table);                                       //And the pointer to the table
    printf("\nDONE DELETING\n\n");

    return HASH_SUCCESS;
}

hashElement *addElement(hashTable *table, char *key, void *data) {

    if (table == NULL || *table == NULL)
        return NULL;                                            //If the the table, or its elements are NULL then we cannot insert anything.


    hashElement *newElement, *oldElement;
    newElement = (hashElement *) malloc(sizeof(hashElement));   //Locate memory for the new element to be added
    int i;

    int length = (int)strlen(key) + 1;

    newElement->key = (char *) malloc(length*sizeof(char));

    for (i = 0; i < length; i++) {                         //We copy the key into our struct
        newElement->key[i] = key[i];
    }

    newElement->data = data;                                    //We assign the data of the element into the struct


    unsigned int hashNumber = badHash(
            key);                     //We get the position in the table with the hash function

    if ((*table)[hashNumber] == NULL) {                         //If there is no element in the position
        (*table)[hashNumber] = newElement;                      //We put the element there
        newElement->next = NULL;                                //And set the next element to null
    } else {                                                    //If there is something in there
        //We insert at the beginning of the list
        oldElement = (*table)[hashNumber];
        (*table)[hashNumber] = newElement;                      //The first element is now the new element
        newElement->next = oldElement;                          //And the next in the list is the old first element


    }
    return newElement;

}

hashElement *getElement(hashTable *table, char *key) {

    if (table == NULL || *table == NULL)
        return NULL;                                        //If the the table, or its elements are NULL then we cannot get any element

    hashElement *currentElement;
    unsigned int hashNumber = badHash(key);                 //We get the position of the element

    if ((*table)[hashNumber] == NULL) return NULL;          //If there is no element then we return NULL

    if (strcmp((*table)[hashNumber]->key, key) == 0) {      //If there is an element and the key is the same
        return (*table)[hashNumber];                        //We return the element
    } else {                                                //If the key is not the same we have to look for it
        currentElement = (*table)[hashNumber];
        while (currentElement->next != NULL) {              //While there is elements in the list
            currentElement = currentElement->next;
            if (strcmp(currentElement->key, key) == 0) {    //We search for out key
                return currentElement;                      //If we find it we return the element
            }
        }
        return NULL;                                        //If not we return NULL
    }
}

int deleteElement(hashTable *table, char *key) {

    if (table == NULL || *table == NULL)
        return HASH_SUCCESS;                                        //If the the table, or its elements are NULL then we cannot delete any element

    hashElement *currentElement;
    unsigned int hashNumber = badHash(key);

    if (table[hashNumber] == NULL) return HASH_ERROR;       //We cannot detete an element that is not in the table

    if (strcmp((*table)[hashNumber]->key, key) == 0) {      //If we find the element in the first position
        currentElement = (*table)[hashNumber];
        (*table)[hashNumber] = currentElement->next;        //Its child will be now in the first position
        free(currentElement);                               //And we delete the element
        return HASH_SUCCESS;
    } else {                                                //If the element seems to be in the list
        hashElement *parentElement = (*table)[hashNumber];     //The parent element is the first one in the list
        currentElement = parentElement->next;               //And the current element is its child
        while (currentElement != NULL) {                    //While we are not in the last element
            if (strcmp(currentElement->key, key) == 0) {    //If we have found the element
                parentElement->next = currentElement->next; //The parent's child will now be the son's child
                if (currentElement->data != NULL)
                    free(currentElement->data);             //We have chosen to delete the elements data when we delete the element itself if it's not set to NULL
                free(currentElement->key);
                free(currentElement);                       //And we delete the element
                return HASH_SUCCESS;
            }                                               //if we have not found the element in this iteration
            parentElement = currentElement;                 //The parent now is the child of the old parent (current)
            currentElement = currentElement->next;          //The new current element is the son of the current element
        }
        return HASH_ERROR;                  //If we can't find the element at all in the list then we cant delete it
    }
}

/*
 * This is just a debug function to show the status of the table
 *
 * We show data like the max length of a list, the total elements inserted, the positions with something in them
 * and the average size of the lists in the table.
 *
 * */
void printState(hashTable table) {

    if (table == NULL)
        return;                                        //If the the table is NULL then we cannot show the status it.

    int maxLength, totalElements, usedSlots, nodesInCurList, i;
    double average;
    hashElement *currentElement;

    maxLength = totalElements = usedSlots = nodesInCurList = 0;
    average = 0.0f;

    printf("\n\nPRINTING TABLE STATUS:\n\n");

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


/*
 * This function is also a debug function that we use for printing all the data in the table
 *
 * We iterate through all the positions and if is not NULL we print the key of the first element and all
 * the possible elements in the linked list.
 *
 * */
void printData(hashTable table) {

    if (table == NULL)
        return;                                        //If the the table is NULL then we cannot print it.

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