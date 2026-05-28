/*
Author: Joshua Wallace
Date: 11-28-2022
Course: CptS 121 Section 02; Lab Section 18L
Description: Plays a game of poker with the dealer!
*/

//Defining macros for suits
#define HEART "\u2665"
#define DIAMOND "\u2666"
#define CLUB "\u2663"
#define SPADE "\u2660"

#define TOPLEFTBOR "Dx2081A"
#define TOPRIGHTBOR "Bx176F"
#define BOTLEFTBOR "Cx1920"
#define BOTRIGHTBOR "Dx2089"

//Defining macros for colors
#define RED "\x1B[31m"
#define CYN   "\x1B[36m"
#define RESET "\x1B[0m"

//Defining macros for readibility
#define PLAY 2
#define UNDRAWN -1

#define NOTHING 0
#define PAIR 1
#define TWOPAIR 2
#define THREEOFKIND 3
#define STRAIGHT 4
#define FLUSH 5
#define FULLHOUSE 6
#define FOUROFKIND 7

//Libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

//Card struct
typedef struct card{
    int faceIndex;
    int valueIndex;
    bool faceDown; //Not asked for but makes things eaiser (Andy approved)
} Card;

typedef struct hand{
    Card current_cards[5];
    int handEval; //Not asked for but makes things easier (Andy approved)
} Hand;

// Authors: Deitel & Deitel - C How to Program
void shuffle (int wDeck[][13]);
void deal (int wDeck[][13], const char *wFace[], const char *wSuit[], Hand *inputHand, int cardsToDraw);


//Toolbox-functions
void clear_screen(void);
void clear_input(char input[]);
void flush_input_buffer(char input[]);
void prompt_enter(void);
void force_hand(Hand *inputHand, int requestedHand, int Deck[][13], const char *Face[], const char *Suit[]);

//Menu functions
void print_menu(void);
void print_game_rules (void);
int get_choice(char input[]);
void execute_choice(int choice);

//Prep functions
void clear_deck (int wDeck[][13]);
void init_hand(Hand *inputHand);
void flip_cards(Hand *inputHand);

//Evaluation/Discard functions
void print_hand(const char *wFace[], const char *wSuit[], Hand *inputHand);
void evaluate_hand(Hand *inputHand);
int determine_discard(Hand *inputHand);
void prompt_redraw(Hand *inputHand, char input[]);
bool validate_redraw_input(char input[]);
int discard_inputted_cards(Hand *inputHand, char input[]);

//Wrap up functions
int determine_winner(int player_hand_value, int dealer_hand_value);
void print_winner(int winner);
bool get_play_again(char input[]);