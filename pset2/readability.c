#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "../cs50/cs50.h"

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
float compute_coleman_liau_index(string text);
void print_grade(float cl_index);

int main(void)
{
    string texts[] = {
        "One fish. Two fish. Red fish. Blue fish.",
        "Would you like them here or there? I would not like them here or there. I would not like them anywhere",
        "Congratulations! Today is your day. You're off to Great Places! You're off and away!",
        "Harry Potter was a highly unusual boy in many ways. For one thing, he hated the summer holidays more than any other time of year. For another, he really wanted to do his homework, but was forced to do it in secret, in the dead of the night. And also he happened to be a wizard.",
        "In my younger and more vulnerable years my father gave me some advice that I've been turning over in my mind ever since.",
        "Alice was beginning to get very tired of sitting by her sister on the bank, and of having nothing to do: once or twice she had peeped into the book her sister was reading, but it had no pictures or conversations in it, \"and what is the use of a book,\" thought Alice \"without pictures or conversation?\"", // Grade 8 not 9
        "When he was nearly thirteen, my brother Jem got his arm badly broken at the elbow. When it healed, and Jem's fears of never being able to play football were assuaged, he was seldom self-conscious about his injury. His left arm was somewhat shorter than his right; when he stood or walked, the back of his hand was at right angles to his body, his thumb parallel to his thigh",
        "There are more things in Heaven and Earth, Horatio, than are dreamt of in your philosophy",
        "It was a bright cold day in April, and the clocks were striking thirteen. Winston Smith, his chin nuzzled into his breast in an effort to escape the vile wind, slipped quickly through the glass doors of Victory Mansions, though not quickly enough  to prevent a swirl of gritty dust from entering along with him.",
        "A large class of computational problems involve the determination of properties of graphs, diagraphs, integers, arrays of integers, finite families of finite sets, boolean formulas and elements of other countable domains."
    };

    // Prompt the user for some text
    string text = get_string("Text: ");

    // Count the number of letters, words, and sentences in the text

    // Compute the Coleman-Liau index
    float cli_index = compute_coleman_liau_index(text);

    // Print the grade level
    print_grade(cli_index);

    // for (int i = 0; i < 10; i++)
    // {
    //     // Get the text
    //     printf("Text: %s", texts[i]);

    //     // Analyse the text: Count the number of letters, words and sentences in the text
    //     printf("Letter count: %i\n", count_letters(texts[i]));
    //     printf("Word count: %i\n", count_sentences(texts[i]));
    //     printf("Sentence count: %i\n", count_sentences(texts[i]));

    //     // Compute the Coleman-Liau index
    //     float cl_index = compute_coleman_liau_index(texts[i]);
    //     printf("Coleman-Liau index: %0.2f\n", cl_index);

    //     // Print the grade level
    //     printf("Grade level: ");
    //     print_grade(cl_index);

    //     printf("\n------------------------------------------------------------------\n\n");
    // }

}

// Count the numbers of letters in a text
int count_letters(string text)
{
    int letter_count = 0;
    for (int i = 0, len = strlen(text); i < len; i++)
        if (isalpha(text[i])) letter_count++;

    return letter_count;
}

// Count the number of words in a text
int count_words(string text)
{
    int word_count = 0;

    // Flags
    bool    word_start,             // indicates the beginning of a word
            curr_char_is_letter,
            prev_char_is_space;

    // Set flags to false
    word_start = curr_char_is_letter = prev_char_is_space = false;

    char prev_char = '\0';          // previous character - set to NUL character initially

    for (int i = 0, len = strlen(text); i < len; i++)
    {
        curr_char_is_letter = isalpha(text[i]);
        word_start = 
            (prev_char == '\0' && curr_char_is_letter) ||       // for the first word
            (curr_char_is_letter && prev_char_is_space);

        if (word_start) word_count++;

        prev_char = text[i];
        prev_char_is_space = isspace(text[i]);
    }

    return word_count;
}

// Count the number of sentences in a text
int count_sentences(string text)
{
    int sentence_count = 0;
    char prev_char = '\0';

    // Flags
    bool    sentence_start,             // indicates the beginning of a sentence
            prev_char_ends_sentence,    // indicates if the previous character can be used to end a sentence
            char_is_a_letter;

    // Set the flags to false
    sentence_start = prev_char_ends_sentence = char_is_a_letter = false;

    for (int i = 0, len = strlen(text); i < len; i++)
    {
        char_is_a_letter = text[i] >= 'A' && text[i] <= 'z';
        prev_char_ends_sentence = prev_char == '.' || prev_char == '?' || prev_char == '!';
        sentence_start = 
            (prev_char == '\0' && char_is_a_letter) ||   // first sentence
            (prev_char_ends_sentence && (isspace(text[i])));  

        if (sentence_start) sentence_count++;

        prev_char = text[i];
    }


    return sentence_count;
}

// Computes the coleman liau index of a string
float compute_coleman_liau_index(string text)
{
    double cl_index;  // Coleman-Liau index

    int letter_count, word_count, sentence_count;
    double  l,          // Average number of letters per 100 words
            s;          // Average number of sentences per 100 words

    letter_count = count_letters(text);
    word_count = count_words(text);
    sentence_count = count_sentences(text);

    l = ((float) letter_count / word_count) * 100;
    s = ((float) sentence_count / word_count) * 100;

    cl_index = (0.0588 * l) - (0.296 * s) - 15.8;

    return cl_index;    
}

// Get the grade a text is suited for given the
// Coleman-Liau index of the text
void print_grade(float cl_index)
{
    int grade = (int) round(cl_index);

    if (grade < 1)
        printf("Before Grade 1\n");
    else if (grade > 16)
        printf("Grade 16+\n");
    else
        printf("Grade %i\n", grade);
}