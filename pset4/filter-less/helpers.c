#include "bmp.h"
#include <math.h>


// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE pxl = image[i][j];

            // Take average of red, green, and blue
            float avg_pxl_value = (
                pxl.rgbtRed + pxl.rgbtBlue + pxl.rgbtGreen
            ) / 3.0;

            // Update pixel values
            image[i][j].rgbtRed = image[i][j].rgbtBlue = 
                image[i][j].rgbtGreen = (int) avg_pxl_value;
        }
    }

    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE pxl = image[i][j];

            // Compute the sepia value
            int sepia_red   = (int) round(.393 * pxl.rgbtRed + .769 * pxl.rgbtGreen + .189 * pxl.rgbtBlue),
                sepia_green = (int) round(.349 * pxl.rgbtRed + .686 * pxl.rgbtGreen + .168 * pxl.rgbtBlue),
                sepia_blue  = (int) round(.272 * pxl.rgbtRed + .534 * pxl.rgbtGreen + .131 * pxl.rgbtBlue);

            // Update pixel with sepia values, capping the values if they
            // happen to exceed the max. value of 255
            image[i][j].rgbtRed     = (int) fmin(255, sepia_red);
            image[i][j].rgbtGreen   = (int) fmin(255, sepia_green);
            image[i][j].rgbtBlue    = (int) fmin(255, sepia_blue);
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        RGBTRIPLE image_row_copy[width];

        // Copy the values of the row into the 
        // temporary 'image_row' array
        for (int j = 0; j < width; j++)
            image_row_copy[j] = image[i][j];

        // Swap the pixels in that row
        for (int j = 0; j < width; j++)
            image[i][j] = image_row_copy[width - 1 - j];
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a copy of image
    RGBTRIPLE image_copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
            image_copy[i][j] = image[i][j];
    }

    // Loop over the pixels
    // Looking for the neighbouring pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Get the neighbouring pixels in a 3-by-3 matrix
            // Keep track of the pixels sum
            int pxl_neighbours_sum_red  = 0,
                pxl_neighbours_sum_green= 0,
                pxl_neighbours_sum_blue = 0;

            // Keep track of how many neighbour the pixel has
            int pxl_neighbour_count = 0;

            const int n = 3;    // dimension of the 3-by-3 matrix
            for (int x = 0; x < n; x++)
            {
                for (int y = 0; y < n; y++)
                {
                    int pxl_neighbour_index_x = i + x - 1,  // horizontal position of neighbour
                        pxl_neighbour_index_y = j + y - 1;  // veritcal position of neighbour

                    // Check if neighbour can exist there
                    if (
                        pxl_neighbour_index_x < 0 || pxl_neighbour_index_x > height - 1 ||
                        pxl_neighbour_index_y < 0 || pxl_neighbour_index_y > width - 1
                    ) continue;

                    RGBTRIPLE pxl_neighbour = image_copy[pxl_neighbour_index_x][pxl_neighbour_index_y];

                    // Add the pixel RGB values of the neighbour to the 'pxl_neighbour_sum'
                    pxl_neighbours_sum_red   += pxl_neighbour.rgbtRed;
                    pxl_neighbours_sum_green += pxl_neighbour.rgbtGreen;
                    pxl_neighbours_sum_blue  += pxl_neighbour.rgbtBlue;

                    pxl_neighbour_count++;
                }
            }

            // Compute the average value of the RGB neighbouring pixels
            RGBTRIPLE pxl_neighbours_avg = {
                .rgbtRed    = round(pxl_neighbours_sum_red    / (float) pxl_neighbour_count),
                .rgbtGreen  = round(pxl_neighbours_sum_green  / (float) pxl_neighbour_count),
                .rgbtBlue   = round(pxl_neighbours_sum_blue   / (float) pxl_neighbour_count)
            };

            // Modify the current pixel
            image[i][j] = pxl_neighbours_avg;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a copy of image
    RGBTRIPLE image_copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
            image_copy[i][j] = image[i][j];
    }

    // Loop over the pixels
    // Looking for the neighbouring pixels
    // that form a 3 by 3 matrix around the pixel
    const int n = 3;    // the dimension of the neighbouring pixel matrix
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Get the neighbouring pixels
            RGBTRIPLE pxl_neighbours[n][n];        
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
                        // No neighbour here, treat as black rgb(0, 0, 0) pixel
                        pxl_neighbours[x][y].rgbtRed   =
                        pxl_neighbours[x][y].rgbtGreen =
                        pxl_neighbours[x][y].rgbtBlue  = 0;
                    }
                    else
                        pxl_neighbours[x][y] = image_copy[pxl_neighbour_index_x][pxl_neighbour_index_y];
                }
            }

            // Sobel operator kernels
            int Gx_kernel[3][3] = {
                {-1,  0,  1},
                {-2,  0,  2},
                {-1,  0,  1}
            };
            int Gy_kernel[3][3] = {
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

                    RGBTRIPLE pxl_neighbour = pxl_neighbours[x][y];

                    // Udpate the Gx values
                    Gx_red  += Gx_kernel_value * pxl_neighbour.rgbtRed;
                    Gx_green+= Gx_kernel_value * pxl_neighbour.rgbtGreen;
                    Gx_blue += Gx_kernel_value * pxl_neighbour.rgbtBlue;

                    // Update the Gy values
                    Gy_red  += Gy_kernel_value * pxl_neighbour.rgbtRed;
                    Gy_green+= Gy_kernel_value * pxl_neighbour.rgbtGreen;
                    Gy_blue += Gy_kernel_value * pxl_neighbour.rgbtBlue;
                }
            }

            // Compute the resultant kernel values
            double  G_resultant_red     = sqrt(pow(Gx_red   , 2) + pow(Gy_red   , 2)),
                    G_resultant_green   = sqrt(pow(Gx_green , 2) + pow(Gx_green , 2)),
                    G_resultant_blue    = sqrt(pow(Gx_blue  , 2) + pow(Gx_blue  , 2));

            // Modify the pixel
            image[i][j].rgbtRed     = (int) fmin(G_resultant_red    , 255);
            image[i][j].rgbtGreen   = (int) fmin(G_resultant_green  , 255);
            image[i][j].rgbtBlue    = (int) fmin(G_resultant_blue   , 255);
        }
    }
    return;
}