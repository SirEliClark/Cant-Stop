/******************************************
 *
 * Official Name:  Eli Clark
 *
 * E-mail:  eaclark@syr.edu
 *
 * Final Project:  Don't Stop
 *
 * Compiler:  Visual Studio 2017
 *
 * Date:  April 13, 2018
 *
 *******************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // for rand(), srand()
#include <time.h> // for time()
#include <math.h> // for abs()

#define WIDTH 67
#define HEIGHT 33
#define LENGTH 30

/*STRUCTURE DEFINITION*/
typedef struct { // (STRUCT)
    char player[LENGTH];
    char name[LENGTH];
    int score;
    char board[HEIGHT][WIDTH];
    int fifthDice[3];
    int timesUsed[3];
    int numFifthDice;
    int dice[5];
    char selections[5];
    int intselections[5];
    int numSelections;
    int selecValues[3];
    int scoreMarks[11];
    int individualScores[11];
} playerType;

/*FUNCTION PROTOTYPES*/
void startGame();
void setGameBoard(char sb[][WIDTH]);
void displayGameBoard(char sb[][WIDTH]);
int mainMenu();
int playComputer();
int play2player();
void initializeValues(playerType * player);
int whoStarts(char player1[], char player2[]);
int start2player(playerType * player1, playerType * player2);
void getPlayerName(playerType * player1, char whichPlayer[]);
int rollDie();
void printDie(int * dieVal);
void roll5Dice(int dice[]);
void Print5Dice(int dice[]);
int playTurn(playerType * player);
int selectDice(playerType * player);
int check5thDie(playerType player);
void select5thDie(playerType * player);
void select1stPair(playerType * player);
void select2ndPair(playerType * player);
void freePlay2ndSelection(playerType * player);
int confirmSelection(playerType * player);
int checkSelection(playerType player);
int checkGameOver(int times5thDieUsed[]);
void assignSelections(playerType * player);
void markBoard(playerType * player);
int search5thDie(playerType * player);
void calculateScore(playerType * player);
void drawScore(playerType * player);
int displayRules();
void readRules();
void readHighScores(char names[][LENGTH], int scores[]);
void updateHighScoreFile(playerType player1, playerType player2);
void sortHighScores(int score[], int numItems, char player[][LENGTH]);
int displayHighScores(char names[][LENGTH], int scores[]);
int searchHighScores(char theArray[][LENGTH], int arrSize, char target[]);
void printScore(int index, int scores[]);
void removeNewLine(char array[]);
int getMaxRange(int list[], int first, int last);
void intSwap(int * first, int * second);
void stringSwap(char first[], char second[]);


/*************************      MAIN      *************************/
int main()
{
    srand(time(NULL)); // (SRAND)
    
    startGame();
    
    return 0;
}

