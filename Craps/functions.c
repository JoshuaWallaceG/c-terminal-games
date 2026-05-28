#include "header.h"

//Making a fake typing loop by repeatidly replacing the typing text.
void sleep_type(int loops){
    for(int i = 0; i < loops; i++){
    printf("Dealer is typing.\n");
    usleep(150000); //Waits for () miliseconds (1500 in this case)
    printf("\x1b[1F"); //Goes up one line in terminal
    printf("\x1b[2K"); //Clears current line in terminal
    printf("Dealer is typing..\n");
    usleep(150000);
    printf("\x1b[1F");
    printf("\x1b[2K");
    printf("Dealer is typing...\n");
    usleep(150000);
    printf("\x1b[1F");
    printf("\x1b[2K");
    }
}

void print_menu(void){
    printf("---------------------------\n");
    printf("1. Display rules.\n");
    printf("2. Play Craps.\n");
    printf("3. Exit.\n");
    printf("---------------------------\n");
    return;
}

//Anytime that the dealer "types", I call the "sleep_type" function with a number for the loops argument 
//I mainly eyeball how much I want it to loop
void print_game_rules (void){
    sleep_type(6);
    printf("\nOkay, here's the rules. First, you roll two dice.\n");
    printf("If you roll a 7 or a 11 on the first roll of your turn, congrats! You win the round.\n");
    printf("If you roll a 2, 3, or 12 on the first roll of your turn, thats called craps, and you're out of luck. House wins.\n");
    printf("If you roll anything else on the first roll of your turn, thats now your point.\n");
    printf("Now you gotta keep rerolling until you roll your point again and win, or you roll a 7 and lose.\n\n");
    usleep(WAITTIME);
    sleep_type(2);
    printf("So, you ready to play?\n");
return;
}

//Using a combination of input vaildator (while(choice == INVALID) and switch statemends for readibility.
//The case of PLAY isnt needed, but it allows me to see if something goes wrong and we get out of our switch statement
int get_choice(void){
    int choice = 0;
    do{
        print_menu();
        scanf("%d", &choice);
        if(choice < RULES || choice > EXIT){
            choice = INVALID;
    }
    }while(choice == INVALID);

    switch(choice){
    case RULES: print_game_rules();
        break;
    case PLAY:
        break;
    case EXIT: 
        sleep_type(2);
        printf("Bye bye!\n");
        exit(1);
    default: printf("HOW THE HELL DID WE GET HERE\n");
        break;
    }
    return choice;
}

//Added some chatter. I made the "dealer" a bit tense, so screwing with him usually ends up having the program closed
double get_bank_balance(void){
    double balance = 0;
    sleep_type(3);
    printf("Alright, how much cash you got in the bank?\n");
    scanf("%lf", &balance);
    if(balance < 0){
    sleep_type(3);
        printf("You're in debt at a casino? Why are you still here? Just get out.\n");
        exit(0);
    }
    return balance;
}

//Another while invalid loop. Added chatter within the function for some personality.
double get_wager_amount (double balance){
    double wager = 0;
    sleep_type(3);
    printf("How much cash you wanna bet this turn? You have $%.2lf in the bank.\n", balance);
    do{
    scanf("%lf", &wager);
    if(wager > balance){
        sleep_type(2);
        printf("Hey buddy! You don't have the cash for that bet. Try again.\n");
        wager = INVALID;}
    else if(wager < 0){
        sleep_type(2);
        printf("Buddy are you dumb? You can't bet a negative value. Try again.\n");
        wager = INVALID;
    }
    }while (wager == INVALID);
    return wager;
}

//Simply comments on how much you are betting. Used IF and switch statements for readibility
int wager_chatter(double balance, double wager, int broke){
    int chatter = 0;
    if(wager == balance && wager == 0.0){
        chatter = 1;
        }
    else if(wager == balance){
        chatter = 2;
        }
    else if(wager == 0.0){
        chatter = 3;
        }

    switch(chatter){
        case 1:
            sleep_type(2);
            printf("No money AND no wagers? Alright, you get 1 more roll for fun.\n");
            usleep(WAITTIME);
            broke = TRUE; //This comes into play later in broke_check().
            break;
        case 2:
            sleep_type(3);
            printf("Ha! All in? That's the spirit. Your bet is locked in at $%.2lf. Lets play craps!\n", wager);
            usleep(WAITTIME);
            break;
        case 3:
            sleep_type(3);
            printf("Aw no bet? What are you, scared? Fine, your bet is locked in at $%.2lf. Lets play craps...\n", wager);
            usleep(WAITTIME);
            break;
        default:
            sleep_type(2);
            printf("Alright, your bet is locked in at $%.2lf. Lets play craps!\n", wager);
            usleep(WAITTIME);
            break;
    }
    return broke;
}

//Rolling dice using mod 6, then adding 1.
int roll_die(void){
    int die = 0;
    die = rand() % 6 + 1;
    return die;
}

int calculate_sum_dice (int die1_value, int die2_value){
    int dice_sum = 0;
    dice_sum = die1_value + die2_value;
    return dice_sum;
}

