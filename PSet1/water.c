#include <stdio.h>
#include <cs50.h>

int main()
{
    printf("Minutes: ");
    int minutes = get_int();
    printf("Bottles: %i", minutes*12);
    return 0;
}