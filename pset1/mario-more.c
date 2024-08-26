#include <stdio.h>

#include "../cs50/cs50.h"

int get_pyramid_height();
void print_adjacent_pyramid(int height);
void print_adjacent_bricks(int no_of_bricks, int length);
void print_bricks_left(int no_of_bricks, int length);
void print_bricks_right(int no_of_bricks, int length);

int main(void)
{
    // Promt user for the pyramid's height
    int height = get_pyramid_height();

    // Print the pyramid
    print_adjacent_pyramid(height);
}

int get_pyramid_height()
{
    int height;
    do
    {
        height = get_int("Pyramid's height: ");
    }
    while (height < 1 || height > 8);
}

void print_adjacent_pyramid(int height)
{
    for (
        int i = 0;
        i < height;
        print_adjacent_bricks(i + 1, height), printf("\n"), i++
    );
}

void print_adjacent_bricks(int no_of_bricks, int length)
{
    // Print the left side of the bricks
    print_bricks_left(no_of_bricks, length);

    // Print the spaces
    printf("  ");

    // Print the right side of the bricks
    print_bricks_right(no_of_bricks, length);
}

void print_bricks_left(int no_of_bricks, int length)
{
    int counter, no_of_spaces;

    // Print the spaces
    for (
        counter = 0, no_of_spaces = length - no_of_bricks;
        counter < no_of_spaces;
        printf(" "), counter++
    );

    // Print the bricks
    for ( ; counter < length; printf("#"), counter++);

}

void print_bricks_right(int no_of_bricks, int length)
{
    for (int i = 0; i < no_of_bricks; printf("#"), i++);
}