//Printing the result of dice for the user and adding them up.
void print_sum_dice (int die1_value, int die2_value, int dice_sum){
    sleep_type(3);
    printf("Looks like you rolled a %d and a %d. Thats %d.\n", die1_value, die2_value, dice_sum);
    usleep(WAITTIME);
}

//Uses while input invalid loop
//I made the dealer kick you out of the program if you dont roll correctly. He is pertty temperamental
int user_roll(void){
    char roll = '\0';
    int die1 = 0, die2 = 0, dice_sum = 0, misroll = 0;
    sleep_type(1);
    printf("Just type 'R' to roll the dice!\n");
    do{
    scanf(" %c", &roll);
    printf("\n");
    //Placing the IF that kicks the user out BEFORE the IF that adds +1 to the misroll counter. 
    if(roll != 'r' && roll != 'R' && misroll == 2){
        sleep_type(2);
        printf("Alright, now you're just screwing around. Get out of my casino.\n");
        exit(0);
    }
    else if(roll != 'r' && roll != 'R' && misroll < 2){
        sleep_type(1);
        printf("What the hell was that? Try again...\n");
        roll = INVALID;
        misroll = misroll + 1;
    }
    
    }while(roll == INVALID);

    die1 = roll_die();
    die2 = roll_die();
    dice_sum = calculate_sum_dice(die1, die2);
    print_sum_dice(die1, die2, dice_sum);
    return dice_sum;
}

//Another IF and switch statement print system. Returns 1/3 total options (WIN, LOSE, or POINT) in macro form.
int is_win_loss_or_point (int dice_sum){
    int result = 0;
    if(dice_sum == 7 || dice_sum == 11){
        result = WIN;
        }
    else if(dice_sum == 2 || dice_sum == 3 || dice_sum == 12){
        result = LOSE;
        }
    else{
        result = POINT;
        }
    
    switch(result){
        case WIN: 
            sleep_type(3);
            printf("You got the magic touch. You're a winner!\n");
            usleep(WAITTIME);
            break;
        case LOSE: 
            sleep_type(3);
            printf("Ooooo tough luck bud... Thats craps.\n");
            usleep(WAITTIME);
            break;
        case POINT: 
            sleep_type(3);
            printf("I guess we roll for point!\n");
            usleep(WAITTIME);
            break;
        }
    return result;
}

//Similar loop as used above, but changed the text to make more sense.
int is_point_loss_or_neither (int sum_dice){
    int result = 0, point_value = 0, roll_count = 1, chatter = 0; //Setting roll count to 1, as the player would have already rolled once at this point.
    point_value = sum_dice;
    do{
    sum_dice = user_roll();
    roll_count = roll_count + 1;
    }while(sum_dice != point_value && sum_dice != 7);
    if(sum_dice == 7){
        sleep_type(3);
        printf("Ouchhh, tough luck bud... Thats craps. You had me worried for a second!\n");
        usleep(WAITTIME);
        if(roll_count > 5){
            sleep_type(3);
            printf("Your turn took %d rolls and you still lost? Even I feel a little bad...\n", roll_count);
            usleep(WAITTIME);
        }
        result = LOSE;
    }
    else{
        sleep_type(3);
        printf("You rolled your point. You're a winner!\n"); 
        usleep(WAITTIME);
        if(roll_count > 5){
            sleep_type(3);
            printf("Took you long enough though... What was that, %d rolls? Jeez.\n", roll_count);
            usleep(WAITTIME);
        }
        result = WIN;
        
    }
    return result;
}

//Checks if the dealer already noticed they are broke. This adds a lose condition as well.
void broke_check(int broke){
    if(broke == TRUE){
        sleep_type(3);
        printf("Hope you had fun, but I told you that was the last roll. You gotta go.\n");
        exit(0);
    }
}

//Switch statement for two different "play again?" printfs
//Another while choice == invalid loop.
int get_play_again(int result, double balance){
    char choice = '\0';
    switch(result){
    case WIN: 
        sleep_type(3);
        printf("Good game! Looks like you won this one. You are at $%.2lf right now. Want to play again? (Y or N)\n", balance);
        break;
    case LOSE: 
        sleep_type(3);
        printf("Sorry, but the house wins this one. You are at $%.2lf right now. Want to play again? (Y or N)\n", balance);
        break;
    }
    do{
        scanf(" %c", &choice);
        if(choice != 'Y' && choice != 'y' && choice != 'N' && choice != 'n'){
        sleep_type(1);
        printf("Whatcha say? (Y or N)\n");
        choice = INVALID;
        }
    }while(choice == INVALID);

    if(choice == 'Y' || choice == 'y'){
        choice = YES;
    }
    else{
        choice = NO;
    };
    return choice;
}

double adjust_balance(double balance, double wager, int win_or_lose){
    switch(win_or_lose){
        case WIN: balance = balance + wager;
        break;
        case LOSE: balance = balance - wager;
        break;
    }
    return balance;
}

void print_final_balance(double balance){
    sleep_type(2);
    printf("You ended this game with $%.2lf. Come again!\n", balance);
}