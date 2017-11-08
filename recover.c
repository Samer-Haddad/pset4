#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover forensicImage\n");
        return 1;
    }

    // remember filenames
    char *forensic = argv[1];

    // open forensic file
    FILE *card = fopen(forensic, "r");
    if (card == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", forensic);
        return 2;
    }

    // initialize block: stores 512 bytes from card, count: counts number of JPEGs found, found: checks if already found a JPEG, filename: stores the name of a JPEG
    uint8_t block[512];
    int count = -1;
    bool found = 0;
    char filename[8];

    // creating an empty image to start the loop with, it will be closed when finding a new JPEG
    sprintf(filename, "%03i.jpg", 0);
    FILE *img = fopen(filename, "w");

    do  // until reaching end of card
    {
        // read 512 bytes from card
        fread(&block, 1, 512, card);

        // check if new JPEG was found
         if ((block[0] == 0xff) && (block[1] == 0xd8) && (block[2] == 0xff) && ((block[3] & 0xf0) == 0xe0))
         {
             found = 1;
             count++;
             fclose(img);                               //close previous image
             sprintf(filename, "%03i.jpg", count);
             img = fopen(filename, "w");
             fwrite(&block, 1, 512, img);
         }

         else
         {
        // check if already found a JPEG
             if (found == 1)
                 fwrite(&block, 1, 512, img);
             //else continue;
         }
    }while (!feof(card));

    fclose(img);
    fclose(card);

    return 0;
}