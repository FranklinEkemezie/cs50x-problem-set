#include <stdbool.h>
#include <stdio.h>

#include "dictionary.h"

#define MAX_WORD_LENGTH 45

int main(int argc, char *argv[])
{
    char *dictionary_filename = argv[1];
    char *word = argv[2];

    if (!load(dictionary_filename))
    {
        printf("Could not load dictionary.\n");
        return 1;
    }

    printf("No. of words loaded: %i\n", size());
    printf("Found %s? %s", word, check(word) ? "Yes :)" : "No :(");
}

// Check a text file for misspelling
int check_text_file(const char *text_filename, bool print_misspelled)
{
    // Open the file
    FILE *text_file = fopen(text_filename, "r");

    // Read the content of the file and check
    // misspellings in words.
    // A word here, is defined as:
    // -> series of alphabetic characters (no numbers)
    // -> not longer than MAX_WORD_LENGTH
    // -> may contain either ' or -

    // Prepare to read the file
    char char_buffer;
    char *word_buffer;

    while (fread())
    {

    }
}