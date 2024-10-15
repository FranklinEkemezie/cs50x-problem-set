// Work on bitmap file

#include <getopt.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "filter.h"

int main(int argc, char *argv[])
{

    int option;
    char filter = '\0';

    // Ensure the program is run with correct number of arguments
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./filter -[b|e|g|r|s] infile.bmp outfile.bmp\n");
        return 1;
    }

    // Parse the options using getopt
    while ((option = getopt(argc, argv, "begrs")) != -1)
    {
        switch (option)
        {
            case 'b':
            case 'e':
            case 'g':
            case 'r':
            case 's':
                filter = option;
                break;
            case '?':
                fprintf(stderr, "Unknown option -%c\n", optopt);
                exit(EXIT_FAILURE);
            default:
                fprintf(stderr, "Usage: ./filter -[b|e|g|r|s]");
                exit(EXIT_FAILURE);
        }

        // Ensire that a filter flag was provided
        if (filter == '\0')
        {
            fprintf(stderr, "Please specify any of the filter: -b, -e, -g, -r, -s\n");
            exit(EXIT_FAILURE);
        }

        // Ensure the correct number of arguments were passed
        if (argc != optind + 2)
        {
            fprintf(stderr, "Usage: ./filter -[b|e|g|r|s]\nProvide infile.bmp and outfile.bmp");
            exit(EXIT_FAILURE);
        }

        // Get the input and output file names
        char *infile    = argv[optind];
        char *outfile   = argv[optind + 1];

        // Create image from file
        BMPImage *img = createBMPImage(infile);

        // Apply filter to image
        switch (filter)
        {
            case 'b':
                blur(img);
                break;
            case 'e':
                edges(img);
                break;
            case 'g':
                grayscale(img);
                break;
            case 'r':
                reflect(img);
                break;
            case 's':
                sepia(img);
                break;
            default:
                fprintf(stderr, "Usage: ./filter -[b|e|g|r|s]");
                exit(EXIT_FAILURE);
        }

        // Write the image file with filter applied to the outfile
        writeBMPImage(img, outfile);

        destroyBMPImage(img);
    }
    
}

