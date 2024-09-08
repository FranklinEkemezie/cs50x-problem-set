#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../cs50/cs50.h"

string encrypt_substitution(string plaintext, string key);

int main(int argc, string argv[])
{
    // Check for right usage
    if (argc != 2)
    {
        printf("Usage: ./substitution <key>\n");
        return 1;
    }

    // Check if the string contains exactly 26 characters
    string key = argv[1];
    int key_len = strlen(key);
    if (key_len != 26)
    {
        printf("Key must contain 26 characters\n");
        return 1;
    }

    // Check if all the characters are all alphabets and are unique:
    // If they are all, convert to the same (uppercase),
    // if not, terminate the program.
    char *key_ = malloc(key_len * sizeof(char) + 1);  // copy of the key to keep track
    for (int i = 0; i < key_len; i++)
    {
        if (!isalpha(key[i]))
        {
            printf("All characters in key must be alphabetic.\n");
            return 1;
        }

        // Check if they are unique
        if (strstr(key_, key + i) != NULL)
        {
            printf("Characters not unique: %c appears more than once.\n", key[i]);
            return 1;
        }

        // Update the copy of the key to keep track of unique characters
        key_[i] = key[i];
    }

    // Get the plain text from the user
    string plaintext = get_string("plaintext: ");

    // Output the corresponding cipher text 
    printf("ciphertext: %s", encrypt_substitution(plaintext, key));
}

string encrypt_substitution(string plaintext, string key)
{
    // Convert the characters of the key to upper case
    for (
        int i = 0, key_len = strlen(key);
        i < key_len;
        key[i] = toupper(key[i]), i++
    );

    printf("Text: %s\n", "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    printf("Key : %s\n", key);

    // 
    int txt_len = strlen(plaintext);
    char *ciphertext = malloc((txt_len * sizeof(char)) + 1);

    for (int i = 0; i <= txt_len; i++)
    {
        char c = plaintext[i];
        if (isalpha(c))
        {
            // Keep track of the index of the character from 'A'
            int char_ascii_index = toascii(c) - (isupper(c) ? 'A' : 'a');
            ciphertext[i] = isupper(c) ? toupper(key[char_ascii_index]) : tolower(key[char_ascii_index]);
        }
        else
        {
            ciphertext[i] = c;
        }
    }

    return ciphertext;
}