/*
Author: Joshua Wallace
Date: 10-25-2022
Course: CptS 121 Section 02; Lab Section 18L
Description: Plays a 2 player game of yahtzee!
*/

#define _CRT_SECURE_NO_WARNINGS // Ignores ScanF warning.

//Defining macros for each value in the score_card[] array(s) Makes code WAY eaiser to read and write.
//Kept position 1-13 as correspondieg to the choice (yahtzee = 12, and is choice 12). Allows me to use the choice to assign it to the value itself
#define PLAYER 0
#define ACES 1
#define TWOS 2
#define THREES 3
#define FOURS 4
#define FIVES 5
#define SIXES 6
#define UPPER_BONUS 15

#define THREEOFKIND 7
#define FOUROFKIND 8
#define FULLHOUSE 9
#define SMSTRAIGHT 10
#define LGSTRAIGHT 11
#define YAHTZEE 12
#define YAHTZEE_BONUS 14
#define CHANCE 13

#define UPPER_TOTAL 16
#define LOWER_TOTAL 17
#define GRAND_TOTAL 18
#define ISFULL 19

#define RULES 1
#define PLAY 2
#define EXIT 3

#define SCRATCH 0 //"scratch" is what I call writing in a 0 for something (Selecting full house when a full house isnt rolled)
#define UNMARKED -1//"Unmarked" is just when no value has been written in

#define INVALID -2
#define VALID 1

#define FALSE -2
#define TRUE 1

#define NO 0
#define YES 1

//libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void print_menu(void);
int get_choice(void);
void print_game_rules (void);
void clear_screen(void);
void print_score_card(int score_card[]);
int ask_reroll(void);
void ask_which_reroll(int dice[]);
void ask_roll(void);
void roll_dice(int dice[]);
void print_dice(int dice[]);
void clear_dice(int dice[]);
void clear_prev_lines(int lines);
int get_score_choice(int score_card[], int dice[], int dice_freq[]);
void print_array(int array[], int size);
void assign_score_to_card(int p1_score_card[], int dice[], int choice);
void find_dice_freq(int dice[], int dice_freq[]);
void print_score_or_blank(int choice);
void update_score_card_totals(int score_card[]);
int yahtzee_or_bonus(int score_card[], int dice[], int dice_freq[], int choice, int isYahtzee);
void determine_if_full(int score_card[]);
int determine_winner(int p1_score_card[], int p2_score_card[]);
void print_winner(int winner, int p1_score_card[], int p2_score_card[]);
int ask_play_again(void);