/*FUNCTION DEFINITIONS*/
/**/
void startGame()
{
    int gameOver = 0;
    char highScoreNames[10][LENGTH];
    int highScores[10];
    //playerType player1, player2;
    
    /*player1.score = 50;
    player2.score = -10;
    strcpy(player1.name, "PLAYER 1");
    strcpy(player2.name, "PLAYER 2");*/
    
    readHighScores(highScoreNames, highScores);
    
    //updateHighScoreFile(player1, player2);
    
    while (!gameOver)
    {
        switch (mainMenu())
        {
            case 1:
                gameOver = play2player();
                break;
            case 2:
                gameOver = displayHighScores(highScoreNames, highScores);
                break;
            case 3:
                gameOver = 1;
                break;
            default:
                gameOver = 0;
                break;
        }
    }
}
/* Draws the gameboard into the parameter screenbuffer */
void setGameBoard(char sb[][WIDTH])
{
    int points[] = {100, 70, 60, 50, 40, 30, 40, 50, 60, 70, 100};
    int j;
    // Draw the horizontal lines in the gameboard
    for (int i = 0; i <= HEIGHT - 2; i += 2)
    {
        for (j = 0; j < WIDTH - 1; j++)
            sb[i][j] = '-';
        sb[i][j] = '\0';
    }
    j = 2;
    // Draw the scoreing section of the board between the horizontal lines
    for (int i = 9; i < HEIGHT - 2; i += 2)
    {
        sprintf(sb[i], "|%3d|%3d ||   |   |   |   |   |   |   |   |   |   ||      |      |", points[(i - 9) / 2], j);
        j++;
    }
    // Assignment of the rest of the board
    strcpy(sb[1], "|                    |     ||   ||   |   |   |   |   |   |   |   |");
    strcpy(sb[2], "|                    | 5th ---------------------------------------");
    strcpy(sb[3], "|       Player       |     ||   ||   |   |   |   |   |   |   |   |");
    strcpy(sb[4], "|                    | Die ---------------------------------------");
    strcpy(sb[5], "|                    |     ||   ||   |   |   |   |   |   |   |   |");
    strcpy(sb[7], "|         |          Scoring Section               |    Score    |");
    strcpy(sb[31], "          |     -200      | 0 | +   +   +   +   + ||             |");
    strcpy(sb[32], "          --------------------------------------------------------");
}
/* Prints the screen buffer */
void displayGameBoard(char sb[][WIDTH])
{
    // print each character of the screen buffer
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
            printf("%c", sb[i][j]);
        printf("\n");
    }
}

/* Display the main menu and return the selection */
int mainMenu()
{
    int selection;
    char newLine;
    // Print the main menu options
    printf("Welcome to Can't Stop!\n");
    printf("1 - Play game\n");
    printf("2 - High Scores\n");
    printf("Enter 1 or 2 to continue(or 3 to exit)\n>");
    // Get the selection
    scanf("%d%c", &selection, &newLine);
    // Return the selection
    return selection;
}
/* Plays the game */
int play2player()
{
    //playerType highScores[10];
    char highScoreNames[12][LENGTH];
    int highScores[12];
    playerType player1, player2;
    readHighScores(highScoreNames, highScores);
    int gameOver1 = 1, gameOver2 = 1;
    int turn;
    char junk;

    // See who will go first
    turn = start2player(&player1, &player2);
    printf("\nEnter any key to start the game\n");
    scanf(" %c", &junk);
    while (gameOver1 || gameOver2) // Loop exits when gameOver is false for both people
    {
        switch (turn) { // Player 1 plays if 0, player 2 if 1
            case 0:
                gameOver1 = playTurn(&player1); // Player 1 playes turn, returns 0 if out of turns
                turn = 1; // Switches to other players turn
                break;
            case 1:
                gameOver2 = playTurn(&player2); // Player 1 playes turn, returns 0 if out of turns
                turn = 0; // Switches to other players turn
                break;
            default:
                break;
        }
    }
    if (player1.score == player2.score) // If both players have the same score
        printf("%s and %s tied!", player1.name, player2.name); // Print they tied
    else if (player1.score > player2.score)  // If player 1 has a higher score
        printf("%s wins!\n", player1.name); // Print player 1 wins
    else// If player 2 has higher score
        printf("%s wins!\n", player2.name); // Print player 2 wins
    updateHighScoreFile(player1, player2); // Update the highscores
    // Wait for players to be ready to continue
    printf("Enter any key to return to the main menu...");
    scanf(" %c", &junk);
    return 0;
}
/* Players roll the dice to determine who will go first
Parameters are the players names*/
int whoStarts(char player1[], char player2[])
{
    int player1Die, player2Die, first;
    char junk;
    int rollAgain = 1;
    printf("\n\nRoll to see who will go first:\n"); // Promt to start
    while (rollAgain) // While roll again is true
    {
        // Have both players roll a die and print the random die
        printf("\n%s enter any key to roll the die:", player1);
        scanf(" %c", &junk);
        printDie(&player1Die);
        printf("%s enter any key to roll the die:", player2);
        scanf(" %c", &junk);
        printDie(&player2Die);
        if (player1Die > player2Die) // If player 1 has a higher die val
        { // Print player 1 will go first and set the return value to 0 and exit the loop
            printf("%s will go first\n", player1);
            first = 0;
            rollAgain = 0;
        }
        else if (player2Die > player1Die)
        { // Print player 2 will go first and set the return value to 1 and exit the loop
            printf("%s will go first\n", player2);
            first = 1;
            rollAgain = 0;
        }
        else // If the dice are the same value
        { // Print roll again and go back through the loop
            printf("Roll again");
            rollAgain = 1;
        }
    }
    return first;
}
/* Initialize a players values to zero */
void initializeValues(playerType * player) // (*PARAM) (STRUCT PARAM)
{
    (*player).timesUsed[0] = 0; // Times a player has used a fifth die
    (*player).timesUsed[1] = 0;
    (*player).timesUsed[2] = 0;
    (*player).numFifthDice = 0; // Number of fifth dice
    (*player).numSelections = 0; // Number of selections
    (*player).fifthDice[0] = 0; // What the fifth dice are
    (*player).fifthDice[1] = 0;
    (*player).fifthDice[2] = 0;
    for (int i = 0; i < 11; i++)
    {
        (*player).scoreMarks[i] = 0; // Numbeer of marks in each row
    }
    setGameBoard((*player).board); // Sets gameboard
    //displayGameBoard((*player).board);
}

