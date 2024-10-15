# Comipler
CC = gcc

# Compiler Flags
CLFAGS = -Wall -Wextra -std=c99

# Target executable name
TARGET = speller_alt

# Object files
OBJ = speller_alt.o dictionary.o

# Build target
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJ)
	$(CC) $(CLFAGS) -o $(TARGET) $(OBJ)

# Compile speller.c to an object file
speller.o: speller_alt.c dictionary.h
	$(CC) $(CLFAGS) -c speller_alt.c

# Compile dictionary.c to an object file
dictionary.o: dictionary.c dictionary.h
	$(CC) $(CLFAGS) -c dictionary.c

# Clean the directory by removing object files and executable
clean:
	rm -f $(OBJ) $(TARGET)

# Run the program with a text file
run:
	./$(TARGET) texts/aca.txt