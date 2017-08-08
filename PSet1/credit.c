#include <cs50.h>
#include <stdio.h>
#include <math.h>

// Verify creditcard number
int verify(unsigned long long number);
// Get the length of the number
int length(unsigned long long number);
// Get the n-th digit from a number
int getDigit(unsigned long long number, int digit);

int main()
{
    printf("Number: ");
    long long number = get_long_long();
    if (verify(number) == 1)
    {
        int ID = getDigit(number, length(number));
        switch(ID)
        {
        case 3:
            printf("AMEX\n");
            break;
        case 4:
            printf("VISA\n");
            break;
        case 5:
            printf("MASTERCARD\n");
            break;
        default:
            printf("UNIDENTIFIED\n");
        }
    }
    else
        printf("INVALID\n");
    return 0;
}

int length(unsigned long long number)
{
    if (number == 0)
        return 1;
        
    int len = 0;
    while(number != 0)
    {
        len++;
        number/=10;
    }
    return len;
}


// second argument is the digit from behind
int getDigit(unsigned long long number, int digit)
{
    if (digit <= 0)
        return -1;
    return number/(long long)pow(10, digit-1) - (number/(long long)pow(10, digit-1)/10)*10;
}

int verify(unsigned long long number)
{
    int sum = 0;
    for(int i = 2, len = length(number); i<=len; i+=2)
    {
        int digit = getDigit(number, i) * 2;
        if (digit > 9)
            sum += getDigit(digit, 1) + getDigit(digit, 2);
        else
            sum += digit;
    }
    
    for(int i = 1, len = length(number); i<=len; i+=2)
    {
        sum += getDigit(number, i);
    }
    
    if (sum % 10 == 0)
        return 1;
    return 0;
}