/* Asks for a players name and stores it in the struct for the given player,
 the parameters are the  * player struct used for passing the name and the player type
 and which player is the player type to be printed and then saved in to the struct (i.e. player 1, or player 2) */
void getPlayerName(playerType * player, char whichPlayer[]) // (*PARAM) (STRUCT PARAM)
{
    printf("%s\nEnter your name:\n>", whichPlayer); // Prompts the player to enter their name
    fgets((*player).name, LENGTH, stdin); // Reads in the name
    strcpy((*player).player, whichPlayer); // Copies player type to player struct
    removeNewLine((*player).name); // Removes new line at the end of the name from fgets
}

/* Called at the begining of play two player to begin game
 Uses star parameters to pass back the players names that are input */
int start2player(playerType * player1, playerType * player2) // (*PARAM) (STRUCT PARAM)
{
    char highScoreNames[12][LENGTH];
    int highScores[12];
    readHighScores(highScoreNames, highScores); // Gets the highsccore list
    // Get both players name and print their highscore
    getPlayerName(&(*player1), "Player 1");
    printScore(searchHighScores(highScoreNames, 10, (*player1).name), highScores); // (SEARCH)
    getPlayerName(&(*player2), "Player 2");
    printScore(searchHighScores(highScoreNames, 10, (*player2).name), highScores); // (SEARCH)
    // Initialize both plaeyrs values
    initializeValues(&(*player1));
    initializeValues(&(*player2));
    return (whoStarts((*player1).name, (*player2).name)); // Returns the value for the player who will go first
}

/* Returns a random number between 1 and 6 inclusively */
int rollDie()
{
    return (rand() % 6 + 1); // (RAND)
}

/* Prints a random die, *dieVal is the value printed on the die, gets returned via pointer*/
void printDie(int * dieVal) // (*PARAM)
{
    *dieVal = rollDie();    // Gat random value for *dieVal
    // Print die with random value
    printf("---\n");
    printf("|%d|\n", *dieVal);
    printf("---\n\n");
}

/* Passes back 5 random numbers between 1 and 6 inclusivly through the parameter array */
void roll5Dice(int dice[])
{
    // Get the five random values
    for (int i = 0; i < 5; i++)
        dice[i] = rollDie();
}

/* Prints five random dice, the array dice is used to return the values rolled */
void Print5Dice(int dice[])
{
    // Print the five dice next to eachother with labels below
    printf("--- --- --- --- ---\n");
    printf("|%d| |%d| |%d| |%d| |%d|\n", dice[0], dice[1], dice[2], dice[3], dice[4]);
    printf("--- --- --- --- ---\n");
    printf(" A   B   C   D   E\n\n");
}

