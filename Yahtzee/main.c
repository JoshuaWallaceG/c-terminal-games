#include "header.h"

int main(void){
    srand((unsigned int)time(NULL)); //Setting up TIME for rand()
    int playAgain = YES;
    do{
    clear_screen();//Gets rid of the inital terminal junk + clears at each new game
    //Declaring variables INSIDE the loop. Skips setitng them all to 0 at the end of each roll.
    int choice = 0, turn = 1, winner = 0, reroll = 0, playerSwap = 0;
    int dice[5] = {0,0,0,0,0};
    int dice_freq[6] = {0,0,0,0,0,0};

    //Delcaring p1 and p2 score cards. Only setting the first value to the corresponding player nubmer. (p1 = 1, p2 = 2)
    int p1_score_card[20] = {1};
    for(int i = 1; i < 14; i++){p1_score_card[i] = UNMARKED;} //Setting all SELECTABLE values to unmarked (-1)
    int p2_score_card[20] = {2};
    for(int i = 1; i < 14; i++){p2_score_card[i] = UNMARKED;}

    //Loops until anything but RULES is selected (Option 3 simply exits game, so no need to check for it.)
    do{
    choice = get_choice();
    }while(choice == RULES);  
    
    do{ //The entire game is in a giant loop)
    reroll = NO;
    switch(playerSwap % 2 + 1){ //Swapping based on playerSwap mod 2 + 1
        case 1: print_score_card(p1_score_card);
        break;
        case 2: print_score_card(p2_score_card);
        break;
    }

    do{//The main dice loop
        if(reroll == YES){//Which dice to reroll is asked on all iterations after first
            ask_which_reroll(dice);}
        ask_roll();
        roll_dice(dice);
        print_dice(dice);
        if(turn < 3){//Only asks if dice have been rerolled less than 3 times
            reroll = ask_reroll();}
        ++turn;
    }while(turn < 4 && reroll == YES); //Keeps looping until they are happy w/ roll, or are out of rolls

    turn = 1;//Resetting turn value

    find_dice_freq(dice, dice_freq); //Finds dice freq. Is used for future functions

    switch(playerSwap % 2 + 1){//Does all the scorecard asks/checks/updates depending on whos roll it is.
        case 1:
            choice = get_score_choice(p1_score_card, dice, dice_freq);
            assign_score_to_card(p1_score_card, dice, choice);
            update_score_card_totals(p1_score_card);
            determine_if_full(p1_score_card);
            if(p2_score_card[ISFULL] == YES){//Checks if the next player is full. (Yahtzee bonus makess this possible)
                ++playerSwap;//Adds another playerswap if the next player is full, effectivly skipping the full player
            }
            break;
        case 2:
            choice = get_score_choice(p2_score_card, dice, dice_freq);
            assign_score_to_card(p2_score_card, dice, choice);
            update_score_card_totals(p2_score_card);
            determine_if_full(p2_score_card);
            if(p1_score_card[ISFULL] == YES){
                ++playerSwap;
            }
            break;
    } 
    //Preparing dice and screen for next round, and adding +1 to playerswap var
    clear_screen();
    clear_dice(dice);
    ++playerSwap;
    }while(p1_score_card[ISFULL] == NO || p2_score_card[ISFULL] == NO); // End of main game loop

    //Determining/printing the winner message, and asking to play again.
    winner = determine_winner(p1_score_card, p2_score_card);
    print_winner(winner, p1_score_card, p2_score_card);
    playAgain = ask_play_again();
    }while(playAgain == YES);//Loop that allows the game to be in a giant loop.

    return 0;
}