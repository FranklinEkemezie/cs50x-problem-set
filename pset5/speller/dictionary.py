"""
Python implementation of `dictionary.c`
"""

# Open the file
text_filename = "texts/her.txt"

with open(text_filename, "r") as text_file:
    for line in text_file:
        print(line)