#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>

int main(void)
{
    int r = 245;

    div_t s = div(245, 6);

    int red = 241, green = 187, blue = 125;
    int s_red = (int) (.393 * red + .769 * green + .189 * blue) % 255;

    printf("Sepia red: %i\n", s_red);

    const int img_height = 5;
    const int img_width = 3;
    int img[][3] = {
        {2, 3, 4},
        {0, 1, 5},
        {4, 3, 2},
        {6, 1, 1},
        {8, 7, 9}
    };

    printf("Original Matrix: \n");
    for (int i = 0; i < img_height; i++)
    {
        for (int j = 0; j < img_width; j++)
            printf("%i, ", img[i][j]);

        printf("\t|\t");

        for (int j = 0; j < img_width; j++)
            printf("%i, ", img[i][img_width - 1 - j]);

        printf("\n");
    }
    printf("\n");


    // Swap individual cells to reflect matrix
    for (int i = 0; i < img_height; i++)
    {
        int img_row[img_width];

        // Copy the values
        for (int j = 0; j < img_width; j++)
            img_row[j] = img[i][j];

        for (int j = 0; j < img_width; j++)
            img[i][j] = img_row[img_width - 1 - j];
    }

    printf("Reflected Matrix:\n");
    for (int i = 0; i < img_height; i++)
    {
        for (int j = 0; j < img_width; j++)
            printf("%i, ", img[i][j]);
        printf("\n");
    }
    printf("\n");

    // Blur the 'img' matrix
    // Make a copy of the img
    int img_copy[img_height][img_width];
    for (int i = 0; i < img_height; i++)
    {
        for (int j = 0; j < img_width; j++)
            img_copy[i][j] = img[i][j];
    }

    // Blur the img
    const int max_neighbour = 8;
    for (int i = 0; i < img_height; i++)
    {
        for (int j = 0; j < img_width; j++)
        {
            int img_pxl = img_copy[i][j];

            int neighbours[max_neighbour];
            int neighbour_count = 0;

            // Is pixel not at the top row?
            if (i > 0)
            {
                // It should have a neighbour just above it
                neighbours[neighbour_count] = img_copy[i - 1][j];
                neighbour_count++;
            }

            // Is pixel not at the bottom row?
            if (i < img_height - 1)
            {
                // It should have a neighbour just below it
                neighbours[neighbour_count] = img_copy[i + 1][j];
                neighbour_count++;
            }

            // Is pixel not at the left edge?
            if (j > 0)
            {
                // It should have a neighbour just to its left
                neighbours[neighbour_count] = img_copy[i][j - 1];
                neighbour_count++;
            }

            // Is pixel not at the right edge?
            if (j < img_width - 1)
            {
                // It should have a neighbour just to its right
                neighbours[neighbour_count] = img_copy[i][j + 1];
                neighbour_count++;
            }

            // Is pixel not in the top row and not at the left edge?
            if (i > 0 && j > 0)
            {
                // It should have a neighbour to its top-left
                neighbours[neighbour_count] = img_copy[i - 1][j - 1];
                neighbour_count++;
            }

            // Is pixel not in the top row and not at the right edge?
            if (i > 0 && j < img_width - 1)
            {
                // It should have a neighbour to its top-right
                neighbours[neighbour_count] = img_copy[i - 1][j + 1];
                neighbour_count++;
            }

            // Is pixel not in the bottom row and not at the left edge?
            if (i < img_height - 1 && j > 0)
            {
                // It should have a neighbour to its bottom-left
                neighbours[neighbour_count] = img_copy[i + 1][j - 1];
                neighbour_count++;
            }

            // Is pixel not in the bottom row and not at the right edge?
            if (i < img_height - 1 && j < img_width - 1)
            {
                // It should have a neighbour to its bottom right
                neighbours[neighbour_count] = img_copy[i + 1][j + 1];
                neighbour_count++;
            }


            // Find the average of the pixel value along with its neighbouring pixel values
            int pixel_sum = img_copy[i][j];
            for (int k = 0; k < neighbour_count; pixel_sum += neighbours[k], k++);

            // Update the average for that pixel
            img[i][j] = (int) round(pixel_sum / (float) (neighbour_count + 1));
        }
    }

    printf("Blurred matrix:\n");
    for (int i = 0; i < img_height; i++)
    {
        for (int j = 0; j < img_width; j++)
            printf("%i, ", img[i][j]);
        printf("\n");
    }
    printf("\n");

    // Making the blurred form of the previous (not blurred) image
    // storing the neighbours as 3 by 3 2D arrays
    for (int i = 0; i < img_height; i++)
    {
        for (int j = 0; j < img_width; j++)
        {
            int img_pxl = img_copy[i][j];
            int neighbours[3][3];   // 3-by-3 neighbours matrix

            for (int x = 0; x < 3; x++)
            {
                for (int y = 0; y < 3; y++)
                {
                    int neighbour_index_x = i + x - 1,  // horizontal position of neighbour
                        neighbour_index_y = j + y - 1;  // vertical position of neighbour
                    
                    // Check if neighbour can exist there
                    if (
                        neighbour_index_x < 0 || neighbour_index_x > img_height - 1 ||
                        neighbour_index_y < 0 || neighbour_index_y > img_width  - 1
                    )
                    {
                        // set that neighbour matrix cell to (int) INFINITY and move on
                        neighbours[x][y] = (int) INFINITY;
                        continue;
                    }
                    neighbours[x][y] = img_copy[i + x - 1][j + y - 1];
                }
            }

            // Print the neighbour of each pixel
            printf("Printing the neighbours of: %i...\n", img_pxl);
            int neighbours_sum = 0,    // keep track of the sum of the neighbours
                neighbours_count = 0;   // keep track of how many neighbours a pixel has given
                                        // that a neighbouring value must have a non-NULL value)
            for (int x = 0; x < 3; x++)
            {
                for (int y = 0; y < 3; y++)
                {
                    int neighbour = neighbours[x][y];
                    if (neighbour != (int) INFINITY)
                    {
                        printf("%i, ", neighbour);
                        neighbours_sum += neighbour;
                        neighbours_count++;
                    }
                    else printf("_, ");
                }
                printf("\n");
            }    

            // Print the sum and average of the neighbours numbers,
            // with the number inclusive.
            printf(
                "Neighbours Sum: %i; Neighbours Average: %.2f\n",
                neighbours_sum, neighbours_sum / (float) neighbours_count
            );
            printf("\n");
        }
    }

    // Check if a file is JPEG
    FILE *file = fopen("images/yard.bmp", "rb");

    // Check if file was opened successfully
    if (file == NULL)
    {
        printf("Could not open file. \n");
        return 1;
    }

    const int n_bytes = 40;

    // Buffer to hold the first n bytes
    unsigned char buffer[n_bytes];

    size_t bytesRead = fread(buffer, sizeof(unsigned char), n_bytes, file);

    // Check if we successfully read 4 bytes
    if (bytesRead != n_bytes)
    {
        printf("Error reading file or file too small.\n");
        fclose(file);
        return 1;
    }

    printf("\nFirst 4 bytes of file: \n");
    for (int i = 0; i < n_bytes; i++)
    {
        printf("%02x\n", buffer[i]);
    }

    // Close file
    fclose(file);

    return 0;


}