#include <stdio.h>

#include "../cs50/cs50.h"

int get_change_owed();
int min_coins_to_pay(int change);

int main(void)
{
    // Prompt the user for the change owed
    int change_owed = get_change_owed();

    // Print the minimum coins to pay change
    printf("%i", min_coins_to_pay(change_owed));
}

int get_change_owed()
{
    int change;
    do
    {
        change = get_int("Change owed: ");
    } while (!(change > 0));
    
    return change;
}

int min_coins_to_pay(int change)
{
    int coins = 0;      // number of coins counter
    while (change > 0)
    {
        // Can a 'quarter' be used to pay change?
        if (change >= 25)
        {
            coins++;
            change -= 25;
        }
        // Can a 'dime' be used to pay change?
        else if (change >= 10)
        {
            coins++;
            change -= 10;
        }
        // Can a 'nickel' be used to pay change?
        else if (change >= 5)
        {
            coins++;
            change -= 5;
        }
        // Lastly, can a 'peny' be used to pay change?
        else if (change >= 1)
        {
            coins++;
            change -= 1;
        }
    }

    return coins;
}