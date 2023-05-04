#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

#define BOARD_SIZE 10
#define NUM_SHIPS 5

char player1_board[BOARD_SIZE][BOARD_SIZE];
char player1_ATKboard[BOARD_SIZE][BOARD_SIZE];
char player2_board[BOARD_SIZE][BOARD_SIZE];
char player2_ATKboard[BOARD_SIZE][BOARD_SIZE];
int ship_sizes[NUM_SHIPS] = {5, 4, 3, 3, 2};

// Thread flags
bool p1_turn = true;
bool p2_turn = false;

int input = 0;
bool flag = 0;

// Time limit
int time_left = 30;

// Signal handler for start input
void handle_signal(int signal) 
{
    if (signal == SIGINT && flag == 0) 
    {
        if(input == 0)
        {
            printf(" - Game starting...\n");
        }
        else if(input > 0)
        {
            printf(" - Continuing...\n");
        }
        
        flag = 1;
        alarm(0);
        sleep(1);
        input++;
    }
}

// Signal handler for time limit
void handle_alarm(int signal) 
{
    time_left = 0;
}

// Function to print the board
void print_board(char board[BOARD_SIZE][BOARD_SIZE]) 
{
    printf("  ");
    for (int i = 0; i < BOARD_SIZE; i++) 
    {
        printf("%d ", i+1);
    }
    printf("\n");
    for (int i = 0; i < BOARD_SIZE; i++) 
    {
        printf("%c ", 'A'+i);
        for (int j = 0; j < BOARD_SIZE; j++) 
        {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

// Function to place ships on the board
void place_ships(char board[BOARD_SIZE][BOARD_SIZE], int ship_sizes[]) 
{
    int ship_size;
    char row;
    int col;
    char orientation;
    bool valid_position;
    
    for (int i = 0; i < NUM_SHIPS; i++) 
    {
        ship_size = ship_sizes[i];
        valid_position = false;
        while (!valid_position) 
        {
            printf("\n - Ship size: %d\n", ship_sizes[i]);
            printf(" - Enter the starting position (e.g. A1 - J10): ");
            scanf(" %c%d", &row, &col);
            row = row - 'A';
            col = col - 1;
            printf(" - Enter the orientation (H for horizontal, V for vertical): ");
            scanf(" %c", &orientation);
            if (orientation == 'H') 
            {
                if (col + ship_size <= BOARD_SIZE) 
                {
                    valid_position = true;
                    for (int j = 0; j < ship_size; j++) 
                    {
                        if (board[row][col+j] != '~') 
                        {
                            valid_position = false;
                            printf(" - There is already a ship at position %c%d.\n", 'A'+row, col+j+1);
                            break;
                        }
                    }
                    if (valid_position) 
                    {
                        for (int j = 0; j < ship_size; j++) 
                        {
                            board[row][col+j] = 'O';
                        }
                    }
                } 
                else 
                {
                    printf(" - Ship goes off the board.\n");
                }
            } 
            else if (orientation == 'V') 
            {
                if (row + ship_size <= BOARD_SIZE) 
                {
                    valid_position = true;
                    for (int j = 0; j < ship_size; j++) 
                    {
                        if (board[row+j][col] != '~') 
                        {
                            valid_position = false;
                            printf(" - There is already a ship at position %c%d.\n", 'A'+row+j, col+1);
                            break;
                        }
                    }
                    if (valid_position) 
                    {
                        for (int j = 0; j < ship_size; j++) 
                        {
                            board[row+j][col] = 'O';
                        }
                    }
                } 
                else 
                {
                    printf(" - Ship goes off the board.\n");
                }
            } 
            else 
            {
                printf(" - Invalid orientation.\n");
            }
        }
        system("clear");
        printf("----Ship Placement----\n");
        printf("Your Board:\n");
        print_board(board);
    }
    printf("\n - All ships positioned, ending turn...\n");
    sleep(3);
    system("clear");  
    flag = 0;
    printf("\n              -----SHIPS POSITIONED PRESS CTRL + C TO CONTINUE-----             \n");
    
    signal(SIGINT, handle_signal); // Register signal handler
    while (flag == 0) 
    {}
}

// Function to check if a player has won
bool check_win(char board[BOARD_SIZE][BOARD_SIZE]) 
{
    int count = 0;
    for (int i = 0; i < BOARD_SIZE; i++) 
    {
        for (int j = 0; j < BOARD_SIZE; j++) 
        {
            if (board[i][j] == 'O') 
            {
                count++;
            }
        }
    }
    return count == 0;
}

// Function to play a turn
void play_turn(char player_board[BOARD_SIZE][BOARD_SIZE], char player_ATKboard[BOARD_SIZE][BOARD_SIZE], char enemy_board[BOARD_SIZE][BOARD_SIZE], int player_num) 
{
    char row;
    int col;
    bool valid_guess = false, hit = false;
    while (!valid_guess || hit) 
    {
        printf("\n - Player %d, enter a guess (e.g. A1 - J10): ", player_num);
        scanf(" %c%d", &row, &col);
        row = row - 'A';
        col = col - 1;
        if (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE) 
        {
            if (enemy_board[row][col] == '~' && player_ATKboard[row][col] != '+') 
            {
                valid_guess = true;
                hit = false;
                time_left = 99;
                alarm(time_left);
                system("clear");
                printf("Miss...\n");
                player_ATKboard[row][col] = '+';
                printf("Your Board:\n");
                print_board(player_board);
                printf("\nEnemy Board:\n");
                print_board(player_ATKboard);
                printf("\n - You missed, ending Turn...\n");
                sleep(3);
                system("clear");
                flag = 0;
            printf("\n                 -----TURN ENDED PRESS CTRL + C TO CONTINUE-----             \n");
        signal(SIGINT, handle_signal); // Register signal handler
        while (flag == 0) 
        {}
            } 
            else if (enemy_board[row][col] == 'O') 
            {
                valid_guess = true;
                hit = true;
                time_left = 30;
                alarm(time_left);
                system("clear");
                printf("---------HIT!--------\n");
                printf("You get another shot!\n");
                player_ATKboard[row][col] = 'X';
                enemy_board[row][col] = 'X';
                print_board(player_board);
                printf("\nEnemy Board:\n");
                print_board(player_ATKboard);
                if (check_win(enemy_board)) 
                {
                    printf("\n - ALL ENEMY SHIPS DOWN! PLAYER %d  - W I N S! -\n", player_num);
                    exit(0);
                }
            } 
            else 
            {
                printf(" - You've already guessed that position.\n");
            }
        } 
        else 
        {
            printf(" - Invalid guess.\n");
        }
    }
}

// Function for player 1's turn
void *player1_turn() 
{
    while (true) 
    {
        if (p1_turn) 
        {
            system("clear");
            printf("Player 1, it's your turn.\n");
            time_left = 30;
            alarm(time_left);
            printf("Your Board:\n");
            print_board(player1_board);
            printf("\nEnemy Board:\n");
            print_board(player1_ATKboard);
            play_turn(player1_board, player1_ATKboard, player2_board, 1);
            p1_turn = false;
            p2_turn = true;
        }
        if (time_left == 0) {
            printf(" - Time's up! Player 1 Wins!\n");
            exit(0);
        }
    }
}

// Function for player 2's turn
void *player2_turn() 
{
    while (true) 
    {
        if (p2_turn) 
        {
            system("clear");
            printf("Player 2, it's your turn.\n");
            time_left = 30;
            alarm(time_left);
            printf("Your Board:\n");
            print_board(player2_board);
            printf("\nEnemy Board:\n");
            print_board(player2_ATKboard);
            play_turn(player2_board, player2_ATKboard, player1_board, 2);
            p2_turn = false;
            p1_turn = true;
        }
        if (time_left == 0) 
        {
            printf(" - Time's up! Player 2 wins!\n");
            exit(0);
        }
    }
}

// Function to initialize the game
void init_game() 
{   
    // Instructions
    printf("\n#############################-B A T T L E S H I P-##############################\n");
    printf("\n                   ---------------INSTRUCTIONS---------------\n");
    printf("\n - BOTH PLAYERS WILL CHOOSE THE POSITIONS OF THEIR FIVE SHIPS (CAPS SENSITIVE!)\n");
    printf("\n - EACH PLAYER WILL ATTEMPT TO SHOOT DOWN THE OTHER'S SHIPS GUESSING THEIR\n   POSSITIONS IN THE BOARD BEFORE THE TURN TIME RUNS OUT (30S)\n");
    printf("\n - IF A SHIP IS HIT, THE PLAYER GETS ANOTHER CHANCE TO SHOOT\n");
    printf("\n - THE FIRST PLAYER TO SHOOT DOWN ALL SHIPS WINS\n");
    printf("\n - '~' unexplored position.\n");
    printf("\n - 'X' position where a ship has been hit\n");
    printf("\n - '+' position where an attempt was made to hit a ship but failed\n");
    printf("\n - 'O' position where a friendly ship is located\n");
    printf("\n                   ---------PRESS CTRL + C TO BEGIN!---------\n");
    
    signal(SIGINT, handle_signal); // Register signal handler
    while (flag == 0) 
    {}
    
    // Initialize boards
    for (int i = 0; i < BOARD_SIZE; i++) 
    {
        for (int j = 0; j < BOARD_SIZE; j++) 
        {
            player1_board[i][j] = '~';
            player1_ATKboard[i][j] = '~';
            player2_board[i][j] = '~';
            player2_ATKboard[i][j] = '~';
        }
    }
    
    // Place ships on boards
    system("clear");
    printf("----Ship Placement----\n");
    printf("Your Board:\n");
    print_board(player1_board);
    printf("\n - Player 1, please place your ships:\n");
    place_ships(player1_board, ship_sizes);
    system("clear");
    printf("----Ship Placement----\n");
    printf("Your Board:\n");
    print_board(player2_board);
    printf("\n - Player 2, please place your ships:\n");
    place_ships(player2_board, ship_sizes);
}

int main() 
{   
    // Initialize game
    init_game();
    
    // Set up signal handler for time limit
    signal(SIGALRM, handle_alarm);
    
    // Set up threads for player turns
    pthread_t p1, p2;
    pthread_create(&p1, NULL, player1_turn, NULL);
    pthread_create(&p2, NULL, player2_turn, NULL);
    
    // Wait for threads to finish
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    return 0;
}