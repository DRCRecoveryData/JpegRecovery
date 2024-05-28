import sys
import os

def main():
    # Check if a file is input or not
    if len(sys.argv) == 1:
        # Generate error for file name not found in command line
        print("Error: Not entered file name in command line", file=sys.stderr)
        # Returning value which specifies something went wrong
        return 1
    else:
        # Assigning variable for size of jpeg blocks
        size = 512

        # Opening file that is corrupted in read mode
        try:
            input_file = open(sys.argv[-1], "rb")
        except IOError as e:
            # Generating error that file cannot open
            print(f"Error: Can't open raw file - {e}", file=sys.stderr)
            # Returning value which implies some error
            return 2

        # Defining temporary buffer for storing 512 bytes of data of raw file
        temp_buffer = bytearray(size)

        # Assigning variable to check for image found
        image_found = False

        # Assigning variable to count images discovered
        image_count = 0

        # Defining array containing identification of jpeg images to compare with first four bytes of block
        jpeg_headers = [b'\xff\xd8\xff\xe0', b'\xff\xd8\xff\xe1']

        # Assigning file pointer variable for storing image found
        image = None

        # While loop to iterate over whole corrupted file to discover image
        while input_file.readinto(temp_buffer):
            # Check for whether jpeg file is found or not
            if temp_buffer[:4] in jpeg_headers:
                # Check for image is found before or not
                if image_found:
                    # Closing image file found
                    image.close()
                else:
                    # Assigning value for image found
                    image_found = True

                # String to specify image file name
                fname = f"{image_count:04d}.jpeg"

                # Opening blank image file
                image = open(fname, "wb")

                # Increasing count of image
                image_count += 1

            # Check for whether image is found or not
            if image_found:
                # Writing the block of size 512 bytes in image file
                image.write(temp_buffer)

        # Closing last file image found if any
        if image:
            image.close()

        # Closing input file
        input_file.close()

        # Returning value for successful run of program
        return 0

if __name__ == "__main__":
    main()
