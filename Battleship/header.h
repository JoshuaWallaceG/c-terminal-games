/*
Author: Joshua Wallace
Date: 11-01-2022
Course: CptS 121 Section 02; Lab Section 18L
Description: Plays a 2 player game of Battleship with a computer! (Now with colors!)
*/

//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

//Allows me to use fun colors for the boards and text
//Full credit to this comment chain: https://stackoverflow.com/questions/3585846/color-text-in-terminal-applications-in-unix
#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define BLU "\x1B[34m"
#define RESET "\x1B[0m"

#define _CRT_SECURE_NO_WARNINGS // Ignores ScanF warning.

//Ship struct which stores all the information on each type of ship
typedef struct ship{
    char name[50];
    char letter;
    int size;
} Ship;

//Playerboards struct (mainly to avoid using 3D arrays)
//Was planning to add customizable names, which was the inital reason
typedef struct playerboards{
    char board[10][10];
} PlayerBoards;

//Statistic struct
typedef struct stats{
    int hits;
    int misses;
    int shots;
    double hitsToMiss; //Hit to Miss ratio
    double hitsToShots; //Hit to Shop percentage
    bool won;
} Stats;

//I try to have very simple "toobox" functions that make the rest of the functions eaiser to use.

//Toolbox-functions
void clear_screen(void);
void clear_prev_lines(int lines);
void clear_input(char input[]);
void flush_input_buffer(char input[], int expectedNewLinePos);
void prompt_enter(void);
//Function-functions
void init_board (char board[10][10], int rows, int cols);
void print_ship_board (char board[10][10], int rows, int cols);
void print_result_board (char board[10][10], int rows, int cols);
char prompt_rand_or_man(char input[]);
void prompt_place(char input[], Ship shipArray[], char board[10][10], int i);
void print_menu(void);
int validate_cell_input(char input[], char board[10][10], int size, int desiredLength);
void print_input_error(int errorType);
void update_ship_placement(char input[], Ship shipArray[], char board[10][10], int i);
void rand_place(char board[10][10], Ship ship_array[]);
void prompt_target(char input[], char board[10][10]);
int validate_target_input(char input[], char board[10][10]);
void rand_target(char input[], char board[10][10]);
char check_hit_or_miss(char input[], char board[10][10]);
void update_hit_or_miss(char input[], char board[10][10], char result);
int check_if_sunk(char input[], char board[10][10]);
void print_hit_or_miss(char input[], char result);
void print_if_sunk(int sunkenShip, Ship shipArray[]);
bool check_if_winner(char board[10][10]);
void update_stats(Stats *player, char result, bool won);
void print_turn_to_file(FILE *output_stream, char input[], char result, int sunkenShip, Ship shipArray[], int currentPlayer, int nextPlayer);
void print_stats_to_file(FILE *output_stream, Stats playerStats[], int winner);