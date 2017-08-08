/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>

#include "helpers.h"

// Binary search
bool search(int value, int values[], int n)
{
    int aIndx = 0;
    int bIndx = n-1;
    while(true)
    {
        if (values[(aIndx+bIndx)/2] == value)
            return true;
        else if (aIndx == bIndx)
            break;
        else if (values[(aIndx+bIndx)/2] > value)
            bIndx = (aIndx + bIndx)/2 - 1;
        else
            aIndx = (aIndx + bIndx)/2 + 1;
    }
    return false;
}

// Counting Sort
void sort(int values[], int n)
{
    int count[65536];
    for(int i = 0; i<65536; i++)
        count[i] = 0;
    
    // Counting...
    for(int i = 0; i<n; i++)
    {
        count[values[i]]++;
    }

    int i = 0;
    int index = 0;

    // Put values to the array
    while(i<65536)
    {
        if (count[i] > 0)
        {
            values[index] = i;
            index++;
            
            count[i]--;
            if (count[i] > 0)
                continue;
        }
        i++;
    }
    return;
}