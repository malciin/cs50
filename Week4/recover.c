/**
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint8_t  BYTE;

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover infile.raw\n");
        return 1;
    }

    char *infile = argv[1];
    
    // buffor for reading
    BYTE buffor[512];
    for(int i = 0; i<512; i++)
        buffor[i] = 0x00;
    
    // 
    FILE * inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }
    
    int correctReaded = 0;
    
    // Buffor for sprintf function
    char sprintfBuffor[10];


    FILE * outptr = NULL;
    int imageNumber = 0;
    do{
        correctReaded = fread(&buffor, sizeof(BYTE), 512, inptr);
        
        if (buffor[0] == 0xff && buffor[1] == 0xd8 && buffor[2] == 0xff && (buffor[3] & 0xf0) == 0xe0)
        {
            if (outptr != NULL)
            {
                fclose(outptr);
                imageNumber++;
                outptr = NULL;
            }
            sprintf(&sprintfBuffor[0], "%03i.jpg", imageNumber);
            outptr = fopen(sprintfBuffor, "w");
            if (outptr == NULL)
            {
                printf("Could not create %s\n", &sprintfBuffor[0]);
                return 4;
            }
        }
        
        if(outptr!=NULL)
            fwrite(&buffor, sizeof(BYTE), 512, outptr);
    } while(correctReaded == 512);
    
    fclose(inptr);
    return 0;
}