/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

void swap(int * a, int * b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    int value = d*d - 1;
    // Initiialize all tiles with decreased order (0 represents blank tile)
    for(int y = 0; y < d; y++)
        for(int x = 0; x < d; x++, value--)
            board[y][x] = value;
    // Swap before before last with before last (the last is blank) if the height and width of the board are even
    if (d%2 == 0)
        swap(&board[d-1][d-3], &board[d-1][d-2]);
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    for(int y = 0; y < d; y++)
    {
        for(int x = 0; x < d; x++)
        {
            if (board[y][x] > 0)
                printf("%4i", board[y][x]);
                //       ^
                //       |
                //   draw width
            else
                printf("%4c",'_');
        }
        printf("\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    int yPos = -1;
    int xPos = -1;

    // Find tile position (if not found xPos and xPos remain negative)
    for(int y = 0; y < d; y++)
        for(int x = 0; x < d; x++)
            if (board[y][x] == tile)
            {
                yPos = y;
                xPos = x;
            }  
    if (xPos == -1 || yPos == -1)
        return false;
    
    ///////////////
    // Check if around found position can move
        // Y cases
        for(int y = -1; y<2; y+=2)
        {
            // border cases
            if (yPos + y < 0 || yPos + y >= d)
                continue;

            // If empty position swap and retrun true
            if (board[yPos + y][xPos] == 0)
            {
                swap(&board[yPos + y][xPos], &board[yPos][xPos]);
                return true;
            }
                
        }

        // X cases
        for(int x = -1; x<2; x+=2)
        {
            if (xPos + x < 0 || xPos + x >= d)
                continue;
            if (board[yPos][xPos + x] == 0)
            {
                swap(&board[yPos][xPos + x], &board[yPos][xPos]);
                return true;
            }
        }
    /////////////////////////////////

    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    int lastValue = -1;
    // Just check if last visited tile is smaller than current tile
    for(int y = 0; y < d; y++)
        for(int x = 0; x < d; x++)
        {
            if (lastValue == -1)
            {
                lastValue = board[y][x];
                continue;
            }
            // If we at last tile (we don't return false earlier)
            // then surely we are at the last blank tile (equal zero) and so we won! 
            if ((y == d-1 && x == d-1) && board[y][x] == 0)
                return true;

            if (board[y][x] > lastValue)
                lastValue = board[y][x];
            else
                return false;
        }
    // something unexpected happend?!
    return false;
}
