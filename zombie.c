#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "misc.h"

// to delay the running screen
void delay(float number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;

    // Storing start time
    clock_t start_time = clock();

    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds)
        ;
}

// clear and update the console after each move made
void board(char c[15][15], int energy, int highscore, int currentscore, char name[20], int level)
{
    int i;

    // funtion to clear the console
    system("clear");

    printf("\t\t Zombie Game\n\n");
    printf("\n\n");
    printf("\n");
    printf("\t\t Level : %d \n", level);
    printf("\n");
    printf("Name : %s \t\t\t\t\t\t HighScore : %d\n\n", name, highscore);
    printf("\t Energy : %d \t\t Current Score: %d\n\n", energy, currentscore);

    for (int i = 0; i < 14; i++)
    {

        if (i == 0)
        {
            printf(" %c | %c | %c |", c[i][0], c[i][1], c[i][2]);
            printf(" %c | %c | %c |", c[i][3], c[i][4], c[i][5]);
            printf(" %c | %c | %c |", c[i][6], c[i][7], c[i][8]);
            printf(" %c | %c | %c |", c[i][9], c[i][10], c[i][11]);
            printf(" %c | %c | %c \n", c[i][12], c[i][13], c[i][14]);
            printf("---|---|---");
            printf("|---|---");
            printf("|---|---");
            printf("|---|---");
            printf("|---|---");
            printf("|---|---");
            printf("|---|---\n");
        }
        else
        {
            printf(" %c | %c | %c |", c[i][0], c[i][1], c[i][2]);
            printf(" %c | %c | %c |", c[i][3], c[i][4], c[i][5]);
            printf(" %c | %c | %c |", c[i][6], c[i][7], c[i][8]);
            printf(" %c | %c | %c |", c[i][9], c[i][10], c[i][11]);
            printf(" %c | %c | %c \n", c[i][12], c[i][13], c[i][14]);
            printf("---|---|---");
            printf("|---|---");
            printf("|---|---");
            printf("|---|---");
            printf("|---|---");
            printf("|---|---");
            printf("|---|---\n");
            // printf("   |   |   ");
            // printf("|   |   ");
            // printf("|   |   ");
            // printf("|   |   ");
            // printf("|   |   ");
            // printf("|   |   ");
            // printf("|   |   \n");
        }
    }
    printf(" %c | %c | %c |", c[14][0], c[14][1], c[14][2]);
    printf(" %c | %c | %c |", c[14][3], c[14][4], c[14][5]);
    printf(" %c | %c | %c |", c[14][6], c[14][7], c[14][8]);
    printf(" %c | %c | %c |", c[14][9], c[14][10], c[14][11]);
    printf(" %c | %c | %c \n", c[14][12], c[14][13], c[14][14]);
}

// displays instructions and rules of the game
void rulesandinst()
{
    printf("\n\n\t\t\t\t\t\t\t Zombie Game\n\n");
    delay(0.5);
    printf("Welcome to the best 2-D survival game in C\n\n");
    delay(0.5);

    // #instructions of the game
    printf("Instructions:-\n");
    delay(0.5);
    printf("\n\n1:'O' represents you the last human alive on this planet and 'X' represents zombies longing to kill the human");

    printf("\n\n2:Your task is to keep yourself alive as long as you can");
    printf("\n\n3:Zombies will keep following you moving one step at a time");
    delay(1);

    // #rules of the game
    printf("\n\nRules:-");
    delay(0.5);
    printf("\n\n1:You will gain 2 energy per move. \t Note:You can have a maximum of 8 energy at a time");

    printf("\n\n2:You can use 1 energy to move to a adjacent block  or  use all your energy to unleash a attack that kills zombie in a square of 5*5 with you as center");
    printf("\n\n3:You need atleast 6 energy to unleash your attack");
    printf("\n\n4:You can also stay in the same place and save energy");

    printf("\n\n5:Once a zombie comes in contact with you that is a zombie is present in an adjacent block to you YOU DIE!!!!");
    delay(1);
    printf("\n\nSURVIVE AS LONG AS YOU CAN!!!!!!!\n\n");
    printf("To start the game enter Y  ");

    char tp;
    scanf("%c", &tp);
}