BMPImage *createBMPImage(const char *filename)
{
    // Open the file
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
        fprintf(stderr, "Could not open file.\n");
        exit(EXIT_FAILURE);
    }

    // Try reading the BMP File Header and BMP File Info
    BMPFileHeader *file_header = malloc(sizeof(BMPFileHeader));
    BMPInfoHeader *info_header = malloc(sizeof(BMPInfoHeader));

    // Read the BMP file header and BMP info header
    fread(file_header, sizeof(BMPFileHeader), 1, file);
    fread(info_header, sizeof(BMPInfoHeader), 1, file);

    // Check if file is a valid BMP file
    if (
        file_header->type != 0x4d42 || file_header->offset != 54 ||
        info_header->header_size != 40 || info_header->bit_count != 24 ||
        info_header->compression != 0
    )
    {
        fprintf(stderr, "Not a valid BMP file!\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    BMPImage *img = malloc(sizeof(BMPImage));
    if (!img)
    {
        fprintf(stderr, "Failed to allocate memory for image.\n");

        // Perform necessary cleanup, and exit
        fclose(file);
        free(file_header);
        free(info_header);

        exit(EXIT_FAILURE);
    }

    // Set the struct fields
    img->file_header = file_header;
    img->info_header = info_header;
    img->pixel_data = NULL;

    // Go on to read the pixel data

    // Move to the pixel data position
    fseek(file, img->file_header->offset, SEEK_SET);

    // The dimensions of image
    int img_width   = get_width(*img),
        img_height  = get_height(*img);

    int bytes_per_row = (img_width * sizeof(RGBPixel) + 3) & (~3);
    int padding = bytes_per_row - (img_width * sizeof(RGBPixel));

    // Allocate memory for rows
    // Each row is an array of pointers pointing to a particular pixel
    img->pixel_data = malloc(img_height * sizeof(RGBPixel*));

    for (int i = 0; i < img_height; i++)
    {
        // Allocate memory for each row
        img->pixel_data[i] = malloc(img_width * sizeof(RGBPixel));

        for (int j = 0; j < img_width; j++)
        {
            // The pixel buffer
            RGBPixel pixel_buffer;

            // Read image pixel by pixel
            size_t read_count = fread(&pixel_buffer, sizeof(RGBPixel), 1, file);
            if (read_count != 1)
            {
                if (feof(file))
                    fprintf(stderr, "Unexpected end of file.\n");
                else if (ferror(file))
                    fprintf(stderr, "An error occurred reading image file.\n");
                else
                    fprintf(stderr, "Something went wrong.\n");

                // Do necessary cleanup, and exit
                fclose(file);
                for (int k = 0; k < i; free(img->pixel_data[k]), k++);
                free(img->pixel_data);
                free(img->info_header);
                free(img->file_header);
                free(img);

                exit(EXIT_FAILURE);
            }

            img->pixel_data[i][j] = pixel_buffer;
        }

        // Skip over the padding bytes
        fseek(file, padding, SEEK_CUR);
    }

    fclose(file);

    return img;
}

void destroyBMPImage(BMPImage *img)
{
    // Free the pointers allocated for each row
    for (
        int i = 0, img_height = get_height(*img);
        i < img_height;
        free(img->pixel_data[i]), i++
    );

    free(img->info_header);
    free(img->file_header);

    // Free the image pointer lastly
    free(img);
}

void writeBMPImage(BMPImage *img, const char *filename)
{
    // Open the file for writing
    FILE *file = fopen(filename, "wb");
    if (file == NULL)
    {
        fprintf(stderr, "Could not open file.\n");
        exit(EXIT_FAILURE);
    }

    // Try writing the BMP File Header and BMP File Info.
    if (
        fwrite(img->file_header, sizeof(BMPFileHeader), 1, file) != 1 ||
        fwrite(img->info_header, sizeof(BMPInfoHeader), 1, file) != 1
    )
    {
        fprintf(stderr, "Could not write into file.\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // Go on to read the image pixel data

    // Move to the pixel data position
    fseek(file, img->file_header->offset, SEEK_SET);

    // The dimensions of image
    int img_width   = get_width(*img),
        img_height  = get_height(*img);

    int bytes_per_row = (img_width * sizeof(RGBPixel) + 3) & (~3);
    int padding = bytes_per_row - (img_width * sizeof(RGBPixel));

    BYTE padding_bytes[3] = {0, 0, 0};
    for (int i = 0; i < img_height; i++)
    {
        for (int j = 0; j < img_width; j++)
        {
            RGBPixel pixel_buffer = img->pixel_data[i][j];
            if (fwrite(&pixel_buffer, sizeof(RGBPixel), 1, file) != 1)
            {
                fprintf(stderr, "Something went wrong reading into the file.\n");

                fclose(file);
                exit(EXIT_FAILURE);
            }
        }

        // Write the padding bytes
        fwrite(padding_bytes, sizeof(BYTE), padding, file);
    }

    fclose(file);
}

void print_bmp_info(BMPImage img)
{
    BMPInfoHeader *info_header = img.info_header;
    BMPFileHeader *file_header = img.file_header;

    printf("BMP Image Information:\n");
    printf("Width: %d pixels\n", info_header->width);
    printf("Height: %d pixels\n", info_header->height);
    printf("Bit Depth: %d bits per pixel\n", info_header->bit_count);
    printf("Bit Size: %u\n", info_header->header_size);
    printf("Compression: %u\n", info_header->compression);
    printf("Image Size: %u bytes\n", info_header->image_size);
    printf("Offset to pixel data: %u bytes\n", file_header->offset);
}

int get_height(BMPImage img)
{
    return abs(img.info_header->height);
}

int get_width(BMPImage img)
{
    return abs(img.info_header->width);
}

void grayscale(BMPImage *img)
{
    // Get the dimensions of the image
    int height = get_height(*img),
        width = get_width(*img);

    // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++ )
        {
            RGBPixel pxl = img->pixel_data[i][j];

            // Take the average of red, green and blue channels
            float avg_channel_value = (
                (int) pxl.red + (int) pxl.green + (int) pxl.blue
            ) / 3.0;

            // Update the pixel values
            img->pixel_data[i][j].red   = 
            img->pixel_data[i][j].green =
            img->pixel_data[i][j].blue  = avg_channel_value;
        }
    }
    return;
}

void sepia(BMPImage *img)
{
    // Get the image dimension
    int height  = get_height(*img),
        width   = get_width(*img);

    // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBPixel pxl = img->pixel_data[i][j];

            // Compute the sepia value
            int sepia_red   = (int) round(.393 * pxl.red + .769 * pxl.green + .189 * pxl.blue),
                sepia_green = (int) round(.349 * pxl.red + .686 * pxl.green + .168 * pxl.blue),
                sepia_blue  = (int) round(.272 * pxl.red + .534 * pxl.green + .131 * pxl.blue);

            // Update pixel with the sepia values, capping the values if they
            // happen to exceed the max. value of 255
            img->pixel_data[i][j].red   = (int) fmin(255, sepia_red);
            img->pixel_data[i][j].green = (int) fmin(255, sepia_green);
            img->pixel_data[i][j].blue  = (int) fmin(255, sepia_blue);
        }
    }
}

