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
#include "dictionary.h"

#define NODES 27    // exactly 26 letters in english alphabet + apostrof char

typedef struct dict
{
    bool isWord;
    struct dict * node[NODES];
} dict;

dict * ROOT;
int WORDS = 0;


 
int hash(char ch)
{
    if (ch == '\'')
        return 26;
    if (ch >= 'A' && ch <= 'Z')
        return ch - 'A';
    if (ch >= 'a' && ch <= 'z')
        return ch - 'a';
    return -1;
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    int i = 0;
    dict * dictionary = ROOT;
    int HASH;
    while(word[i] != '\0')
    {
        HASH = hash(word[i]);
        if (HASH == -1)
            return false;
        if (dictionary->node[HASH] == NULL)
            return false;
        dictionary = dictionary->node[HASH];
        i++;
    }
    return dictionary->isWord;
}


bool addWord(char * wsk)
{
    int i = 0;
    dict * dictionary = ROOT;
    int HASH;
    while(wsk[i] != '\n')
    {
        HASH = hash(wsk[i]);
        if (HASH == -1)
            return false;
        if (dictionary->node[HASH] == NULL)
        {
            dictionary->node[HASH] = malloc(sizeof(dict));
            if (dictionary->node[HASH] == NULL)
                return false;
        }
            
        dictionary = dictionary->node[HASH];
        i++;
    }
    dictionary->isWord = true;
    WORDS++;
    return true;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    ROOT = malloc(sizeof(dict));
    ROOT->isWord = false;
    
    FILE * file;
    char mystring [46];

    file = fopen (dictionary , "r");
    if (file == NULL)
        return false;
    
    while (fgets(mystring , 46 , file) != NULL)
        addWord(mystring);
    
    fclose (file);
    printf("LOADED %i WORDS\n", WORDS);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return WORDS;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
 
void unloadRecursive(dict * node)
{
    for(int i = 0; i<NODES; i++)
        if (node->node[i]!=NULL)
            unloadRecursive(node->node[i]);
    if (node->isWord)
        WORDS--;
    free(node);
    node = NULL;
}
 
bool unload(void)
{
    unloadRecursive(ROOT);
    if (WORDS != 0) // something wrong happend
        return false;
    return false;
}
