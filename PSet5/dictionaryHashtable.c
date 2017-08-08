/**
 * Implements a dictionary's functionality.
 * dictionary.h IS SAME AS PROVIDED BY CS50
 * SO JUST COPY AND PASTE THIS CODE INTO dictionary.c

 * TRIE VERSION
 */

#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "dictionary.h"

#define HashtableBuckets 65000
#define HashSeed 12345

typedef struct linkedList
{
    char * string;
    struct linkedList * n;
} linkedList;

linkedList * dictionary[HashtableBuckets];

int WORDS = 0;

/*
Murmur hash copied from 
https://github.com/jwerle/murmurhash.c/blob/master/murmurhash.c
*/
uint32_t murmurhash(const char * key, uint32_t len, uint32_t seed);

uint32_t hash(const char * key)
{
    return murmurhash(key, strlen(key), HashSeed) % HashtableBuckets;
}

linkedList * add(const char * str, linkedList * list)
{
    linkedList * newNode = malloc(sizeof(linkedList));
    newNode->string = malloc(sizeof(char) * strlen(str) + sizeof(char));
    strcpy(newNode->string, str);
    newNode->n = list;
    list = newNode;
    return list;
}

bool check(const char *wordConst)
{
    char word[strlen(wordConst) + 1];
    int i = 0;
    while(wordConst[i]!='\0')
    {
        word[i] = tolower(wordConst[i]);
        i++;
    }
    word[i] = '\0'
    int HASH = hash(word);

    linkedList * wsk = dictionary[HASH];

    while(wsk!=NULL)
    {
        if (strcmp(wsk->string, word) == 0)
            return true;
        wsk = wsk->n;
    }
    return false;
}


bool addWord(char * word)
{
    int HASH = hash(word);
    dictionary[HASH] = add(word, dictionary[HASH]);
    WORDS++;
    return true;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{    
    FILE * file;
    char mystring [46];

    file = fopen (dictionary , "r");
    if (file == NULL)
        return false;
    
    while (fgets(mystring , 46 , file) != NULL)
    {
        mystring[strlen(mystring)-1] = '\0';    // remove '\n' from string (to proper hash)
        addWord(mystring);
    }
        
    
    fclose (file);
    printf("LOADED %i WORDS\n", WORDS);
    return true;
}

unsigned int size(void)
{
    return WORDS;
}

void unloadLinkedList(linkedList * front)
{
    linkedList * wsk = front;
    while(wsk != NULL)
    {
        linkedList * next = wsk->n;
        free(wsk->string);
        free(wsk);
        WORDS--;
        wsk = next;
    }
}

// Need to tweak!
bool unload(void)
{
    for(int i = 0; i<HashtableBuckets; i++)
        if (dictionary[i] != NULL)
            unloadLinkedList(dictionary[i]);
    if (WORDS != 0) // something wrong happend
        return false;
    return true;
}


uint32_t murmurhash (const char *key, uint32_t len, uint32_t seed) {
    uint32_t c1 = 0xcc9e2d51;
    uint32_t c2 = 0x1b873593;
    uint32_t r1 = 15;
    uint32_t r2 = 13;
    uint32_t m = 5;
    uint32_t n = 0xe6546b64;
    uint32_t h = 0;
    uint32_t k = 0;
    uint8_t *d = (uint8_t *) key;
    const uint32_t *chunks = NULL;
    const uint8_t *tail = NULL;
    int i = 0;
    int l = len / 4;
    h = seed;
    chunks = (const uint32_t *) (d + l * 4);
    tail = (const uint8_t *) (d + l * 4);
    for (i = -l; i != 0; ++i) {
        k = chunks[i];
        k *= c1;
        k = (k << r1) | (k >> (32 - r1));
        k *= c2;
        h ^= k;
        h = (h << r2) | (h >> (32 - r2));
        h = h * m + n;
    }
    k = 0;
    switch (len & 3) {
        case 3: k ^= (tail[2] << 16);
        case 2: k ^= (tail[1] << 8);
        case 1:
        k ^= tail[0];
        k *= c1;
        k = (k << r1) | (k >> (32 - r1));
        k *= c2;
        h ^= k;
    }
    h ^= len;
    h ^= (h >> 16);
    h *= 0x85ebca6b;
    h ^= (h >> 13);
    h *= 0xc2b2ae35;
    h ^= (h >> 16);
    return h;
}