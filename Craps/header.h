/*
Author: Joshua Wallace
Date: 10-2-2022
Course: CptS 121 Section 02; Lab Section 18L
Description: Plays craps with a vitural dealer! (He is very temperamental...)
*/

#define _CRT_SECURE_NO_WARNINGS // Ignores ScanF warning.

//Defining a ton of macros. Most of these are unneeded, but it makes future code more intuitive to read
#define RULES 1
#define PLAY 2
#define EXIT 3

#define INVALID -1
#define VALID 1

#define FALSE 0
#define TRUE 1

#define WIN 1
#define LOSE 2
#define POINT 3

#define NO 0
#define YES 1

//Defined a macro here for the wait time after the dealer sent a message.
#define WAITTIME 700000

//A bunch of libraries.
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

void sleep_type(int loops);
void print_menu(void);
int get_choice(void);
void process_choice(int choice);
void print_game_rules (void);
double get_bank_balance (void);
double get_wager_amount (double balance);
int wager_chatter(double balance, double wager, int broke);
int roll_die (void);
int calculate_sum_dice (int die1_value, int die2_value);
void print_sum_dice (int die1_value, int die2_value, int dice_sum);
int user_roll(void);
int is_win_loss_or_point (int sum_dice);
int is_point_loss_or_neither (int sum_dice);
void broke_check(int broke);
int get_play_again(int result, double balance);
double adjust_balance(double balance, double wager, int win_or_lose);
void print_final_balance(double balance);