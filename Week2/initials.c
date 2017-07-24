#include <stdio.h>
#include "cs50.h"
#include <ctype.h>
int main(int argc, char * argv[])
{
    char * name = GetString();
    int i = 0;
    int startChar = 1;
    while(name[i]!='\0')
    {
        if ((name[i] <= 90 && name[i] >= 65) || (name[i]>= 97 && name[i] <= 122))
        {
            if (startChar == 1)
            {
                printf("%c", toupper(name[i]));
                startChar = 0;
            }
        }
        else
        {
            startChar = 1;
        }
        i++;
    }
    printf("\n");
}