/*  */
int playTurn(playerType * player) // (*PARAM) (STRUCT PARAM)
{
    char junk;
    // if the player has filled any of the fifth die rows, if so exit function and return 1
    if (checkGameOver((*player).timesUsed)) // If the player has filled any fifth die row
    {
        printf("\n\n%s:\nOut of turns\n", (*player).name); // Tells the player they are out of turns
        // Calculate the players score and print it to their card
        calculateScore(&(*player));
        drawScore(&(*player));
        displayGameBoard((*player).board);
        return 0; // Leave function with value 0
    }
    // Propt player to begin turn
    printf("\n\n\n\n%s:\n", (*player).name);
    displayGameBoard((*player).board);
    printf("\n\nEnter any key to roll the dice:");
    scanf(" %c%c", &junk, &junk);
    roll5Dice((*player).dice); // Gets their roll
    do { // Have player select their dice
        Print5Dice((*player).dice);
        (*player).numSelections = 0;
        selectDice(&(*player));
    } while (!confirmSelection(&(*player))); // Repeat loop til player confils the selection
    // Print the gameboard with the players selections marked
    markBoard(&(*player));
    displayGameBoard((*player).board);
    printf("\nEnter any key to continue..."); // Wait for player to be ready to continue
    scanf(" %c", &junk);
    return 1;
}

/* Checks if the player is out of turns */
int checkGameOver(int times5thDieUsed[])
{
    for (int i = 0; i < 3; i++) // For i = 0 to 2
    {
        if (times5thDieUsed[i] == 8) // If the number of times that fifth die has been used is 8
            return 1;
    }
    return 0;
}

/* Promts the payer to select their dice the star parameter returns the selections */
int selectDice(playerType * player) // (*PARAM) (STRUCT PARAM)
{
    printf("To select a die enter the corresponding letter (A-E)\n");
    // Choose each die
    select5thDie(&(*player));
    select1stPair(&(*player));
    select2ndPair(&(*player));
    return 1;
}

/* Runs the fifth die selection returns the selection through the star parameter
along with the number of selections */
void select5thDie(playerType * player) // (*PARAM) (STRUCT PARAM)
{
    int index;
    if (check5thDie(*player) > 0) // If you cant choose a fifth die with three already selsected
    { // Print free roll and assign value of 'F' for selection
        printf("Free Roll\n");
        (*player).selections[0] = 'F';
    }
    else
    {
        do
        { // Promt the player to select a fifth die
            printf("Select a fifth die:\t");
            scanf(" %c", &(*player).selections[0]);
            switch ((*player).selections[0])
            { // Assigns corresponding index for die value from the selection
                case 'A':
                case 'a': index = 0;
                    break;
                case 'B':
                case 'b': index = 1;
                    break;
                case 'C':
                case 'c': index = 2;
                    break;
                case 'D':
                case 'd': index = 3;
                    break;
                case 'E':
                case 'e': index = 4;
                    break;
                default: index = 5;
                    printf("Invalid selection try again.\n");
                    break;
            }
            if ((*player).numFifthDice == 3) // If the payer has already selected three fifth dice
                // If the selected die does not equal any of the chosen fifth dice
                if (!((*player).fifthDice[0] == (*player).dice[index]))
                    if (!((*player).fifthDice[1] == (*player).dice[index]))
                        if (!((*player).fifthDice[2] == (*player).dice[index]))
                        {
                            printf("Invalid selection try again.\n");
                            index = 5;
                        }
        } while (index == 5); // Repeat loop if invalid selection
    }
    (*player).numSelections++; // Increment the number of selections
}

