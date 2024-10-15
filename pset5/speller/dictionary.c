// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[26];

// No. of words in the dictionary
int word_count = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO

    // Hash the word to obtain its hash value
    int word_hash_value = hash(word);

    node *curr_node = table[word_hash_value];

    while (curr_node != NULL)
    {
        if (strcasecmp(word, curr_node->word) == 0)
            return true;

        curr_node = curr_node->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    return toupper(word[0]) - 'A';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO

    // Open the dictionary file
    FILE *dictionary_file = fopen(dictionary, "r");
    if (dictionary_file == NULL)
    {
        return false;
    }

    // Read each word in the file

    // Prepare to read file
    char char_buffer;           // Buffer to hold character read
    char *word_buffer = NULL;   // Buffer to hold pointer to word read

    int char_count = 0;         // Characters read for each word

    // Read the file character by character
    while (fread(&char_buffer, sizeof(char), 1, dictionary_file))
    {
        // If we haven't reached end of the line
        if (char_buffer != '\n')
        {
            // Check if a word is being read
            if (word_buffer == NULL)
            {
                // Start a new word
                word_buffer = malloc(LENGTH * sizeof(char));
                if (word_buffer == NULL)
                {
                    // Memory allocation failed, free memory
                    // allocated before and return
                    return false;
                }
            }

            word_buffer[char_count] = char_buffer;
            char_count++;
        }
        // else, we have reached the end of the line
        else
        {
            // End the current word being read
            word_buffer[char_count] = '\0';
            char_count = 0;

            // Add word to the hash table
            if (! insert_word(word_buffer))
            {
                // Insert word failed, free memory
                // allocated and exit.
                for (   // Free memory allocated to the hash table nodes
                    int i = 0; i <= hash(word_buffer); i++
                ) for (
                    node *w = table[i], *w_tmp = NULL; w != NULL;
                    w_tmp = w->next, free(w), w = w_tmp
                );

                return false;
            }

            // Update word count
            word_count++;

            // Reset word buffer
            word_buffer = NULL;
        }
    }
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO

    // Free memory allocated in 'load()'
    for (int i = 0; i < N; i++)
    {
        node *word_node = table[i];

        while(word_node != NULL)
        {
            node *word_node_tmp = word_node->next;

            // Free memory allocated to the 'word' and the 'node' itself
            free(word_node->word);
            free(word_node);

            word_node = word_node_tmp;
        }
    }

    return true;
}

// Helper functions

// Insert a word to the hash table
bool insert_word(char *word)
{
    // Create node for word
    node *word_node = malloc(sizeof(node));
    if (word_node == NULL)
        return false;

    strcpy(word_node->word, word);
    word_node->next = NULL;

    // Hash word
    int word_hash_value = hash(word);

    // Insert word into hash table
    node *word_node_tmp = table[word_hash_value];

    table[word_hash_value] = word_node;
    word_node->next = word_node_tmp;

    return true;
}

void print_hash_table()
{
    for (int i = 0; i < N; i++)
    {
        node * word_node = table[i];

        printf("%02i: ", i);

        while (word_node != NULL)
        {
            printf("%s -> ", word_node->word);

            word_node = word_node->next;
        }

        printf("NULL\n");
    }
}