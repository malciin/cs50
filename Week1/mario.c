#include <cs50.h>
#include <stdio.h>

void mario(int n);
int main()
{
    int value = -1;
    do
    {
        printf("Height: ");
        value = get_int();
    } while(value < 0 || value > 23);
    mario(value);
    return 0;
}

void mario(int n)
{
    for(int i = 1; i<=n; i++)
    {
        for (int spaces = 0; spaces < n - i; spaces++)
            printf(" ");
        for(int ch = 0; ch < i; ch++)
            printf("#");
            
        printf("  ");
        
        for(int ch = 0; ch < i; ch++)
            printf("#");
            
        printf("\n");
    }
}