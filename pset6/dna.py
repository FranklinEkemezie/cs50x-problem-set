import csv
import sys
import os


def main():

    # TODO: Check for command-line usage
    if(len(sys.argv) != 3):
        print('Usage: python dna.py database.csv sequence.txt')
        return

    database_filename, sequence_filename = sys.argv[1: ]

    # Check if file exists
    if not os.path.isfile(database_filename):
        print(f'File {database_filename} does not exist')
        return
    
    if not os.path.isfile(sequence_filename):
        print(f'File {sequence_filename} does not exist')
        return

    # TODO: Read database file into a variable
    persons = []
    strs = []
    with open(database_filename) as db_file:
        db_reader = csv.DictReader(db_file)

        # Get the STRS in the database
        strs = [str.strip() for str in db_reader.fieldnames[1:]]

        # Add the person from database and details
        for person in db_reader:
            persons.append(person)
    
    # TODO: Read DNA sequence file into a variable
    sequence = (open(sequence_filename)).read()

    # TODO: Find longest match of each STR in DNA sequence
    longest_str_matches = {str: 
        longest_match(sequence, str) for str in strs
    }

    # TODO: Check database for matching profiles
    for person in persons:
        is_a_match = True

        for str in longest_str_matches:
            if (int(person[str]) != int(longest_str_matches[str])):
                is_a_match = False
                break

        if is_a_match:
            return person['name']

    return 'No match'


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1
            
            # If there is no match in the substring
            else:
                break
        
        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


print(
    main()
)
