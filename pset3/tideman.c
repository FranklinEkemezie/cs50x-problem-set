#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../cs50/cs50.h"

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool graph_forms_a_cycle(void);

int main(int argc, string argv[])
{
    // Check for invalid storage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n");
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    // // TEST: Print the preferences
    // printf("Preferences: \n");
    // for (int i = 0; i < candidate_count; i++)
    // {
    //     for (int j = 0; j < candidate_count; j++)
    //     {
    //         printf("%i, ", preferences[j][i]);
    //     }
    //     printf("\n");
    // }
    // printf("\n");

    add_pairs();
    // // TEST: Print all the pairs
    // printf("Pairs: \n");
    // for (int i = 0; i < pair_count; i++) {
    //     pair p = pairs[i];

    //     int p_w = p.winner,
    //         p_l = p.loser;

    //     printf("(%s - %i, %s - %i)\n", 
    //         candidates[p_w], preferences[p_w][p_l],
    //         candidates[p_l], preferences[p_l][p_w]
    //     );
    // }
    // printf("\n");

    sort_pairs();
    // // TEST: Print all the pairs (now sorted)
    // printf("Sorted Pairs: \n");
    // for (int i = 0; i < pair_count; i++) {
    //     pair p = pairs[i];

    //     int p_w = p.winner,
    //         p_l = p.loser;

    //     printf("(%s - %i, %s - %i)\n", 
    //         candidates[p_w], preferences[p_w][p_l],
    //         candidates[p_l], preferences[p_l][p_w]
    //     );
    // }
    // printf("\n");

    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0) {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            // Get the index of both candidates 'i' and 'j' in the 'ranks' array
            int index_i, index_j;
            for (int k = 0; k < candidate_count; k++)
            {
                if (strcmp(candidates[ranks[k]], candidates[i]) == 0)
                    index_i = k;

                if (strcmp(candidates[ranks[k]], candidates[j]) == 0)
                    index_j = k;
            }

            // If 'index_i' is less than 'index_j', candidate 'i' appears before candidate 'j'
            // in the 'ranks' array, so voter 'i' prefers to 'j'
            if (index_i < index_j)
                preferences[i][j]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            // Each (i, j) combination is a pair.
            // Each pair meets twice: As (i, j) and then as (j, i)
            // Combbination (i, j) represents the same pair as (j, i)
            // If i > j, then the pair must have been considered before,
            // probably as (j, i)
            // Also, if i == j, pair contains same candidates, ignore them too
            if (i >= j) continue;

            // Compare the voter preferences of i over j and j over i
            if (preferences[i][j] != preferences[j][i]) // no tie
            {
                pair *p = malloc(sizeof(pair));
                p->winner = preferences[i][j] > preferences[j][i] ? i : j;
                p->loser = preferences[i][j] > preferences[j][i] ? j : i;
                
                pairs[pair_count] = *p;
                pair_count++;
            }
        }
    }

    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    // Sort pairs in decreasing ordder of strength of victory.
    // strength of victory = number of voters who prefer the preferred candidate
    if (!(pair_count > 1)) return;

    // Using bubble sort alogrithm
    bool any_swap_done;
    for (int i = 0; i < pair_count; i++)
    {
        any_swap_done = false;
        for (int j = 0; j < pair_count - 1 - i; j++)
        {
            // Compare between neighbours, j and j + 1 say a and b
            pair pair_a = pairs[j];
            pair pair_b = pairs[j + 1];

            // Compute the strength of these neighbours
            int strength_a = preferences[pair_a.winner][pair_a.loser];
            int strength_b = preferences[pair_b.winner][pair_b.loser];

            if (strength_a >= strength_b) continue; // neighbours are in right order

            // Swap them
            pair tmp = pair_a;
            pairs[j] = pair_b;
            pairs[j + 1] = tmp;

            // A swap has been done, record it
            any_swap_done = true;
        }

        if(!any_swap_done) break;
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    for (int i = 0; i < pair_count; i++)
    {
        pair p = pairs[i];

        int lock_er = p.winner,     // the person who locks in
            lock_ee = p.loser;      // the person who is locked in

        // Check if 'locking pair in' would create a cycle.
        // First, lock in
        locked[lock_er][lock_ee] = true;
        
        // If locking form a cycle, reverse it and move over
        // to the next pair
        if (graph_forms_a_cycle()) {
            locked[lock_er][lock_ee] = false;
            continue;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    for (int i = 0; i < pair_count; i++) 
    {
        pair p = pairs[i];

        // check if any arrow points to the winner of the current pair,
        // in other words, if the winner in that pair has been locked
        bool curr_pair_winner_has_been_locked = false;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[i][p.winner]) {
                curr_pair_winner_has_been_locked = true;
                break;
            }
        }

        if (!curr_pair_winner_has_been_locked)
        {
            printf("%s\n", candidates[p.winner]);
            break;
        }
        
    }
    return;
}

// Checks if the 'adjacent matrix' representation of our graph form a cycle.
// A graph represented in "adjacent matrix" is said to form
// a cycle if there exists a non-zero (in this case, a 'true') entry
// for each row.
// That is, if we can find at least a row which does not contain
// a non-zero value in the graph, the graph is not in a cycle
bool graph_forms_a_cycle(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        bool row_contains_non_zero_entry = false;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i])
            {
                row_contains_non_zero_entry = true;
                break;
            }
        }

        if (!row_contains_non_zero_entry)
            return false;
    }
    return true;
}