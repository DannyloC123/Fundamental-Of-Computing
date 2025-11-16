// Libraries --------------------------------------------------------------------------------------------------------------->
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>   // for toupper() and isalpha()


// Global Variables -------------------------------------------------------------------------------------------------------->
#define MAX_WORDS 20
#define MAX_WORD_LENGTH 15
#define BOARD_SIZE 15


// Object for word --------------------------------------------------------------------------------------------------------->
typedef struct {
    int word[MAX_WORD_LENGTH]
} word;


// Functions --------------------------------------------------------------------------------------------------------------->
// Comparison function for qsort to sort strings by length in descending order
int compareStringsDescending(const void *a, const void *b) {
    const char *str1 = (const char *)a;
    const char *str2 = (const char *)b;
    return strlen(str2) - strlen(str1);
}

// Comparison function for qsort to sort strings by length in ascending order
int compareStringsAscending(const void *a, const void *b) {
    const char *str1 = (const char *)a;
    const char *str2 = (const char *)b;
    return strlen(str1) - strlen(str2);
}

// Converts a word to uppercase
void toUpperCase(char *word) {
    for (int i = 0; word[i]; i++)
        word[i] = toupper((unsigned char)word[i]);
}

// Checks if a word is valid: only letters, length between 2 and 15
int isValidWord(const char *word) {
    int len = strlen(word);
    if (len < 2 || len > MAX_WORD_LENGTH)
        return 0; // reject
    for (int i = 0; i < len; i++) {
        if (!isalpha((unsigned char)word[i])) // if not A–Z or a–z
            return 0;
    }
    return 1;
}

// Initialize the board
void initBoard(char board[BOARD_SIZE][BOARD_SIZE], char fill){
    // Initialize the crossword board with dots
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            board[i][j] = fill;  // fill with dots
        }
    }
}

// Prints out board
int printBoards(char board[BOARD_SIZE][BOARD_SIZE], char *title){
    printf("\n%s Board:\n", title);
    printf("-----------------\n");
    for (int i = 0; i < MAX_WORD_LENGTH; i++) {
        printf("|");
        for (int j = 0; j < MAX_WORD_LENGTH; j++) {
            printf("%c", board[i][j]);
        }
        printf("|\n");
    }
    printf("-----------------\n");
}

// Prints out the first word



// Main function
int main(int argc, char *argv[]) {
    char word[MAX_WORD_LENGTH];
    char words[MAX_WORDS][MAX_WORD_LENGTH];
    char board[BOARD_SIZE][BOARD_SIZE];
    int count = 0;

    // Open the file containing words
    FILE *inputFile = NULL;
    FILE *outputFile = NULL;

    // Introduction Message
    printf("Anagram Crossword Puzzle Generator\n");
    printf("----------------------------------\n\n");

    // Check if user provided a filename
    // If not, read words from standard input
    if (argc == 1) {
        printf("Enter a list of words (enter '.' to stop):\n");
        // While loop to get words for crossword until '.' is entered or until MAX_WORDS are entered
        while (count < MAX_WORDS) {
            scanf("%s", word);
            if (strcmp(word, ".") == 0)
                break;

            // Validate and convert to uppercase
            if (isValidWord(word)) {
                toUpperCase(word);
                strcpy(words[count], word);
                count++;
            } else {
                printf("Ignored invalid word: %s\n", word);
            }
        }
    }
    // If a single file is added then read words from the file
    else if (argc == 2) {
        // Read up to MAX_WORDS words from file (one per line or space-separated)
        inputFile = fopen(argv[1], "r");
        if (inputFile == NULL) {
            printf("Error opening file.\n");
            return 1;
        }
        while (count < MAX_WORDS && fscanf(inputFile, "%15s", word) == 1) {
            if (strcmp(word, ".") == 0)
                break;

            // Validate and convert to uppercase
            if (isValidWord(word)) {
                toUpperCase(word);
                strcpy(words[count], word);
                count++;
            } else {
                printf("Ignored invalid word: %s\n", word);
            }
        }
    }
    // If two file names are entered, the first will be used to read words from and the second one will be used to display the output on
    else if (argc == 3)
    {
        // Read up to MAX_WORDS words from file (one per line or space-separated)
        inputFile = fopen(argv[1], "r");
        outputFile = fopen(argv[2], "w");
        if (inputFile == NULL) {
            printf("Error opening input file.\n");
            return 1;
        }
        if (outputFile == NULL) {
            printf("Error opening output file.\n");
            fclose(inputFile);
            return 1;
        }
        while (count < MAX_WORDS && fscanf(inputFile, "%15s", word) == 1) {
            if (strcmp(word, ".") == 0)
                break;

            // Validate and convert to uppercase
            if (isValidWord(word)) {
                toUpperCase(word);
                strcpy(words[count], word);
                count++;
            } else {
                printf("Ignored invalid word: %s\n", word);
            }
        }
    }
    else {
        printf("Usage:\n");
        printf("  %s              → manual input\n", argv[0]);
        printf("  %s input.txt    → read from file\n", argv[0]);
        printf("  %s input.txt output.txt → read from file and write to file\n", argv[0]);
        return 1;
    }

    // Initialize the crossword board with dots
    initBoard(board, '.');

    // Display the Solution Board
    printBoards(board, 'Solution');

    // Initialize the crossword board with dots
    initBoard(board, '#');

    // Display the Crossword Puzzle Board
    printBoards(board, 'Crossword Puzzle');

    // Sort words by length in either descending order
    qsort(words, count, sizeof(words[0]), compareStringsDescending);

    // Display the entered words
    printf("\nYou entered the following words:\n");
    for (int i = 0; i < count; i++) {
        if (outputFile)  // If output file is provided, write there
            fprintf(outputFile, "%s\n", words[i]);
        else              // Otherwise print to console
            printf("%s\n", words[i]);
    }

    // Close any open files safely
    if (inputFile != NULL)
        fclose(inputFile);
    if (outputFile != NULL)
        fclose(outputFile);

    return 0;
}
