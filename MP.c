#include <stdio.h>
#include <stdbool.h>

#define SIZE 16

//testing git

int winningPatterns[4][4] = {
    {0, 1, 2, 3},    // (1,1),(1,2),(1,3),(1,4)
    {0, 5, 10, 15},  // (1,1),(2,2),(3,3),(4,4)
    {3, 6, 9, 12},   // (1,4),(2,3),(3,2),(4,1)
    {12, 13, 14, 15} // (4,1),(4,2),(4,3),(4,4)
};

bool Uno[SIZE], Tres[SIZE], F[SIZE]; // each
bool turn = true, go = false, over = false;

/*
The posToIndex Function converts the position inputs @param x and @param y
into a number from 0 to 15, which would be used as the index of Uno and Tres
for various computations in the NextPlayerMove Function.

@param x - Inputted ROW position.
@param y - Inputted COLUMN position

returns a number from 0 to 15 (e.g. [1,1] = 0, [4,4] = 15, [3,2] = 9)
*/

int posToIndex(int x, int y)
{
    return (x - 1) * 4 + (y - 1); // Formula for conversion
}
/*

The updateF function updates the F array, which tracks available positions.
A position is available (true) if neither Uno nor Tres has occupied it.

*/
void updateF()
{
    for (int i = 0; i < SIZE; i++)
    {
        F[i] = !Uno[i] && !Tres[i];
    }
}
/* 
The displayBoard function prints the current game board. 
It shows the 4x4 grid with row and column labels for easy reference.

Legend:
    - 'U' represents a position claimed by Uno. 
    - 'T' represents a position claimed by Tres. 
    - '.' represents an unoccupied position. 

The board is displayed in a structured format with borders. 
*/
void displayBoard()
{
    printf("\n  1 2 3 4\n"); // Column numbers
    for (int i = 0; i < 4; i++)
    {
        printf("%d ", i + 1); // Row numbers
        for (int j = 0; j < 4; j++)
        {
            int idx = posToIndex(i + 1, j + 1);
            if (Uno[idx])
            {
                printf("U ");
            }
            else if (Tres[idx])
            {
                printf("T ");
            }
            else
            {
                printf(". ");
            }
        }
        printf("\n");
    }
    printf("\n");
}
/* 
The isWinning function checks if a given player has met a winning condition.asm

@param playerSet - A boolean array representing the player's occupied positions. 

@return true if the player has occupied all positions in any winning pattern,
        false otherwise
*/
bool isWinning(bool playerSet[SIZE])
{
    for (int i = 0; i < 4; i++) // Loop through each winning pattern
    {
        bool win = true; // Assume the pattern is a win
        for (int j = 0; j < 4; j++)
        {
            if (!playerSet[winningPatterns[i][j]]) // If the playerSet does not occupy (true/1) the current position (winningPatterns[i][j])
            {
                win = false; // then mark the pattern as false and break the inner loop
                break;
            }
        }
        if (win)
            return true;
    }
    return false;
}
/*
The checkGameOver function checks if the game has ended. 
It determines if a player has won or if the board is full. 

Winning conditions:    
                - Uno wins if all positions in a winning pattern are occupied by Uno. 
                - Tres wins if all positions in a winning pattern are occupied by Tres. 
                - Dos wins if there are no available positions left on the board
*/
void checkGameOver()
{
    if (isWinning(Uno))
    {
        printf("Uno Wins!\n");
        over = true;
        return;
    }
    if (isWinning(Tres))
    {
        printf("Tres Wins!\n");
        over = true;
        return;
    }

    updateF();
    bool empty = false;
    for (int i = 0; i < SIZE; i++)
    {
        if (F[i])
        {
            empty = true;
            break;
        }
    }

    if (!empty)
    {
        printf("Dos Wins!\n");
        over = true;
    }
}
/*
The NextPlayerMove function processes a player's move

@param x - The row position selected by the player.
@param y - The column position selected by the player. 

    - Uno and Tres claim positions on their respective turns. 
    - Dos removes an occupied position when it is its turn. 
    - If a move is invalid (out of bounds or already occupied), the player is asked to retry.
*/
void NextPlayerMove(int x, int y)
{
    if (over)
        return;

    int pos = posToIndex(x, y);

    if (turn && go && F[pos])
    { // Uno's move
        Uno[pos] = true;
        turn = !turn;
        go = !go;
    }
    else if (!turn && (Uno[pos] || Tres[pos]))
    { // Dos removes a move
        Uno[pos] = false;
        Tres[pos] = false;
        turn = !turn;
    }
    else if (turn && !go && F[pos])
    { // Tres's move, goes first.
        Tres[pos] = true;
        go = !go;
    }
    else
    {
        printf("Invalid move or position taken.\n");
        return;
    }

    displayBoard(); // Show the board after every move
    updateF();
    checkGameOver();
}

int main()
{
    for (int i = 0; i < SIZE; i++) // initalize all of Uno's and Tres' values to false.
    {
        Uno[i] = false;
        Tres[i] = false;
    }
    updateF();
    printf("========================================\n\n");
    printf("Welcome to (CCDS)TRU-TAC-TOE!\n\n");
    printf("========================================\n\n");
    printf("Rules:\n");
    printf(" - Uno (U) and Tres (T) take turns claiming spaces.\n");
    printf(" - Dos removes positions to disrupt the game.\n");
    printf(" - The game ends when a player wins or the board is full.\n\n");
    printf("========================================\n");
    displayBoard();

    while (!over) // loops until either one of the player wins or there are no more free positions
    {
        int x, y;

        if (turn) // if turn is true, Uno or Tres moves
        {
            if (go)
            {
                // if go is true, Uno moves, else Tres moves
                printf("{(x,y) | x,y E {1,2,3,4}}\n");
                printf("Uno's turn to claim! Enter coordinates (row - col): ");
            }
            else
            {

                printf("{(x,y) | x,y E {1,2,3,4}}\n");
                printf("Tres's turn to claim! Enter coordinates (row - col): ");
            }
        }
        else
        { // if turn is false, Dos moves
            printf("========================================\n");
            printf("Dos's turn to remove! Enter coordinates (row - col): ");
        }

        if (scanf("%d %d", &x, &y) != 2)
        {
            printf("Invalid input. Enter numbers only.\n");
            while (getchar() != '\n')
                continue;
        }

        if (x < 1 || x > 4 || y < 1 || y > 4)
        {
            printf("{(x,y) | x,y E {1,2,3,4}}\n");
            printf("Out of Bounds! Enter a row and column between 1 - 4.\n");
            continue; // skip remaining code, loop again
        }

        NextPlayerMove(x, y);
    }
    printf("========================================\n");
    printf("Game Over! Thanks for Playing!\n");
    printf("========================================\n");
    return 0;
}