/* Makes sure the player has picked a valid selection
Parameter is used for the selections and the number of selections
Returns zero if invalid selection of 1 if valid */
int checkSelection(playerType player) // (STRUCT PARAM)
{
    switch (player.selections[player.numSelections])
    { // Switch based off newest selection
        case 'A':
        case 'a':
        case 'B':
        case 'b':
        case 'C':
        case 'c':
        case 'D':
        case 'd':
        case 'E':
        case 'e':
            break;
        default: // Any entry not A - E will terminate function
            printf("Invalid selection try again.\n");
            return 0;
    }
    for (int i = 0; i < player.numSelections; i++) // For i = 0 to less than the number of selections
        if (player.selections[player.numSelections] == player.selections[i]) // If the selection has already been made
        { // Print invalid and terminate function
            printf("Invalid selection try again.\n");
            return 0;
        }
    return 1; // Return 1 (valid)
}

/* Promts player to select the first pair of dice
Uses star parameter to pass backt he selections and the number of selections */
void select1stPair(playerType * player) // (*PARAM) (STRUCT PARAM)
{
    do
    { // Get the players first selection for pair
        printf("Select a die for your first pair:");
        scanf(" %c", &(*player).selections[1]);
    } while (!checkSelection((*player))); // Repeat if selection is invalid
    (*player).numSelections++; // Increment number of selections
    do
    { // Get the players second selection for pair
        printf("Select another die for your first pair:");
        scanf(" %c", &(*player).selections[2]);
    } while (!checkSelection((*player))); // Repeat if selection is invalid
    (*player).numSelections++; // Increment number of selections
}

/* Selects the second pair of dice
Passes the selections back along with the number of selections via star parameter */
void select2ndPair(playerType * player) // (*PARAM) (STRUCT PARAM)
{
    int alreadyselected[5] = { 0 , 0 , 0 , 0 , 0 };
    if ((*player).selections[0] == 'F') // If the player has a free play
        freePlay2ndSelection(&(*player)); // Get them to select another pair
    else
    { // Auto select the last two dice
        for (int i = 0; i < 3; i++) // for i = 0 to 2
        {
            switch ((*player).selections[i]) { // Based of the selection already made mark as selected
                case 'A':
                case 'a': alreadyselected[0] = 1;
                    break;
                case 'B':
                case 'b': alreadyselected[1] = 1;
                    break;
                case 'C':
                case 'c': alreadyselected[2] = 1;
                    break;
                case 'D':
                case 'd': alreadyselected[3] = 1;
                    break;
                case 'E':
                case 'e': alreadyselected[4] = 1;
                    break;
                default:
                    break;
            }
        }
        // Selects the reamaining choices
        for (int i = 0; i < 5; i++) // For i = 0 to 5
            if (!(alreadyselected[i])) // If the selection has not been made
            {
                (*player).selections[3] = (char)(i + 97); // Make selection
                break;
            }
        for (int i = 0; i < 5; i++) // For i = 0 to 5
            if (!(alreadyselected[i])) // If the selection has not been made
                (*player).selections[4] = (char)(i + 97); // Make selection
    }
}

/* Prompts player to select a second pair of dice */
void freePlay2ndSelection(playerType * player) // (*PARAM) (STRUCT PARAM)
{
    do
    { // Get the first selection for the second pair
        printf("Select a die for your second pair:");
        scanf(" %c", &(*player).selections[3]);
    } while (!checkSelection((*player))); // Repeat if selection is invalid
    (*player).numSelections++; // Increment number of selections
    do
    { // Get the second selection for the second pair
        printf("Select another die for your first pair:");
        scanf(" %c", &(*player).selections[4]);
    } while (!checkSelection((*player))); // Repeat if selection is invalid
    (*player).numSelections++; // Increment number of selections
}

/* Asks the player to confirm the selection
parameter returns the selected values of the dice/pairs via star param
return value of 1 if confirmed or 0 if chose to reselect */
int confirmSelection(playerType * player) // (*PARAM) (STRUCT PARAM)
{
    char selection;
    assignSelections(&(*player)); // Gets the values on the selected dice
    // Prints the selections and asks player to confirm and gets response
    printf("Selection:\n");
    printf("5th die:  %c = %d\nFirst Pair: \t%c + %c = %d\nSecond Pair:\t%c + %c = %d\n", (*player).selections[0], (*player).selecValues[0], (*player).selections[1], (*player).selections[2], (*player).selecValues[1], (*player).selections[3], (*player).selections[4], (*player).selecValues[2]);
    printf("Enter x to reselect or any other key to continue\n");
    scanf(" %c", &selection);
    if (selection == 'x' || selection == 'X') // If the input is x or X
        return 0;
    return 1;
}

