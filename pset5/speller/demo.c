// Implement binary search on a large data

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORDS_IN_DICTIONARY 150000
#define MAX_WORD_LENGTH 45

int load_dictionary(char *dictionary_filename, char *words[]);
int search_word(char *dictionary[], const char *word, const int start_index, const int end_index);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./demo <word>.\n");
        return 1;
    }

    // Get the word to search
    char *word_to_search = argv[1];

    // Store dictionary data in an array
    char *words_in_dictionary[MAX_WORDS_IN_DICTIONARY];
    int dictionary_size = 0;

    // Load the dictionary data into an array
    if (! (dictionary_size = load_dictionary("dictionaries/large", words_in_dictionary)))
    {
        printf("Could not load dictionary.\n");
        return 1;
    }

    // Search for the word in the dictionary
    int word_index = search_word(words_in_dictionary, word_to_search, 0, dictionary_size - 1);
    if (word_index != -1)
        printf("Word found @ index: %i\n", word_index);
    else
        printf("Word not found!\n");
}


// Loads the dictionary data into an array of strings:
// Returns the length of the array
int load_dictionary(char *dictionary_filename, char *words[])
{
    // Open the file
    FILE *dictionary_file = fopen(dictionary_filename, "r");
    if (dictionary_file == NULL)
    {
        printf("Could not open dictionary.\n");
        return false;
    }

    // Read the content of the file

    // Prepare to read the file
    char char_buffer;
    int char_count = 0;

    char *word_buffer = NULL;
    int words_count = 0;

    // Read the file 
    while (fread(&char_buffer, sizeof(char), 1, dictionary_file))
    {
        // Start the first word
        if (word_buffer == NULL)
        {
            word_buffer = malloc(MAX_WORD_LENGTH * sizeof(char));
            if (word_buffer == NULL)
            {
                printf("Could not allocate memory for word.\n");

                // Free memory allocated
                for (
                    int i = 0;
                    i < words_count; 
                    free(words[i]), i++
                );

                return false;
            }
        }

        // If we, havent reached end of a line
        if (char_buffer != '\n')
        {
            word_buffer[char_count] = char_buffer;
            char_count++;
        }

        // We are at the end of the line
        else
        {
            // End the word
            word_buffer[char_count] = '\0';

            // Update the 'words' array
            words[words_count] = word_buffer;

            // Update the 'words_count' variable
            words_count++;

            // Reset the 'char_count' variable
            char_count = 0;

            // Reset the 'word_buffer' string
            word_buffer = NULL;
        }
    }

    fclose(dictionary_file);

    return words_count;
}

// Search the part or whole content of a dictionary for
// a word 
int search_word(char *dictionary[], const char *word, const int start_index, const int end_index)
{
    // Base case
    if (start_index > end_index)
        return -1;

    int mid_index = (start_index + end_index) / 2;

    if (strcmp(word, dictionary[mid_index]) < 0)
        return search_word(dictionary, word, start_index, mid_index - 1);
    else if (strcmp(word, dictionary[mid_index]) > 0)
        return search_word(dictionary, word, mid_index + 1, end_index);
    else
        return mid_index;
}