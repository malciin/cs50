#include <stdio.h>
#include <stdlib.h>
#include "cs50.h"
#include <ctype.h>
void crypt(char * sentence, char * key);  // return a pointer to crypted string
int main(int argc, char * argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./vigenere k");
        return 1;
    }
    char * key = argv[1];
    for(int i = 0; key[i]!='\0'; i++)
    {
        if (key[i] < 65 || key[i] > 122 || (key[i] > 90 && key[i] < 97))
        {
            printf("Usage: ./vigenere k");
            return 1;
        }
    }
    printf("plaintext: ");
    char * toCrypt = GetString();
    crypt(toCrypt, key);
    printf("ciphertext: %s\n", toCrypt);
    return 0;
}

int isBigLetter(char ch)
{
    if (ch <= 90 && ch >= 65)
        return 1;
    return 0;
}
int isLowerLetter(char ch)
{
    if (ch>= 97 && ch <= 122)
        return 1;
    return 0;
}

void crypt(char * str, char * key)
{
    int i = 0;
    int keyCounter = 0;
    for(; str[i] != '\0'; i++)
    {
        if (isBigLetter(str[i]) == 0 && isLowerLetter(str[i]) == 0)
            continue;

        if (key[keyCounter] == '\0')
            keyCounter = 0;

        int value = tolower(key[keyCounter]) - 97;

        if (isBigLetter(str[i]) == 1 && str[i] + value > 90)
            str[i] = str[i] + value - 26;
        else if (isLowerLetter(str[i]) == 1 && (str[i]+value > 122 || str[i]+value < 97)) 
            str[i] = str[i] + value - 26;
        else
            str[i] += value;

        keyCounter++;
    }
}