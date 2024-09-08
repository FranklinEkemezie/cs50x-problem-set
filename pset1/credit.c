#include <stdio.h>
#include <ctype.h>

#include "../cs50/cs50.h"

long long get_card_number();
string get_card_type(long long card_no);
int sum_of_digits(int number);
int calculate_card_luhn_checksum(long long card_no);
bool is_card_checksum_valid(long long card_no);
bool is_card_amex(long long card_no);
bool is_card_mastercard(long long card_no);
bool is_card_visa(long long card_no);
int no_of_digits_long_long(long long number);
long long get_first_n_digits_long_long(int n, long long number);
long long power_long_long(long long number, int power);


int main(void)
{
    // Prompt the user for the card number
    // long long card_no = get_card_number();

    long long cards[] = {
        378282246310005,
        371449635398431,
        3566002020360505,
        4003600000000014,
        2223000048400011,
        66792452876468,
        4012888888881881,
        4999992222222229,
        6490568974560356,
        2222222222222222,
        4111111111111111,
        202495847344849854,
        54545667654443,
        4012888888881881,
        5199999999999991,
        3566002020360505,
        4095302037452454,
        4999991111111113,
        5105105105105100,
        467842436334276,
        6011111111111117,
        9154871245,
        4054356579876675,
        202022202020202002,
        4222222222222,
    };

    for (int i = 0; i < 25; i++)
    {
        printf("Card Num %i   :   %lld\n", i + 1000, cards[i]);
        printf("Card Checksum   :   %i\n", calculate_card_luhn_checksum(cards[i]));
        printf("Checksum valid  :   %s\n", is_card_checksum_valid(cards[i]) ? "true" : "false");
        printf("No. of digits   :   %i\n", no_of_digits_long_long(cards[i]));
        printf("First 3 digits  :   %i\n", get_first_n_digits_long_long(3, cards[i]));
        printf("Card type is    :   %s\n", get_card_type(cards[i]));
        printf("-------------------------------------------------");

        printf("\n\n");
    }


}

long long get_card_number()
{
    long long card_no;
    do
    {
        card_no = get_long_long("Enter your card number: ");
    } while (card_no < 1);

    return card_no;
}

string get_card_type(long long card_no)
{
    // Validate card
    if (is_card_checksum_valid(card_no))
    {
        if (is_card_amex(card_no)) return "AMEX";
        if (is_card_mastercard(card_no)) return "MASTERCARD";
        if (is_card_visa(card_no)) return "VISA";
    }
    return "INVALID";
}

// Check if a card is valid
// using Luhn's algorithm
bool is_card_checksum_valid(long long card_no)
{
    return calculate_card_luhn_checksum(card_no) % 10 == 0;
}

// Calculate the checksum of a card
// using Luhn's algorithm
int calculate_card_luhn_checksum(long long card_no)
{
    // Loop over the digits of the number,
    // starting from the last digit

    int counter = 0, checksum = 0, last_digit;
    while (card_no > 0)
    {
        int last_digit = card_no % 10;

        // printf("%i\n", last_digit);

        if ((++counter) % 2 == 0) // Are we at the even position?
        {
            // Add the sum of the double of
            // digits at that position
            checksum += sum_of_digits(last_digit * 2);
        }
        else // else, we are at the odd position!
        {
            // Add the last digit to the checksum
            checksum += last_digit;
        }

        // Decrement the number to remove the last digit
        card_no -= (long long)last_digit;
        card_no /= 10;
    }

    return checksum;
}

int sum_of_digits(int number)
{
    int sum_of_digits = 0, last_digit;
    while (number > 0)
    {
        last_digit = number % 10;

        sum_of_digits += last_digit;

        number -= last_digit;
        number /= 10;
    }

    return sum_of_digits;
}

int no_of_digits_long_long(long long number)
{
    int no_of_digits = 0, last_digit;
    while (number > 0)
    {
        last_digit = number % 10;

        no_of_digits++;

        number -= last_digit;
        number /= 10;
    }

    return no_of_digits;
}

// Get the first 'n' digits of a long long type number
long long get_first_n_digits_long_long(int n, long long number)
{
    int no_of_digits = no_of_digits_long_long(number);
    if (n <= 0 || n > 6) return 0;
    long long dvsr = power_long_long((long long) 10, no_of_digits - n);        // divisior
    return number / dvsr;
}


// Get the first 'n' digits of a long long type number
// int get_first_n_digits_long_long(int n, long long number)
// {
//     // 548102
//     int n_digits = 0,           // keeps track of the first n digits gotte
//         first_n_digits = 0,
//         curr_no_of_digits;     // number of digits 

//     while (n_digits  < n)
//     {
//         int counter = 0,
//             first_digit,
//             curr_no_of_digits = no_of_digits_long_long(number);

//         long long   place_value = (long long) power_long_long((long long) 10, curr_no_of_digits - 1),       // place value multiples
//                     place_value_multiples = 0
//         ;

//         // Loop over to get the first digit of the number
//         while (number > place_value_multiples)
//         {
//             counter++;

//             place_value_multiples = place_value_multiples + place_value;
//         }

//         // While looping, we passed over the number to verify it's larger
//         // To account for that:
//         counter--;
//         place_value_multiples -= place_value; 

//         first_digit = counter;
//         number -= place_value_multiples;

//         // printf(">>: %i\n", number > place_value_multiples);
//         // printf("Number: %lld\n", number);
//         // printf("Counter: %i\n", counter);
//         // printf("Place val: %lld\n", place_value);
//         // printf("Place val mul: %lld\n", place_value_multiples);
//         // printf("First digit: %i\n", first_digit);

//         // printf("\n");

//         first_n_digits = (first_n_digits * 10) + first_digit;

//         n_digits++;
//     }

//     return first_n_digits;

// }


bool is_card_amex(long long card_no)
{
    if (no_of_digits_long_long(card_no) != 15) return false;

    int first_2_digits = get_first_n_digits_long_long(2, card_no);
    if (first_2_digits != 34 && first_2_digits != 37) return false;

    return true;
}

bool is_card_mastercard(long long card_no)
{
    if (no_of_digits_long_long(card_no) != 16) return false;
    
    int first_2_digits = get_first_n_digits_long_long(2, card_no);
    if (first_2_digits < 51 || first_2_digits > 55) return false;

    return true;
}

bool is_card_visa(long long card_no)
{
    int len = no_of_digits_long_long(card_no);
    if (len != 15 && len != 16) return false;

    if (get_first_n_digits_long_long(1, card_no) != 4) return false;

    return true;
}

long long power_long_long(long long number, int power)
{
    int counter = 1;
    long long result = number;
    for ( ; counter < power; result *= number, counter++);
    return result;
}