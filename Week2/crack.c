#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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

    for(int i = 1; i <= 4; i++)
        crack(alphabet, sizeof(alphabet), i, argv[1]);
    return 0;
}

int compare(char * str, char * hash)
{
    if (strcmp(crypt(str, "50"), hash) == 0)
    {
        return 1;
    }
    return 0;
}

void recursionCrack(char alphabet[], int size, char * str, int index, char * hash)
{
    for(int i = 0; i<size; i++)
    {
        str[index] = alphabet[i];
        if (str[index+1] == '\0')
        {
            if (compare(str, hash) == 1)
            {
                printf("%s\n", str);
            }
        }
        else
        {
            recursionCrack(alphabet, size, str, index+1, hash);
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