void reflect(BMPImage *img)
{
    // Get the dimensions of the image
    int height  = get_height(*img),
        width   = get_width(*img);

    // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        // Grab a copy of the image row to avoid modify it
        // during pixel swapping
        RGBPixel image_row_copy[width];

        // Copy the values of the row into the temporary
        // 'image_row' array
        for (int j = 0; j < width; j++)
            image_row_copy[j] = img->pixel_data[i][j];
        
        // Swap the pixels in that row
        for (int j = 0; j < width; j++)
            img->pixel_data[i][j] = image_row_copy[width - 1 - j];
    }
    return;
}

void blur(BMPImage *img)
{
    // Get the dimension of the image
    int height  = get_height(*img),
        width   = get_width(*img);

    // Create a copy of image pixel data
    RGBPixel img_pxl_data_copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
            img_pxl_data_copy[i][j] = img->pixel_data[i][j];
    }

    // Loop over the pixels
    // Looking for the neighbouring pixels 
    // which form a 3-by-3 matrix around it
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Get the neighbouring pixels in a 3-by-3 matrix
            // Keep track of the sum of the pixel value channels
            int pxl_neighbours_sum_red      = 0,
                pxl_neighbours_sum_green    = 0,
                pxl_neighbours_sum_blue     = 0;
            
            // Keep track of how many neighbour the pixel has
            int pxl_neighbour_count = 0;

            const int n = 3;    // dimension of the 3-by-3 matrix 
            for (int x = 0; x < n; x++)
            {
                for (int y = 0; y < n; y++)
                {
                    int pxl_neighbour_index_x = i + x - 1,  // horizontal position of neighbour
                        pxl_neighbour_index_y = j + y - 1;  // vertical position of neighbour

                    // Check if neighbour can exist there
                    if (
                        pxl_neighbour_index_x < 0 || pxl_neighbour_index_x > height - 1 ||
                        pxl_neighbour_index_y < 0 || pxl_neighbour_index_y > width - 1
                    ) continue;

                    RGBPixel pxl_neighbour = img_pxl_data_copy[pxl_neighbour_index_x][pxl_neighbour_index_y];

                    // Add the pixel RGB channel values of the neighbour to the 'pxl_neighbour_sum'
                    pxl_neighbours_sum_red  += pxl_neighbour.red;
                    pxl_neighbours_sum_green+= pxl_neighbour.green;
                    pxl_neighbours_sum_blue += pxl_neighbour.blue;

                    pxl_neighbour_count++;
                }
            }

            // Compute the average value of the RGB neighbouring matrix pixels
            RGBPixel pxl_neighbours_avg = {
                .red    = (BYTE) round(pxl_neighbours_sum_red    / (float) pxl_neighbour_count),
                .green  = (BYTE) round(pxl_neighbours_sum_green  / (float) pxl_neighbour_count),
                .blue   = (BYTE) round(pxl_neighbours_sum_blue   / (float) pxl_neighbour_count)
            };

            // Modify the current pixel
            img->pixel_data[i][j] = pxl_neighbours_avg;
        }
    }
    return;
}

