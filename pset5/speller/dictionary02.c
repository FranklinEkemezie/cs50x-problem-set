// Implement's dictionary functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

#define BUCKET_L1_SIZE 10       // ASCII sum (mod 10)
#define BUCKET_L2_SIZE 26       // First letter index from 0 to 25

// Represents a node storing a word in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// Layer 1 Bucket
// Struct holding nodes for words with same ASCII sum (mode 10)
typedef struct bucket
{
    node *nodes[BUCKET_L1_SIZE];
} bucket;

// Hash table:
// Consists of 26 Layer 1 buckets (based on the index of first letter).
// Each Layer 1 bucket consists of 10 Layer 2 buckets (simply referred
// to as 'buckets', since they hold the nodes directly). Words are grouped
// into either of the 10 Layer 2 buckets based on the ASCII sum of the
// letters of the word in the range 0 to 9 (inclusive).
bucket *table[BUCKET_L2_SIZE];

// Number of words loaded from the dictionary
unsigned int NO_OF_WORDS_LOADED = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO

    // Hash word
    int word_hash = hash(word);

    // Get info from word hash value
    int s = word_hash       % BUCKET_L1_SIZE,
        i = (word_hash - s) / BUCKET_L1_SIZE;

    // Starting from the word hash bucket: Specifically,
    // the first element in the hash bucket 'linked list'
    node *word_node = table[i]->nodes[s];

    while (word_node != NULL)
    {
        if (strcasecmp(word_node->word, word) == 0)
            return true;
        
        // Go to the next node
        word_node = word_node->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function

    // Get the index of the first letter of the word
    int i = toupper(word[0]) - 'A';

    // Get the ASCII sum of the word
    int s = 0;
    for (int i = 0, len = strlen(word); i < len; s += toupper(word[i]), i++);

    return ((i * 10) + (s % BUCKET_L1_SIZE));
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO

    // Initialise buckets for nodes
    if(! initialise_buckets())
        return false;

    // Open the file
    FILE *dictionary_file = fopen(dictionary, "r");
    if (dictionary_file == NULL)
    {
        // Free memory allocated, and return
        unload();
        return false;
    }

    // Prepare to read file
    char char_buffer;           // Buffer to hold the character read
    char *word_buffer = NULL;   // Buffer to hold pointer to word read

    int char_count = 0;         // No. of characters read for each word

    // Read the content of the dictionary file one character at a time.
    while (fread(&char_buffer, sizeof(char), 1, dictionary_file))
    {
        // Check if the character is not a new line
        if (char_buffer != '\n')
        {
            // If no word is being read at the moment.
            if (word_buffer == NULL)
            {
                // Start new word
                word_buffer = malloc((LENGTH + 1) * sizeof(char));
                if (word_buffer == NULL)
                {
                    // Allocating memory for word failed.
                    // Free memory allocated to previous words, and return.
                    unload();

                    NO_OF_WORDS_LOADED = 0;     // no word loaded :(

                    return false;
                }
            }

            // Append the character to the end of the word
            word_buffer[char_count] = char_buffer;
            char_count++;
        }
        // else, we reached end of line
        else
        {
            // End the word
            if (word_buffer != NULL)
            {
                word_buffer[char_count] = '\0';
                char_count = 0;

                // Insert word into hash table
                if (! insert_word(word_buffer))
                {
                    // Inserting word failed, free memory
                    // allocated to previous nodes and word,
                    // and return.
                    unload();

                    NO_OF_WORDS_LOADED = 0;     // no word loaded :(

                    return false;
                }

                NO_OF_WORDS_LOADED++;

                // Reset 'word buffer'
                word_buffer = NULL;
            }
        }
    }

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return NO_OF_WORDS_LOADED;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO

    for (int i = 0; i < BUCKET_L2_SIZE; i++)
    {
        // If bucket pointer is `NULL`, memory allocation failed here,
        // break out of the loop since no memory is allocated hence.
        // See `initialise_buckets()` for more info.
        if (table[i] == NULL) break;

        for (int s = 0; s < BUCKET_L1_SIZE; s++)
        {
            node *node_curr = table[i]->nodes[s];
            while (node_curr != NULL)
            {
                // Let the 'temporary node pointer' keep record
                // of the next node pointer
                node *node_tmp = node_curr->next;

                // Free the current node
                free(node_curr);

                // Move on to the next node
                node_curr = node_tmp;
            }
        }

        free(table[i]);
    }

    return true;
}

// Helper function


// Initialise the pointer for the Layer 2 buckets
// holding the word nodes
bool initialise_buckets(void)
{
    for (int i = 0; i < BUCKET_L2_SIZE; i++)
    {
        // Set bucket pointer to NULL incase it fails,
        // (signals `unload()` to skip freeing it).
        table[i] = NULL;

        // Allocate memory, and let the bucket pointer
        // point to that chunk of memory.
        bucket *b = malloc(sizeof(bucket));
        if (b == NULL)
        {
            // Free memory allocated, and return
            unload();

            return false;
        }
        table[i] = b;

        // Set the initial pointer for each
        // Layer 2 bucket to NULL
        // (to indicate no nodes in the bucket yet)
        for (
            int s = 0; s < BUCKET_L1_SIZE;
            table[i]->nodes[s] = NULL, s++
        );
    }

    return true;
}

// Insert word into hash table
bool insert_word(char *word)
{
    // Hash word
    int word_hash = hash(word);

    // Extract info from word hash value
    int s = word_hash       % BUCKET_L1_SIZE,   // ASCII sum of characters (mod 10)
        i = (word_hash - s) / BUCKET_L1_SIZE;   // index of first letter

    // Allocate memory for a node for the word
    node *word_node = malloc(sizeof(node));
    if (word_node == NULL)
    {
        // Allocating memory for node failed,
        // and return.
        return false;
    }

    // Set initial values for the word node
    strcpy(word_node->word, word);

    // Let the node's next pointer point to the
    // current first element in the hash bucket
    word_node->next = table[i]->nodes[s];

    // Make the node the first element, effectively
    // pushing the other elements, if any, backwards.
    table[i]->nodes[s] = word_node;

    return true;
}

void print_hash_table()
{
    for (int i = 0; i < BUCKET_L2_SIZE; i++)
    {
        for (int s = 0; s < BUCKET_L1_SIZE; s++)
        {
            node *word_node = table[i]->nodes[s];
            while (word_node != NULL)
            {
                printf("%s\n", word_node->word);

                // Move on to the next node
                word_node = word_node->next;
            }
        }
    }
}