/* Gets the values of the selected fifth die and pairs */
void assignSelections(playerType * player) // (*PARAM) (STRUCT PARAM)
{
    for (int i = 0; i < 5; i++) // For each die
    {
        switch ((*player).selections[i]) { // Switch based on the selected letter of die
            // Get corresponding die index
            case 'A':
            case 'a':
                (*player).intselections[i] = 0;
                break;
            case 'B':
            case 'b':
                (*player).intselections[i] = 1;
                break;
            case 'C':
            case 'c':
                (*player).intselections[i] = 2;
                break;
            case 'D':
            case 'd':
                (*player).intselections[i] = 3;
                break;
            case 'E':
            case 'e':
                (*player).intselections[i] = 4;
                break;
            default:
                break;
        }
    }
    // Assign the values of the fifth die and pairs
    (*player).selecValues[0] = (*player).dice[(*player).intselections[0]];
    (*player).fifthDice[search5thDie(&(*player))] = (*player).selecValues[0];
    (*player).selecValues[1] = (*player).dice[(*player).intselections[1]] + (*player).dice[(*player).intselections[2]];
    (*player).selecValues[2] = (*player).dice[(*player).intselections[3]] + (*player).dice[(*player).intselections[4]];
}

/* Puts the x's in the gameboard using the selected values and nuber of marks
form the struct and returns the board via star parameter */
void markBoard(playerType * player) // (*PARAM) (STRUCT PARAM)
{
    if (!((*player).selections[0] == 'F')) // If it is not a free roll
    {
        (*player).board[1 + search5thDie(&(*player)) * 2][35 + (*player).timesUsed[search5thDie(&(*player))] * 4] = 'X';    // Mark the sorresponding row for the chosen fifth die
        ((*player).timesUsed[search5thDie(&(*player))])++; // increment the number of times that fifth die has been used
        if ((*player).numFifthDice < 3) // If there are less than three fifth dice chosen
        {
            (*player).board[1 + search5thDie(&(*player)) * 2][30] = (char)((*player).selecValues[0] + 48); // Put the number of the fifth die in the next availible space
            if ((*player).numFifthDice == search5thDie(&(*player)))
                ((*player).numFifthDice)++;
        }
    }
    // Marks the corresponding row for the chosen pairs
    (*player).board[5 + (*player).selecValues[1] * 2][12 + (*player).scoreMarks[(*player).selecValues[1] - 2] * 4] = 'X'; // marks the corresponding row for the chosen first pair
    if ((*player).scoreMarks[(*player).selecValues[1]] < 8) // If the player has less than eight marks in the row
        ((*player).scoreMarks[(*player).selecValues[1] - 2])++; // Increments the number of times the first pair has been chosen
    (*player).board[5 + (*player).selecValues[2] * 2][12 + (*player).scoreMarks[(*player).selecValues[2] - 2] * 4] = 'X'; // marks the corresponding row for the chosen second pair
    if ((*player).scoreMarks[(*player).selecValues[1]] < 8) // If the player has less than eight marks in the row
        ((*player).scoreMarks[(*player).selecValues[2] - 2])++; // Increments the number of times the second pair has been chosen
}

/* Checks if a fifth die can be chosen from the rolled dice
Returns the index of the fifth die or -1 if none can be chosen */
int search5thDie(playerType * player) // (*PARAM) (STRUCT PARAM)
{
    for (int i = 0; i < 3; i++) // From zero to two
    {
        if ((*player).timesUsed[i] == 0) // If the fifth die has not been used
            return i; // Return that index value
        if ((*player).fifthDice[i] == (*player).selecValues[0]) // If the slected value has already been pickd
            return i; // Return that index value
    }
    return -1; // If none an be selected return -1
}

