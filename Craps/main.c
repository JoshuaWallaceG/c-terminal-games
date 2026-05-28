#include "header.h"

int main(void){
    int choice = 0, dice_sum = 0, result = 0, broke = FALSE;
    double balance = 0, wager = 0;
    
    srand((unsigned int)time(NULL)); //Seeding the rand() function with time, which lets me get random dice rolls
    //Worked with my CS friend (Aaron Lieberman) to get rid of a few warnings. I learned that unsigned may be better for time (no negatives, double the positives) 

    do{
    choice = get_choice();
    }while(choice == RULES); //The only option that causes a repeat is RULES, so I only check for RULES.
    
    balance = get_bank_balance(); //Stays on the outside, so the player cant change their balance mid game
    
    do{
        wager = get_wager_amount(balance);
        broke = wager_chatter(balance, wager, broke);
        dice_sum = user_roll();
        result = is_win_loss_or_point(dice_sum);
        if(result == POINT){
            result = is_point_loss_or_neither(dice_sum);
        }
        balance = adjust_balance(balance, wager, result);
        broke_check(broke);
        choice = get_play_again(result, balance);
    }while(choice == YES); //The entire game is in a loop.

    print_final_balance(balance); //Prints final balance
    return 0;
}