#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../cs50/cs50.h"

int score_word(string word);
void announce_winner(int player1_score, int player2_score);
string strtoupper(string str);

const int POINTS[] = {
    1, 3, 3, 2, 1, 4, 2, 4, 1, 8,
    5, 1, 3, 1, 1, 3, 10, 1, 1, 1,
    1, 4, 4, 8, 4, 10
};

int main(void)
{
    // Prompt the user for two words
    string player1_word = get_string("Player 1: ");
    string player2_word = get_string("Player 2: ");

    // Computer the score of each user
    int player1_score = score_word(player1_word);
    int player2_score = score_word(player2_word);

    // Print the winnner
    announce_winner(player1_score, player2_score);
}

int score_word(string word)
{
    int score = 0, letter_index;
    word = strtoupper(word);

    for (int i = 0, len = strlen(word); i < len; i++)
    {
        letter_index = (int) word[i] - (int) 'A';      // index of the letter from ASCII 'A'
        if (letter_index >= 0 && letter_index < 26)
            score += POINTS[letter_index];
    }

    return score;
}

void announce_winner(int player1_score, int player2_score)
{
    if (player1_score > player2_score)
        printf("Player 1 wins!");
    else if (player1_score < player2_score)
        printf("Player 2 wins");
    else
        printf("Tie!");
    printf("\n");
}

string strtoupper(string str)
{
    for (int i = 0, len = strlen(str); i < len; i++)
        str[i] = toupper(str[i]);

    return str;
}