/* Verifys the selection of the 5th die
returns 0 if valid or 1 if invalid */
int check5thDie(playerType player) // (STRUCT PARAM)
{
    for (int i = 0; i < 3; i++) // From zero to two
    {
        if (player.timesUsed[i] == 0) // If the the fifth die has not been used return zero
            return 0;
        for (int j = 0; j < 5; j++) // From zero to five
            if (player.dice[j] == player.fifthDice[i]) // If any of the dice rolled are equal to any of the previously selected fifth dice
                return 0; // Return zero
    }
    return 1; // If no conditions are met return 1
}

/* Calculates the score of a player passes it back via pointer */
void calculateScore(playerType * player) // (*PARAM) (STRUCT PARAM)
{
    int points[] = {100, 70, 60, 50, 40, 30, 40, 50, 60, 70, 100};
    (*player).score = 0; // Initializes score to zero
    for (int i = 0; i < 11; i++) // From i = zero to i = ten
    {
        if ((*player).scoreMarks[i] == 0) // If a player has no X's int the row
            (*player).individualScores[i] = 0; // No points for the row
        else if ((*player).scoreMarks[i] < 5) // If the player has less than five marks
            (*player).individualScores[i] = -200; // -200 points for the row
        else // If the player has more than five marks
            (*player).individualScores[i] = points[i] * ((*player).scoreMarks[i] - 5); //
        (*player).score += (*player).individualScores [i];
    }
}

/* Puts each digit in the score card for the scores in rows and total
returns scores via star parameter */
void drawScore(playerType * player) // (*PARAM) (STRUCT PARAM)
{
    int positiveScore;
    // Print score for each row
    for (int i = 0; i < 11; i++) // For each value 0 - 10
    {
        if ((*player).individualScores[i] < 0)  // If they scored no points in the row
        { // Print -200
            (*player).board[9 + i * 2][53] = '-';
            (*player).board[9 + i * 2][54] = '2';
            (*player).board[9 + i * 2][55] = '0';
            (*player).board[9 + i * 2][56] = '0';
        }
        else // If they scored in the category
        {
            (*player).board[9 + i * 2][63] = '0'; // Put a zero in the ones place, all possible scores are divisible by 10
            if ((*player).individualScores[i] >= 10) // If the player scored over ten points
                (*player).board[9 + i * 2][62] = (char)(((*player).individualScores[i] % 100) / 10 + 48); // Put the tens digit down
            if ((*player).individualScores[i] >= 100) // If the player scored over 100 points
                (*player).board[9 + i * 2][61] = (char)(((*player).individualScores[i] % 1000 - (*player).individualScores[i] % 10) / 100 + 48); // Put the hundreds digit down
            //if ((*player).individualScores[i] >= 1000)
                //(*player).board[9 + i * 2][60] = (char)(((*player).individualScores[i] % 10000 - (*player).individualScores[i] % 100) / 1000 + 48);

        }
    }
    positiveScore = abs((*player).score); // Gets the positive value of the score, uses to print
    (*player).board[31][60] = '0'; // Put a zero in the ones place, all possible scores are divisible by 10
    if (positiveScore >= 10) // If pos score is over 10
        (*player).board[31][59] = (char)((positiveScore % 100) / 10 + 48); // Put the 10's digit in the board
    if (positiveScore >= 100) // If pos score is over 100
        (*player).board[31][58] = (char)((positiveScore % 1000 - positiveScore % 10) / 100 + 48); // Put the 100's digit in the board
    if (positiveScore >= 1000) // If pos score is over 1000
        (*player).board[31][57] = (char)((positiveScore % 10000 - positiveScore % 100) / 1000 + 48); // Put the 1000's digit in the board
    if ((*player).score < 0) // If score is negative
        (*player).board[31][56] = '-'; // Put a negative sign in the board
}