// Backstory of our game
void backstory()
{
    printf("Hello... \t");
    delay(1.5);
    printf("Hello.. \t");
    delay(1.5);
    printf("Is anyone there? \n\n");
    delay(2);
    printf("I walk closer to this weird looking man \n\n");
    delay(2);
    printf("Its dark.. ");
    delay(1.5);
    printf("street lights flicker...");
    delay(1.5);
    printf("I cant see his face clearly\n\n");
    delay(1.5);
    printf("Hey(I walk closer) \n\n");
    delay(1.5);
    printf("After everyone disappeared i thought i will not find anyone but you.... \n\n");
    delay(1.5);
    printf("Hey... \n");
    delay(1.5);
    printf("BRHHHH RHHHHHH SEHHHHH...");
    delay(3);
}
// main function of the game
void main_zombie(void)
{
    int maxscore[7];
    for (int i = 0; i < 7; i++)
    {
        maxscore[i] = 0;
    }

    system("color 04");
    backstory();
    system("clear");

    system("color 05");
    char board_char[15][15];
    int highscore = 0; // initialising high score of each level
    int curscore = 0;  // initaliising  current score of each level
    rulesandinst();

// starting the game here
game:
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            board_char[i][j] = '.';
        }
    }
    int k, khash; // here khash represents the level of the game
    printf("Enter your name  ");
    char st[20];
    scanf("%s", st); // Asking name of player
    printf("\nChoose Difficulty level 1-6 (note greater number means more difficulty) \n");
    scanf("%d", &khash); // Asking Difficulty level
    k = 7 - khash;

    highscore = maxscore[khash - 1];

    int spawncount = 1; // count of the move
    int energy = 1;     // energy of player

    // Initailising the intial zombies
    board_char[0][0] = 'X';
    board_char[0][14] = 'X';
    board_char[14][0] = 'X';
    board_char[14][14] = 'X';
    board_char[7][7] = 'O';

    int x = 7, y = 7; // keeps position of player

    system("color 09");
    board(board_char, energy, highscore, curscore, st, khash);
    bool check = true;

    // making changes in the board after every move
    while (check)
    {
        // Provides an option to move, attack or save energy
        printf("Press 1 to move, 2 to attack, 3 to save energy\n");
        energy+=2; // Adds energy because per move 2 energy is promised
       

        int r;
        scanf("%d", &r);

        if (r == 1)
        {
            energy--; // moving takes one energy
            // Askig directions for movement
            printf("Which direction would you like to move?\n");
            printf("Press 'w' for Up \tPress 'a' for Left \tPress 's' for Down \tPress 'd' for Right \t\n");
            char ch;
            scanf(" %c", &ch);
            // changing position of player according to choice of player
            if (ch == 'a')
            {
                board_char[x][y] = '.';
                y = max(y - 1, 0);
                board_char[x][y] = 'O';
            }
            if (ch == 'w')
            {
                board_char[x][y] = '.';
                x = max(x - 1, 0);
                board_char[x][y] = 'O';
            }
            if (ch == 'd')
            {
                board_char[x][y] = '.';
                y = min(y + 1, 19);
                board_char[x][y] = 'O';
            }
            if (ch == 's')
            {
                board_char[x][y] = '.';
                x = min(x + 1, 19);
                board_char[x][y] = 'O';
            }
        }
        // if energy required is not sufficient for attack
        else if (r == 2 && energy < 6)
        {
            // Telling player energy is not sufficient and reasking what to do
            energy-=2; // no move happened so deducting the 2 energy which we added earlier
            board(board_char, energy, highscore, curscore, st, khash);
            printf("\n");
            printf("\n");
            printf("\n");
            printf("\n");
            printf("Energy not sufficient\n");
            continue;
        }
        // if player chooses attack and condition satisfies
        else if (r == 2 && energy >= 6)
        {
            printf("%d", energy);
            energy -= 8;
            
            for (int i = max(x - 2, 0); i <= min(x + 2, 14); i++)
            {
                for (int j = max(y - 2, 0); j <= min(y + 2, 14); j++)
                {
                    board_char[i][j] = '.';
                }
            }

            board_char[x][y] = 'O';
        }
        else if (r == 3)
        {
            ; // do nothing save energy
        }

        // make sure energy doesn't go out of bounds
        energy = max(energy, 0);
        energy = min(energy,8); 
        // making a temporary grid to store movement of zombies
        int board_char_temp[15][15];
        for (int i = 0; i < 15; i++)
        {
            for (int j = 0; j < 15; j++)
            {
                if (board_char[i][j] == 'O')
                {
                    board_char_temp[i][j] = 'O';
                }
                else
                {
                    board_char_temp[i][j] = '.';
                }
            }
        }

        // making zombies move so they always follow the person
        for (int i = 0; i < 15; i++)
        {
            for (int j = 0; j < 15; j++)
            {
                if (board_char[i][j] == 'X')
                {
                    if (i == x)
                    {
                        if (j > y)
                        {
                            board_char_temp[i][j - 1] = 'X';
                        }
                        else
                        {
                            board_char_temp[i][j + 1] = 'X';
                        }
                    }
                    else if (j == y)
                    {
                        if (i > x)
                        {
                            board_char_temp[i - 1][j] = 'X';
                        }
                        else
                        {
                            board_char_temp[i + 1][j] = 'X';
                        }
                    }
                    else
                    {
                        if (i > x)
                        {
                            if (j > y)
                            {
                                board_char_temp[i - 1][j - 1] = 'X';
                            }
                            else
                            {
                                board_char_temp[i - 1][j + 1] = 'X';
                            }
                        }
                        else
                        {
                            if (j > y)
                            {
                                board_char_temp[i + 1][j - 1] = 'X';
                            }
                            else
                            {
                                board_char_temp[i + 1][j + 1] = 'X';
                            }
                        }
                    }
                }
            }
        }

        // updating our main grid with help of temporary grid

        for (int i = 0; i < 15; i++)
        {
            for (int j = 0; j < 15; j++)
            {
                board_char[i][j] = board_char_temp[i][j];
            }
        }

        // Spawning new zombies
        if (spawncount % k == 0)
        {
            board_char[0][0] = 'X';
            board_char[14][14] = 'X';
            if (spawncount % (2 * k) == 0)
            {
                board_char[0][14] = 'X';
                board_char[14][0] = 'X';
            }
        }
        if (spawncount % (k + 2) == 0)
        {
            board_char[7][0] = 'X';
            board_char[14][7] = 'X';
            if (spawncount % (2 * (k+2)) == 0)
            {
                board_char[0][7] = 'X';
                board_char[7][14] = 'X';
            }
        }
        spawncount++;

        // Checking for game end
        for (int i = max(x - 1, 0); i <= min(x + 1, 14); i++)
        {
            for (int j = max(y - 1, 0); j <= min(y + 1, 14); j++)
            {
                if (board_char[i][j] == 'X')
                {
                    check = false;
                    board_char[x][y] = 'O';
                    break;
                }
            }
            if (!check)
            {
                break;
            }
        }

        // updating current score
        curscore++;
        // printing grid everytime
        board(board_char, energy, highscore, curscore, st, khash);
        // printf("1");
    }

    printf("\n\n");
    // After game ends
    printf("GAME OVER!!!!!! \n");
    // updating highscore
    maxscore[khash - 1] = max(curscore, maxscore[khash - 1]);

    // setting current score to 0
    curscore = 0;

    // Restarting the game
    printf("Enter y to restart the game or n to end the game ");
    char re;
    scanf(" %c", &re);
    if (re != 'n')
    {
        goto game;
    }
}
