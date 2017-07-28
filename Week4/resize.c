/**
 * Resize more comfortable
 */
       
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // bool
#include <math.h>    // fabsf

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize factor infile outfile\n");
        return 1;
    }
    
    float factor = 0.0;
    factor = atof(argv[1]);
    
    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file 
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    BITMAPFILEHEADER bfNew = bf;
    BITMAPINFOHEADER biNew = bi;
    
    // Set new size
    biNew.biWidth = (int)(biNew.biWidth*factor);
    biNew.biHeight = (int)(biNew.biHeight*factor);
    
    // Set padding and set biSizeImage and bfSize
    int originalPadding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int newPadding = (4 - (biNew.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    biNew.biSizeImage = (biNew.biWidth * sizeof(RGBTRIPLE) + newPadding) * abs(biNew.biHeight);
    bfNew.bfSize = biNew.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bfNew, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&biNew, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines
    //printf("Original height: %i\tNew: %i\n", bi.biHeight, biNew.biHeight);
    //printf("Original width: %i\tNew: %i\n", bi.biWidth, biNew.biWidth);
    
    
    
    RGBTRIPLE * row = malloc(sizeof(RGBTRIPLE) * bi.biWidth);
    
    // Load first row from original file
    for(int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        fread(&row[i], sizeof(RGBTRIPLE), 1, inptr);
    }
    // skip over padding, if any
    fseek(inptr, originalPadding, SEEK_CUR);
    
    float yWsp = 0;
    float xWsp = 0;
    
    float yWspDelta = fabsf((float)bi.biHeight/biNew.biHeight);
    float xWspDelta = fabsf((float)bi.biWidth/biNew.biWidth);
    
    float yCounter = 0;
    
    for(int i = 0, biNewHeight = abs(biNew.biHeight); i < biNewHeight; i++)
    {
        // If we are at situation when we exceed original height
        // then we load to memory another row
        
        //printf("if %i == %i\n", (int)(yWsp + (float)bi.biHeight/biNewHeight), (int)yWsp);
        if (i!=0)
        {
        while ((int)(yWsp + yWspDelta) != yCounter)
        {
            for(int z = 0, biHeight = abs(bi.biHeight); z < biHeight; z++)
            {
                fread(&row[z], sizeof(RGBTRIPLE), 1, inptr);
            }
            // skip over padding, if any
            fseek(inptr, originalPadding, SEEK_CUR);
            yCounter++;
        }
        
        yWsp += yWspDelta;
        }
        //printf("yWsp: %f\n", yWsp);
        xWsp = 0;
        
        for (int j = 0; j < biNew.biWidth; j++)
        {
            fwrite(&row[(int)xWsp], sizeof(RGBTRIPLE), 1, outptr);
            xWsp += xWspDelta;
        }
        // Add neccessary padding
        for (int k = 0; k < newPadding; k++)
        {
            fputc(0x00, outptr);
        }
    }
    
    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}

// END OF FILE
////////////////////////////////
////////////////////////////////
////////////////////////////////















// Old unused function that I have wrote because I forget
// about that C have atof function so I wrote one myself
// I left that because I never actually wrote string to float function
// so I do not wanna delete that.

// Usage in main:
/*
    if (validateFloatString(argv[1]))
        factor = stof(argv[1]);
    else
    {
        fprintf(stderr, "Cannot parse factor number!\n");
        return 1;
    }
*/

/*
 *  CONVERSION FUNCTIONS
 *  FROM STRING -> FLOAT
 */

bool validateFloatString(char * string)
{
    bool dot = false;
    bool startedZero = false;
    for(int i = 0; string[i] != '\0'; i++)
    {
        // if char isnt equal: '0' - '9' v '.' v '-'
        if ((string[i] < 0x30 || string[i] > 0x39) && string[i] != 0x2E)
            return false;
        
        if (i==0)
        {
            if (string[i] == 0x30)
                startedZero = true;
        }
        
        if (string[i] == 0x2E)
        {
            // double dot case
            if (dot)
                return false;
            dot = true;
        }
        
        // if string started with '0' next must be dot
        if (i==1 && startedZero && !dot)
            return false;
    }
    return true;
}
// Convert from char to int
int CHtoI(char ch)
{
    if (ch < 0x30 || ch > 0x39)
        return -1;
    return ch - 48;
}

// move floating point in the left. Operate on integers
int tenthPower(int value, int n)
{
    for(int i = 0; i < n; i++)
        value *= 10;
    return value;
}

// move floating point in the right. Operate on float
float tenthPowerNegative(float value, int n)
{
    for(int i = 0; i < n; i++)
        value /= 10;
    return value;
}

// string to float
// Assume string is validated before!!!
float stof(char * string)
{
    int beforeDot = 0;
    int afterDot = 0;
    bool dot = false;
    
    int indexAfterDot = -1;
    
    for(int i = 0; string[i]!='\0'; i++)
    {
        if (string[i] == 0x2E)
        {
            dot = true;
            if (string[i+1]!= '\0')
                indexAfterDot = i+1;
            continue;
        }
        if (!dot)
            beforeDot++;
        else
            afterDot++;
    }
    
    float value = 0.0;
    for(int i = 0; i<beforeDot; i++)
    {
        if (string[i] == 0x30)
            continue;
        value += tenthPower(CHtoI(string[i]), beforeDot - i - 1);
    }
    for(int i = indexAfterDot; i < indexAfterDot + afterDot; i++)
    {
        if (string[i] == 0x30)
            continue;
        value += tenthPowerNegative(CHtoI(string[i]), i - indexAfterDot + 1);
    }
    return value;
}