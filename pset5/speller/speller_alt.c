// Implements a spell-checker (Alternative form to speller.c)

#include <ctype.h>
#include <stdio.h>
#include <windows.h>

#include "dictionary.h"

// Default dictionary
#define DICTIONARY "dictionaries/large"

// Prototype
double calculate(FILETIME *start, FILETIME* end);

int main(int argc, char *argv[])
{
    // Check for correct number of args
    if (argc != 2 && argc != 3)
    {
        printf("Usage: ./speller [DICTIONARY] text\n]");
        return 1;
    }

    // Structures for timing data
    FILETIME createTime, exitTime, kernelTimeBefore, userTimeBefore;
    FILETIME kernelTimeAfter, userTimeAfter;

    // Benchmarks
    double time_load = 0.0, time_check = 0.0, time_size = 0.0, time_unload = 0.0;

    // Determine dictionary to use
    char *dictionary = (argc == 3) ? argv[1] : DICTIONARY;

    // Load dictionary
    GetProcessTimes(GetCurrentProcess(), &createTime, &exitTime, &kernelTimeBefore, &userTimeBefore);
    bool loaded = load(dictionary);
    GetProcessTimes(GetCurrentProcess(), &createTime, &exitTime, &kernelTimeAfter, &userTimeAfter);

    // Exit if dictioary not loaded
    if (!loaded)
    {
        printf("Could not load %s. \n", dictionary);
        return 1;
    }

    // Calculate the time to load dictionary
    time_load = calculate(&userTimeBefore, &userTimeAfter);

    // Try to open text
    char *text = (argc == 3) ? argv[2] : argv[1];
    FILE *file = fopen(text, "r");
    if (file == NULL)
    {
        printf("Could not open %s.\n", text);
        unload();
        return 1;
    }

    // Prepare to report misspellings
    printf("\nMISSPELLED WORDS\n\n");

    // Prepare to spell-check
    int index = 0, misspellings = 0, words = 0;
    char word[LENGTH + 1];

    // Spell-check each word in text
    char c;
    while (fread(&c, sizeof(char), 1, file))
    {
        // Allow only alphabetical characters and apostrophes
        if (isalpha(c) || c == '\'' && index > 0)
        {
            // Append character to word
            word[index] = c;
            index++;

            // Ignore alphabetical strings too long to be words
            if (index > LENGTH)
            {
                // Consume remainder of alphabetical string
                while (fread(&c, sizeof(char), 1, file) && isalpha(c));

                // Prepare for new word
                index = 0;
            }
        }

        // Ignore words with numbers (like MS Word can)
        else if (isdigit(c))
        {
            // Consume remainder of alphanumeric string
            while (fread(&c, sizeof(char), 1, file )&& isalnum(c));

            // Prepare for new word
            index = 0;
        }

        // We must have found a whole word
        else if (index > 0)
        {
            // Terminate current word
            word[index] = '\0';

            // Update counter
            words++;

            // Check word's spelling
            GetProcessTimes(GetCurrentProcess(), &createTime, &exitTime, &kernelTimeBefore, &userTimeBefore);
            bool misspelled = !check(word);
            GetProcessTimes(GetCurrentProcess(), &createTime, &exitTime, &kernelTimeAfter, &userTimeAfter);

            // Update benchmark
            time_check += calculate(&userTimeBefore, &userTimeAfter);

            // Print word if mispelled
            if (misspelled)
            {
                printf("%s\n", word);
                misspellings++;
            }

            // Prepare for next word
            index = 0;
        }
    }

    // Check whether there was an error
    if (ferror(file))
    {
        fclose(file);
        printf("Error reading %s.\n", text);
        unload();
        return 1;
    }

    // Close text
    fclose(file);

    // Determine dictionary's size
    GetProcessTimes(GetCurrentProcess(), &createTime, &exitTime, &kernelTimeBefore, &userTimeBefore);
    unsigned int n = size();
    GetProcessTimes(GetCurrentProcess(), &createTime, &exitTime, &kernelTimeAfter, &userTimeAfter);

    // Calculate time to deterime dictionary's size
    time_size = calculate(&userTimeBefore, &userTimeAfter);

    // Unload dictionary
    GetProcessTimes(GetCurrentProcess(), &createTime, &exitTime, &kernelTimeBefore, &userTimeBefore);
    bool unloaded = unload();
    GetProcessTimes(GetCurrentProcess(), &createTime, &exitTime, &kernelTimeAfter, &userTimeAfter);

    // Abort if dictionary not unloaded
    if (!unloaded)
    {
        printf("Could not unload %s. \n", dictionary);
        return 1;
    }

    // Calculate time to unload dictionary
    time_unload = calculate(&userTimeBefore, &userTimeAfter);

    // Report benchmarks
    printf("\nWORDS MISSPELLED:     %d\n", misspellings);
    printf("WORDS IN DICTIONARY:  %d\n", n);
    printf("WORDS IN TEXT:        %d\n", words);
    printf("TIME IN load:         %.2f\n", time_load);
    printf("TIME IN check:        %.2f\n", time_check);
    printf("TIME IN size:         %.2f\n", time_size);
    printf("TIME IN unload:       %.2f\n", time_unload);
    printf("TIME IN TOTAL:        %.2f\n\n",
        time_load + time_check + time_size + time_unload);

    // Success
    return 0;
}

// Returns number of seconds between b and a
double calculate(FILETIME* start, FILETIME* end)
{
    ULARGE_INTEGER startTime, endTime;
    startTime.LowPart = start->dwHighDateTime;
    startTime.HighPart = start->dwHighDateTime;

    endTime.LowPart = end->dwLowDateTime;
    endTime.HighPart = end->dwHighDateTime;

    // Return time difference in seconds
    return (double)(endTime.QuadPart - startTime.QuadPart) / 10000000.0;
}