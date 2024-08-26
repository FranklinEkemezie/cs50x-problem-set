#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../cs50/cs50.h"

int score_word(string word);
void announce_winner(string player1_word, string player2_word);

const int POINTS[] = {
    1, 3, 3, 2, 1, 4, 2, 4, 1, 8,
    5, 1, 3, 1, 1, 3, 10, 1, 1, 1,
    1, 4, 4, 8, 4, 10
};

int main(void)
{
    string player1 = get_string("Player 1: ");
    string player2 = get_string("Player 2: ");

    announce_winner(player1, player2);
}

int score_word(string word)
{
    int score = 0, letter_index;
    // word = toupper(word);

    for (int i = 0, len = strlen(word); i < len; i++)
    {
        letter_index = (int) word[i] - 65;      // index of the letter from ASCII 'A'
        if (letter_index >= 0 && letter_index < 26)
        {
            score += POINTS[letter_index];
        }
    }

    return score;
}

void announce_winner(string player1_word, string player2_word)
{
    int player1_score = score_word(player1_word);
    int player2_score = score_word(player2_word);

    if (player1_score > player2_score)
    {
        printf("Player 1 wins!");
    }
    else if (player1_score < player2_score)
    {
        printf("Player 2 wins!");
    }
    else
    {
        printf("Tie!");
    }
    printf("\n");
}
