#include <stdio.h> /*standard input output header*/
#include <stdlib.h> /*standard library for memmory allocation*/
#include <errno.h> /*system error number */

//main function including command line argument as parameters
int main(int argc, char ** argv)
{
    // check if file is input or not
    if (argc == 1)
    {
        //generating error for file name not found in command line
        perror("Not entered file name in command line");
        //returning value which specifies something went wrong
        return 1;
    }
    else
    {
        //assigning variable for size of jpeg blocks
        int size = 512;

        //opening file that is corrupted in read mode
        FILE* InputFile = fopen(argv[argc - 1], "r");

        //check for file is empty or not
        if (InputFile == NULL)
        {
            //generating error that file cannot open
            perror("Can't open raw file");

            //returning value which implies some error
            return 2;
        }

        //defining temprary buffer for storing 512 bytes of data of raw file
        unsigned char TempBuffer[size];

        //assigning variable to check for image found
        int ImageFound = 0;

        //assigning variable for number of blocks
        int block = 1;

        //assigning variable to count images discovered
        int ImageCount = 0;

        //defining array containing identification of jpeg images to compare with first four bytes of block
        int jpeg[5] = {0xff, 0xd8, 0xff, 0xe0, 0xe1};

        //assigning file pointer variable for storing image found
        FILE* image = NULL;

        //while loop to itterate over whole corrupted file to discover image
        while (fread(TempBuffer, size, block, InputFile))
        {
            //check for whether jpeg file is found or not
            if (TempBuffer[0] == jpeg[0] && TempBuffer[1] == jpeg[1] && TempBuffer[2] == jpeg[2] && (TempBuffer[3] == jpeg[3] || TempBuffer[3] == jpeg[4]))
            {
                //check for image is found before or not
                if (ImageFound)
                {
                    //closing image file found
                    fclose(image);
                }
                else
                {
                    //assigning value for image found
                    ImageFound = 1;
                }

                //character array to specify image file name
                char fname[10];
                sprintf(fname, "%04d.jpeg", ImageCount);

                //opening blank image file
                image = fopen(fname, "a");

                //increasing count of image
                ImageCount += 1;
            }

            //check for whether image is found or not
            if (ImageFound)
            {
                //writing the block of size 512 bytes in image file
                fwrite(&TempBuffer, size, block, image);
            }
        }

        //closing last file image found
        fclose(image);

        //closing input file
        fclose(InputFile);

        //returning value for successful run of program
        return 0;
    }
}