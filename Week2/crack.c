#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Bruteforce attack
// Crack function will recursive generate every word from alphabet with certain length
// First argument is char table with given alphabet
// Second argument is size of alphabet table
// Third argument is world length
// Second argument is hash
void crack(char alphabet[], int size, int length, char * hash);
int main(int argc, char * argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./crack hash\n");
        return 1;
    }
    char alphabet[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                       'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

    // Try crack 1length passwrd, next 2length next... to 4 length
    for(int i = 1; i <= 4; i++)
        crack(alphabet, sizeof(alphabet), i, argv[1]);
    return 0;
}

// Recursive function to generate word
// Third argument is actually generated string and second argument is current index on what 
// actuall recurstion is worked
void recursionCrack(char alphabet[], int size, char * str, int index, char * hash)
{
    for(int i = 0; i<size; i++)
    {
        // set at current index all possibly chars from alphabet
        str[index] = alphabet[i];

        // If we are at the end word then word is currently generated and we perform
        // hash check
        if (str[index+1] == '\0')
        {
            if (strcmp(crypt(str, "50"), hash) == 0 == 1)
                printf("%s\n", str);
        }
        // We call recursionCrack on next char in "str"
        else
        {
            recursionCrack(alphabet, size, str, index+1, hash, cracked);
        }
    }
}

void crack(char alphabet[], int size, int length, char * hash)
{
    char * str = malloc((sizeof(char)+1)*length);
    for(int i = 0; i<length; i++)
        str[i] = 'a';
    str[length] = '\0';

    recursionCrack(alphabet, size, str, 0, hash);
    free(str);
}

