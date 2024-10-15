// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;
const int SAMPLE_SIZE = sizeof(int16_t);

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "rb");
    if (input == NULL)
    {
        printf("Could not open input file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "wb");
    if (output == NULL)
    {
        printf("Could not open output file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // TODO: Copy header from input file to output file
    BYTE header[HEADER_SIZE];
    fread(header, sizeof(BYTE), HEADER_SIZE, input);
    fwrite(header, sizeof(BYTE), HEADER_SIZE, output);

    // TODO: Read samples from input file and write updated data to output file

    // Create a buffer for a single sample
    int16_t sample_buffer;

    // Read single sample from input into buffer while 
    // there are samples left to read
    while (fread(&sample_buffer, SAMPLE_SIZE, 1, input))
    {
        // Modify the sample by multiplying with the factor
        sample_buffer = sample_buffer * factor;

        // Write the updated sample to the output file
        size_t write_count = fwrite(&sample_buffer, SAMPLE_SIZE, 1, output);
        // Check if the write was successsful
        if (write_count != 1)
        {
            printf("Error writing sample\n");
            break;
        }
    }

    // Close files
    fclose(input);
    fclose(output);
}
