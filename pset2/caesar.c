#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../cs50/cs50.h"

string encrypt_caesarcipher(string plaintext, int key);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./caesar <key>\n");
        return 1;
    }

    int key = atoi(argv[1]);
    if (key < 0)
    {
        printf("Key must be a non-negative integral value.\n");
        return 1;
    }

    // Get the plain text from user
    string plaintext = get_string("plaintext:  ");

    // Output the corresponding ciphertext
    string ciphertext = encrypt_caesarcipher(plaintext, key);

    printf("ciphertext: %s", ciphertext);
}

string encrypt_caesarcipher(string plaintext, int key)
{
    int str_len = strlen(plaintext);
    char *ciphertext = malloc(str_len * sizeof(char) + 1);

    for (int i = 0; i <= str_len; i++)
    {
        char c = plaintext[i];
        if (isalpha(c))
        {
            int char_index = c - (isupper(c) ? 'A' : 'a');
            int cipher_index = (char_index + key) % 26;

            ciphertext[i] = (isupper(c) ? 'A' : 'a') + cipher_index;
        } else
        {
            ciphertext[i] = c;
        }
    }

    return ciphertext;
}