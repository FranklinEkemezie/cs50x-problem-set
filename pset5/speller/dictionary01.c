// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Reperesents a node in a hash table
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

// Retursn true if word is in dictionary, else false
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
    char char_buffer;           // Buffer to hold the character read
    char *word_buffer = NULL;   // Buffer to hold pointer to word read

    int char_count = 0;         // No. of characters read for each word

    // Read the file character by character
    while (fread(&char_buffer, sizeof(char), 1, dictionary_file))
    {
        // If we haven't reached end of the line
        if (char_buffer != '\n')
        {
            // If no word is being read at the moment
            if (word_buffer == NULL)
            {
                // Start a new word
                word_buffer = malloc(LENGTH * sizeof(char));
                if (word_buffer == NULL)
                {
                    // Memory allocation failed, free memory 
                    // allocated to previous nodes and words
                    // before now and return

                    return false;
                }
            }

            word_buffer[char_count] = char_buffer;
            char_count++;
        }
        // else, we have reached the end of a line
        else
        {
            // End the current word being read
            word_buffer[char_count] = '\0';
            char_count = 0;

            // Add word to the hash table
            if (! insert_word(word_buffer))
            {
                // Insert word failed, free memory 
                // allocated to previous nodes and words
                // and return

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

// Returns number of words in dictionary id loaded, else 0 if not loaded
unsigned int size(void)
{
    // TOOD
    return word_count;
}

// Unloads dictionary from memory, returning true if successfuly, else false
bool unload(void)
{
    // TODO
    return true;
}

// Inserts a word to the hash table
bool insert_word(char *word)
{
    // Create node for word
    node *word_node = malloc(sizeof(node));
    if (word_node == NULL) return false;

    // Hash word
    int word_hash_value = hash(word);

    // Store the word in the node
    strcpy(word_node->word, word);

    // Insert word into hash table in a specific
    // buccket corresponding to the word hash value

    // Let the word node to be inserted point 
    // to the current first element in the bucket
    word_node->next = table[word_hash_value];

    // Now, (prepend) make the word node the first
    // element in the bucket 'linked list'
    table[word_hash_value] = word_node;

    return true;
}