/* Updated the highscore file takes in the new names and scores */
void updateHighScoreFile(playerType player1, playerType player2) // (STRUCT PARAM)
{
    char player[12][LENGTH];
    int score[12];
    
    readHighScores(player, score); // Reads in previous highscores
    
    FILE * outputF = fopen("highscoresout.txt", "w"); // Open file for output
    // Puts the new names and scores at the end of the array
    strcpy(player[10], player1.name);
    score[10] = player1.score;
    strcpy(player[11], player2.name);
    score[11] = player2.score;
    sortHighScores(score, 12, player); // (SORT) sort the highscores
    for (int i = 0; i < 10; i++) // For i = 0 to 10
    {
        fprintf(outputF, "%s\n%d\n", player[i], score[i]); // (I/O) prints the new highscores to the output file
    }
    fclose(outputF); // Close output file
}

/* Puts the highscores in order from first to last */
void sortHighScores(int score[], int numItems, char player[][LENGTH])
{
    int largestElement;
    for (int i = 0; i < numItems - 2; i++) // For i = number of items in the array to one
    { // Find the largest number in the array and swap it with the begining of the searched array
        largestElement = getMaxRange(score, i, numItems - 1);
        intSwap(&score[largestElement], &score[i]);
        stringSwap(player[largestElement], player[i]);
    }
}

/* Reads the highscores from the imput file, passes them back via the
array parameters */
void readHighScores(char names[][LENGTH], int scores[])
{
    FILE * inputF = fopen("highScores.txt", "r");
    char nSpot;
    for (int i = 0; i < 10; i++) // From i = 0 to 9
    { // Read in a players name and score
        fgets(names[i], LENGTH, inputF); // (I/O)
        removeNewLine(names[i]);
        fscanf(inputF, "%d%c", &scores[i], &nSpot); // (I/O)
    }
    fclose(inputF);
}

/* Takes in the highscores as parameters and prints
 return value of 1 to continue to menu*/
int displayHighScores(char names[][LENGTH], int scores[])
{
    char goBack;
    for (int i = 0; i < 10; i++) // For each highscore
    {
        printf("#%2d %5d\t%s\n", i + 1, scores[i], names[i]); // Print the highscore and the name
    }
    // Wait for the player to be ready to continue
    printf("Enter any character to go back to main menu...");
    scanf(" %c", &goBack);
    return 1;
}

/* Searches he highscore player list for a certain name and returns the index value
or -1 if no highscore is found */
int searchHighScores(char theArray[][LENGTH], int arrSize, char target[])
{
    for (int i = 0; i < arrSize; i++) // For each element in the array
    {
        if (!(strcmp(theArray[i], target))) // If the element matches the target
            return i;  // Return the corresponding index
    }
    return -1; // If no value is found return -1
}

/* Uses the highscore index from search parallel to the scores to tell the player their highscore*/
void printScore(int index, int scores[])
{
    if (index == -1) // If no highscore is on the board
        printf("You are not on the high score board yet. Beat %d to get on the board!\n", scores[9]);
    else // If highscore is found print the players highscore
        printf("Your high score is %d\n", scores[index]);
}

/* Removes new line read in from fgets */
void removeNewLine(char array[])
{
    int nSpot;
    nSpot = strlen(array) - 1; // set the index equal to the length minus 1
    array[nSpot] = '\0'; // Set the last character to the numm character
}

/* Find the largets item in the input array between the first and last input index
 Returns the largest index*/
int getMaxRange(int list[], int first, int last)
{
    int index = first;
    for (int i = first + 1; i <= last; i++) // From the first index + 1 to the last index
        if (list[i] > list[index]) // If the current value is greater than the current largets value
            index = i; // Set the largest index equal to the surrent index
    return index;
}

/* Swaps integers */
void intSwap(int * first, int * second) // (*PARAM)
{
    int temp;
    temp = *first;
    *first = *second;
    *second = temp;
}

/* Swaps strings */
void stringSwap(char first[], char second[])
{
    char temp[LENGTH];
    if (!(strcmp(first, second) == 0))
    {
        strcpy(temp, first);
        strcpy(first, second);
        strcpy(second, temp);
    }
}