void edges(BMPImage *img)
{
    // Get the dimensions of the image
    int height  = get_height(*img),
        width   = get_width(*img);

    // Create a copy of the image pixel data
    RGBPixel img_pxl_data_copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
            img_pxl_data_copy[i][j] = img->pixel_data[i][j];
    }

    // Loop over the pixels
    // Looking for the neighbouring pixels
    // that form a 3-by-3 mmatrix around the pixel
    const int n = 3;    // dimension of the neighbouring pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Get the neighbouring pixels of the pixel
            RGBPixel pxl_neighbours[n][n];

            for (int x = 0; x < n; x++)
            {
                for (int y = 0; y < n; y++)
                {
                    int pxl_neighbour_index_x = i + x - 1,  // horizontal position of neighbour
                        pxl_neighbour_index_y = j + y - 1;  // vertical position of neighbour

                    // Check if neighbour can exist there
                    if (
                        pxl_neighbour_index_x < 0 || pxl_neighbour_index_x > height - 1 ||
                        pxl_neighbour_index_y < 0 || pxl_neighbour_index_y > width - 1
                    )
                    {
                        // No neighbour here, treat as black pixel: rgb(0, 0, 0)
                        pxl_neighbours[x][y].red    =
                        pxl_neighbours[x][y].green  =
                        pxl_neighbours[x][y].blue   = 0;
                    }
                    else
                        pxl_neighbours[x][y] = img_pxl_data_copy[pxl_neighbour_index_x][pxl_neighbour_index_y];
                }
            }

            // Detect the edges using Sobel operator.
            // Compute the values for Gx and Gy of each RGB channel

            // Sobel operator kernels
            int Gx_kernel[][3] = {
                {-1,  0,  1},
                {-2,  0,  2},
                {-1,  0,  1}
            };
            int Gy_kernel[][3] = {
                {-1, -2, -1},
                { 0,  0,  0},
                { 1,  2,  1}
            };

            // Compute Gx and Gy for the three channels (RGB respectively)
            int Gx_red = 0, Gx_green = 0, Gx_blue = 0,
                Gy_red = 0, Gy_green = 0, Gy_blue = 0;

            for (int x = 0; x < n; x++)
            {
                for (int y = 0; y < n; y++)
                {
                    int Gx_kernel_value = Gx_kernel[x][y],
                        Gy_kernel_value = Gy_kernel[x][y];

                    RGBPixel pxl_neighbour = pxl_neighbours[x][y];

                    // Update the Gx values
                    Gx_red  += (Gx_kernel_value * pxl_neighbour.red);
                    Gx_green+= (Gx_kernel_value * pxl_neighbour.green);
                    Gx_blue += (Gx_kernel_value * pxl_neighbour.blue);

                    // Update the Gy values
                    Gy_red  += (Gy_kernel_value * pxl_neighbour.red);
                    Gy_green+= (Gy_kernel_value * pxl_neighbour.green);
                    Gy_blue += (Gy_kernel_value * pxl_neighbour.blue);

                }
            }

            // Compute the resultant kernel values
            double  G_resultant_red     = sqrt(pow(Gx_red,  2) + pow(Gy_red,    2)),
                    G_resultant_green   = sqrt(pow(Gx_green,2) + pow(Gx_green,  2)),
                    G_resultant_blue    = sqrt(pow(Gx_blue, 2) + pow(Gx_blue,   2));

            // Modify the pixel
            img->pixel_data[i][j].red   = (BYTE) fmin(G_resultant_red,  255);
            img->pixel_data[i][j].green = (BYTE) fmin(G_resultant_green,255);
            img->pixel_data[i][j].blue  = (BYTE) fmin(G_resultant_blue, 255);
        }
    }
    return;
}