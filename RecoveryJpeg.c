#include <stdio.h>  /* Standard input output header */
#include <stdlib.h> /* Standard library for memory allocation */
#include <errno.h>  /* System error number */

// Main function including command line argument as parameters
int main(int argc, char **argv)
{
    // Check if file is input or not
    if (argc == 1)
    {
        // Generating error for file name not found in command line
        perror("Not entered file name in command line");
        // Returning value which specifies something went wrong
        return 1;
    }

    // Assigning variable for size of JPEG blocks
    int size = 512;

    // Opening file that is corrupted in read mode
    FILE *inputFile = fopen(argv[argc - 1], "r");

    // Check if file is empty or cannot be opened
    if (inputFile == NULL)
    {
        // Generating error that file cannot open
        perror("Can't open raw file");

        // Returning value which implies some error
        return 2;
    }

    // Defining temporary buffer for storing 512 bytes of data of raw file
    unsigned char tempBuffer[size];

    // Assigning variable to check for image found
    int imageFound = 0;

    // Assigning variable for number of blocks
    int block = 1;

    // Assigning variable to count images discovered
    int imageCount = 0;

    // Defining array containing identification of JPEG images to compare with first four bytes of block
    int jpegStart[4] = {0xff, 0xd8, 0xff};
    int jpegMarker1 = 0xe0;
    int jpegMarker2 = 0xe1;

    // Assigning file pointer variable for storing image found
    FILE *image = NULL;

    // While loop to iterate over whole corrupted file to discover image
    while (fread(tempBuffer, size, block, inputFile))
    {
        // Check for whether JPEG file is found or not
        if (tempBuffer[0] == jpegStart[0] &&
            tempBuffer[1] == jpegStart[1] &&
            tempBuffer[2] == jpegStart[2] &&
            (tempBuffer[3] == jpegMarker1 || tempBuffer[3] == jpegMarker2))
        {
            // Check if an image was previously found
            if (imageFound)
            {
                // Closing previous image file found
                fclose(image);
            }
            else
            {
                // Assigning value indicating image found
                imageFound = 1;
            }

            // Character array to specify image file name
            char fname[10];
            sprintf(fname, "%04d.jpeg", imageCount);

            // Opening a new blank image file
            image = fopen(fname, "wb");

            // Increasing count of image
            imageCount += 1;
        }

        // Check if an image has been found
        if (imageFound)
        {
            // Writing the block of size 512 bytes in image file
            fwrite(tempBuffer, size, block, image);
        }
    }

    // Closing last image file found, if any
    if (image)
    {
        fclose(image);
    }

    // Closing input file
    fclose(inputFile);

    // Returning value for successful run of program
    return 0;
}
