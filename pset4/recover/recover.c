#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef uint8_t BYTE;

const int block_size = 512;
const char *recovered_jpgs_dir = "recovered_jpgs/";
const char *jpg_extension = ".jpg";
int jpg_filename_size = 5;

int main(int argc, char *argv[])
{
    // Accept single command line argument
    if (argc != 2)
    {
        printf("Usage: ./recover <forensic_image.raw>\n");
        return 1;
    }

    // Open the memory card
    FILE *card = fopen(argv[1], "rb");
    if (card == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    // While there is still data to be read
    // from the memory card
    BYTE block_buffer[512];

    // Keep track of the JPEG being read
    int jpeg_count = 0;
    bool is_jpg_being_read = false;
    FILE *jpeg = NULL;

    const int jpeg_filename_size = (
        (sizeof(char) * strlen(recovered_jpgs_dir)) +   // 'recovered_jpgs/'
        (sizeof(int) * 3) +                             // '###'
        (sizeof(char) * strlen(jpg_extension)) +        // '.jpg'
        (sizeof(char) * 1)                              // '\0' - nul character
    );
    size_t block_read;
    
    // While there is still data to be read from the memory card
    while (block_read = fread(block_buffer, sizeof(BYTE), block_size, card))
    {

        // Check if incoming block is a JPEG
        if (
            block_buffer[0] == 0xff && block_buffer[1] == 0xd8 &&
            block_buffer[2] == 0xff && (block_buffer[3] & 0xf0) == 0xe0
        )
        {
            // Check if a JPEG is currently being read
            if (is_jpg_being_read)
            {
                // Close the JPEG being read as we have reached the end
                fclose(jpeg);

                // Turn flag indicator false
                is_jpg_being_read = false;
            }

            // Open up a new JPEG file
            char jpeg_filename[jpeg_filename_size];   // "recovered_jpgs/###.jpg"
            sprintf(jpeg_filename, "recovered_jpgs/%03i.jpg", jpeg_count);
            
            // Open the JPEG file for writing into it
            jpeg = fopen(jpeg_filename, "wb");
            if (jpeg == NULL)
            {
                printf("Something went wrong writing JPEG\n");
                return 1;
            }

            // Write into the jpeg
            fwrite(block_buffer, sizeof(BYTE), block_size, jpeg);

            // Turn flag indicator on
            is_jpg_being_read = true; 
            jpeg_count++;       
        }
        else
        {
            // Incoming block is not a jpeg
            
            // Check if a JPEG is being read at the moment
            if (is_jpg_being_read)
            {
                // Continue reading and writing it to the jpeg file
                fwrite(block_buffer, sizeof(BYTE), block_size, jpeg);

                is_jpg_being_read = true;
            }
        }
    }
}