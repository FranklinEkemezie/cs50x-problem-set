#include <stdio.h>

#include "../cs50/cs50.h"

int get_pyramid_height();
void print_pyramid(int height);
void print_bricks(int no_of_bricks, int height);

int main(void)
{
    // Prompt user for pyramid's actual height
    int height = get_pyramid_height();

    // Print the pyramid
    print_pyramid(height);
}

int get_pyramid_height()
{
    int height;
    do
    {
        height = get_int("Pyramid's height: ");
    }
    while (height < 1);

    return height;
}

void print_pyramid(int height)
{
    for (int i = 0; i < height; i++)
    {
        // Print row
        print_bricks(i, height);
        printf("\n");
    }
}

// Print the bricks for a row given
// the number of bricks and the length
void print_bricks(int no_of_bricks, int length)
{
    // Print spaces
    int counter, no_of_spaces;
    for (
        counter = 0, no_of_spaces = length - (no_of_bricks + 1);
        counter < no_of_spaces;
        printf(" "), counter++
    );

    // Print the bricks
    for ( ; counter < length; printf("#"), counter++);
}