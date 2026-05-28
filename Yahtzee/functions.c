#include "header.h"

void clear_prev_lines(int lines){//Alows me to clear just the right amount of lines to keep the screen clean
    for(int i = 0; i < lines; ++i){
    printf("\x1b[1F");
    printf("\x1b[2K");
    }
}

void print_array(int array[], int size){ //Mainly used for tesing purposes. Could be removed, but is nice to have
    for (int i = 0; i < size; i++) {printf("%d ", array[i]);}
    printf("\n");
}

void print_menu(void){
    printf("---------------------------\n");
    printf("1. Display rules.\n");
    printf("2. Play YAHTZEE!\n");
    printf("3. Exit.\n");
    printf("---------------------------\n");
    return;
}

int get_choice(void){//A loop to get the choice for the menu
    int choice = 0;
    do{
        print_menu();
        scanf("%d", &choice);
        clear_screen();
        if(choice < RULES || choice > EXIT){
            choice = INVALID;
    }
    }while(choice == INVALID);

    switch(choice){
    case RULES: 
        print_game_rules();
        break;
    case PLAY:
        break;
    case EXIT: 
        printf("Bye bye!\n");
        exit(1);
    default: printf("HOW THE HELL DID WE GET HERE\n");//lol
        break;
    }
    return choice;
}

void print_game_rules (void){
    printf("Each turn, roll up to 3 times each turn to rack up the best possible score.\n");
    printf("Then, decide which dice combo you're going for.\n");
    printf("After each turn, write your score in one empty box on the score card.\n");
    printf("If your roll doesn't apply to one of the options, you may still select it and 'scratch' it with a 0.\n");
    printf("After each turn, the players alternate. They will alternate until both players have filled up their cards.\n");
    printf("If a player has filled out their card before the other player, then that player will keep rerolling until all cards are filled.\n");
    printf("(This can happen due to a player scoring BONUS YAHTZEE's)\n");
}

//I put system("cls") in a function because on windows, it is "cls" and macOS, it is system("clear").
//Using a function makes it easy to just change it in 1 place when testing on other platforms
void clear_screen(void){
    system("clear");
}

void print_score_or_blank(int choice){//Prints a blank character if the value is unmarked (-1), or the value of the choice in that position
    char blank = '-';
    if(choice == UNMARKED){
        printf("%c\n", blank);} 
    else{
        printf("%d\n", choice);}
}

//Very ugly function, but formatting like this allowed met to get the spacing correct w/o trial and error
void print_score_card(int score_card[]){
    char blank = '-';
    printf("--------PLAYER %d'S SCORECARD--------\n", score_card[PLAYER]);
    printf(" -----------UPPER SECTION----------- \n");
    printf("[1]  ACES     (Sum of 1's)          "); print_score_or_blank(score_card[ACES]);
    printf("[2]  TWOS     (Sum of 2's)          "); print_score_or_blank(score_card[TWOS]);
    printf("[3]  THREES   (Sum of 3's)          "); print_score_or_blank(score_card[THREES]);
    printf("[4]  FOURS    (Sum of 4's)          "); print_score_or_blank(score_card[FOURS]);
    printf("[5]  FIVES    (Sum of 5's)          "); print_score_or_blank(score_card[FIVES]);
    printf("[6]  SIXS     (Sum of 6's)          "); print_score_or_blank(score_card[SIXES]);
    printf("UPPER BONUS   (35 if Upper > 63)    "); printf("%d\n", score_card[UPPER_BONUS]);
    printf("UPPER TOTAL                         "); printf("%d\n", score_card[UPPER_TOTAL]);
    printf(" -----------LOWER SECTION----------- \n");
    printf("[7]  3KIND    (Sum of all dice)     "); print_score_or_blank(score_card[THREEOFKIND]);
    printf("[8]  4KIND    (Sum of all dice)     "); print_score_or_blank(score_card[FOUROFKIND]);
    printf("[9]  FULL H.  (25 points)           "); print_score_or_blank(score_card[FULLHOUSE]);
    printf("[10] SM. STR  (30 points)           "); print_score_or_blank(score_card[SMSTRAIGHT]);
    printf("[11] LG. STR  (40 points)           "); print_score_or_blank(score_card[LGSTRAIGHT]);
    printf("[12] YAHTZEE  (50 points)           "); print_score_or_blank(score_card[YAHTZEE]);
    printf("[13] CHANCE   (Sum of all dice)     "); print_score_or_blank(score_card[CHANCE]);
    printf("YAHTZEE BONUS (100 points/per)      "); printf("%d\n", score_card[YAHTZEE_BONUS]);
    printf("LOWER TOTAL                         "); printf("%d\n", score_card[LOWER_TOTAL]);
    printf(" --------------TOTALS--------------  \n");
    printf("UPPER TOTAL:                        "); printf("%d\n", score_card[UPPER_TOTAL]);
    printf("LOWER TOTAL:                        "); printf("%d\n", score_card[LOWER_TOTAL]);
    printf("GRAND TOTAL:                        "); printf("%d\n", score_card[GRAND_TOTAL]);
    printf("-------------------------------------\n");
}

void roll_dice(int dice[]){//Simple roll dice that rerolls any dice that are set to 0
    for(int i = 0; i < 5; i++){
        if(dice[i] == 0){
            dice[i] = rand() % 6 + 1;}
    }
}

void print_dice(int dice[]){//Priting dice with little fake "dice" boxes.
    printf("Your roll:\n");
    printf("---  ---  ---  ---  ---\n");
    printf("|%d|  |%d|  |%d|  |%d|  |%d|\n", dice[0], dice[1], dice[2], dice[3], dice[4]);
    printf("---  ---  ---  ---  ---\n");
    printf("\n");
}

void ask_which_reroll(int dice[]){
    printf("Enter which dice you would like to reroll, seperated by spaces. (E.G.: 4 2)\n");
    int rerollArray[5];//defining a temp reroll array 
    char temp = 0;//using a temp char
    for(int i = 0; temp != '\n'; ++i){//looping through inputs until temp == new line 
        scanf(" %d%c", &rerollArray[i], &temp);//scanning for both a number and a character at once (allows us to check for new line)
        dice[rerollArray[i]-1] = 0; //setting all dice in the reroll array(-1) to 0. We have to minus 1 beause the array starts at 0;
    }
    clear_prev_lines(2);
}

void ask_roll(void){ //Roll loop + validator
    char roll = '\0';
    printf("Just type 'R' to roll the dice!\n");
    do{
    scanf(" %c", &roll);
    if(roll != 'r' && roll != 'R'){
        roll = INVALID;
        clear_prev_lines(1);
    }
    }while(roll == INVALID);
    clear_prev_lines(2);
}

int ask_reroll(void){//Reroll loop + validator
    char choice = '\0';
    printf("Would you like to reroll any dice? ('Y' or 'N')\n");
    do{
    scanf(" %c", &choice);
    if(choice != 'Y' && choice != 'y' && choice != 'N' && choice != 'n'){
        choice = INVALID;
        clear_prev_lines(1);
    }
    }while(choice == INVALID);

    if(choice == 'Y' || choice == 'y'){
        choice = YES;
    }
    else{
        choice = NO;
    };
    clear_prev_lines(2);
    return choice;
}

void clear_dice(int dice[]){//Sets all dice to 0, clearing them
    for(int i = 0; i < 5; i++){
    dice[i] = 0;
    }
}

void find_dice_freq(int dice[], int dice_freq[]){//Finds the frequency of each number rolled
for(int i = 0; i < 6; i++){//clearing dice_freq
    dice_freq[i] = 0;}
for(int i = 1; i < 7; ++i){//Using two loops. One for changing the position of the dice_freq[], and the other for counting in dice[]
    for(int e = 0; e < 5; ++e){
        if(dice[e] == i){
            dice_freq[(i-1)] = dice_freq[(i-1)] + 1;//Subtracing 1 from i because array starts at 0
        }
    }
}
}

int get_score_choice(int score_card[], int dice[], int dice_freq[]){//Input validator and returns the choice
    int choice = 0, straightCtr = 0, isYahtzee = 0;
    printf("What would you like to use this roll as? (Type 1 - 13)\n");
    do{
    scanf(" %d", &choice);
    if(choice < 1 || choice > 13){
        clear_prev_lines(2);
        printf("Invalid choice. Please choose again\n");
        choice = INVALID;
    }
    else if(score_card[choice] != UNMARKED && choice != YAHTZEE){//Seperate case for YAHTZEE because of bonus yahtzee things
        clear_prev_lines(2);
        printf("You have already written in the score for %d. Please choose again\n", choice);
        choice = INVALID;
    }
    switch(choice){
        case THREEOFKIND://Loops through dice_freq and checks if there is a 3 in any position
            for(int i = 0; i < 6; ++i){
                if(dice_freq[i] >= 3){
                    i = 6;//Stops loop if 3 or above is found
                    choice = THREEOFKIND;}//Assigning choice to the positive version of the choice
                else{
                    choice = THREEOFKIND * -1;}//If not found, we assign choice to the negative version of the choice
            }
            break;
        case FOUROFKIND:
            for(int i = 0; i < 6; ++i){
                if(dice_freq[i] >= 4){
                    i = 6;
                    choice = FOUROFKIND;}
                else{
                    choice = FOUROFKIND * -1;}
            }
            break;
        case FULLHOUSE://Very similar to 3/4 of a kind, but instead checks if there is a 2 of a kind AND a 3 of a kind
            for(int i = 0; i < 6; ++i){
                if(dice_freq[i] == 2){
                    for(int e = 0; e < 6; e++){
                        if(dice_freq[e] == 3){
                            i = 6; //If both 2 or 3 are found, break loop
                            e = 6;
                            choice = FULLHOUSE;
                        }
                        else{
                            choice = FULLHOUSE * -1;
                        }
                    }
                }
                else{
                    choice = FULLHOUSE * -1; //Another failure else{}
                }
            }
            break;
        case SMSTRAIGHT:
            for(int i = 0; i < 6; ++i){//Iterates through the dice_freq
                if(dice_freq[i] > 0 && dice_freq[i+1] > 0){//Checks if there is a streak (Dice_freq of i and next dice freq are over 1)
                    ++straightCtr; //Adds 1 to counter if
                    if(straightCtr >= 3){//If straightCTR gets to 3, break loop
                        i = 6;
                        choice = SMSTRAIGHT;
                    }
                }
                else{
                    choice = SMSTRAIGHT * -1;
                    straightCtr = 0; //Resets straightCtr if streak is broken
                }
            }
            break;
        case LGSTRAIGHT:
            for(int i = 0; i < 6; ++i){
                if(dice_freq[i] > 0 && dice_freq[i+1] > 0){
                    ++straightCtr;
                    if(straightCtr >= 4){
                        i = 6;
                        choice = LGSTRAIGHT;
                    }
                }
                else{
                    choice = LGSTRAIGHT * -1;
                    straightCtr = 0;
                }
            }
            break;
        case YAHTZEE:
            for(int i = 0; i < 6; ++i){
                if(dice_freq[i] >= 5){//Checks for yahtzee
                    i = 6;//breaks loop if found
                    isYahtzee = YES;}
                else{
                    isYahtzee = NO;}
            }
            choice = yahtzee_or_bonus(score_card, dice, dice_freq, choice, isYahtzee);//"Out sources" the yahtzee bonus logic to seperate function
            break;
        }
        
    }while(choice == INVALID);
    return choice;
}

//I only found out a few minutes ago that bonus yahtzee's are not required on this assignment...
//This also made a huge pain for checking when the players are done, cause a player could have 16 turns if all 3 bonus yahtzees are rolled
int yahtzee_or_bonus(int score_card[], int dice[], int dice_freq[], int choice, int isYahtzee){
    if(score_card[YAHTZEE] == UNMARKED){//Logic chain for if yahtzee is unmarked
        if(isYahtzee == YES){
            choice = YAHTZEE;}
        else{
            choice = YAHTZEE * -1;
        }
    }
    else if(score_card[YAHTZEE] == 50){//Logic chain for if it is marked w/ 50
        if(isYahtzee == YES){
            if(score_card[YAHTZEE_BONUS] < 300){
                choice = YAHTZEE_BONUS;}
            else{
                clear_prev_lines(2);
                printf("You have already written in all 3 bonus YAHTZEE's. Please choose again\n");
                choice = INVALID;}
            }
        else{
            clear_prev_lines(2);
            printf("You have already written in the score for %d. Please choose again\n", choice);
            choice = INVALID;
        }
    }
    else if(score_card[YAHTZEE] == SCRATCH){//Logic chain for if is marked with a 0 (scratched)
        clear_prev_lines(2);
        printf("You have already written in the score for %d. Please choose again\n", choice);
        choice = INVALID;}
        
    return choice;
}

void assign_score_to_card(int score_card[], int dice[], int choice){//Assigns proper score based on choice and roll
    int score = 0, type = 0;
    if(choice <= 0){type = SCRATCH;} //This is where negative versions of the choices are used
    else if(choice < 7){type = 1;}//Choices are 1-6 are all just calculating the sums of the chosen dice side.
    else if(choice == THREEOFKIND || choice == FOUROFKIND || choice == CHANCE){type = 2;} //All 3 choices do the same thing (add total sum)
    else{type = choice;}
    switch (type){
        case SCRATCH://If negatiive, then set the score to scratch (0)
            choice = choice * -1;
            score = SCRATCH;
            break;
        case 1: //Checkig sum of the chosen dice face
            for(int i = 0; i < 5; ++i){
                if(dice[i] == choice){
                    score = score + dice[i];
                }
            }
            break;
        case 2: //Adding all dice sum
            for(int i = 0; i < 5; ++i){
                    score = score + dice[i];
                }
            break;
        case FULLHOUSE:
            score = 25;
            break;
        case SMSTRAIGHT:
            score = 30;
            break;
        case LGSTRAIGHT:
            score = 40;
            break;
        case YAHTZEE:
            score = 50;
            break;
        case YAHTZEE_BONUS: //Adds +100 to the current bonus
            score = score_card[YAHTZEE_BONUS] + 100;
            break;
    }
    score_card[choice] = score;
}

void update_score_card_totals(int score_card[]){
    for(int i = 15; i < 19; i++){//Resets all totals to avoid double counting
        score_card[i] = 0;
    }
    for(int i = 1; i < 7; i++){
        if(score_card[i] != UNMARKED){//Adds all upper values that arent unmarked (unmarked is -1)
            score_card[UPPER_TOTAL] = score_card[UPPER_TOTAL] + score_card[i];
        }
    }
    if(score_card[UPPER_TOTAL] >= 63){//Adds the 35 point bonus if upper bonus is over 63
        score_card[UPPER_BONUS] = 35;
        score_card[UPPER_TOTAL]= score_card[UPPER_TOTAL] + score_card[UPPER_BONUS];
    }
    for(int i = 8; i < 15; i++){
        if(score_card[i] != UNMARKED){//Adds all lower values that arent unmarked
            score_card[LOWER_TOTAL] = score_card[LOWER_TOTAL] + score_card[i];
        }
    }
    score_card[GRAND_TOTAL] =  score_card[UPPER_TOTAL] + score_card[LOWER_TOTAL];//Adds upper and lower for grand total
}
void determine_if_full(int score_card[]){//Checks if the given score card is full
    for(int i = 1; i < 14; i++){
        if(score_card[i] == UNMARKED){
            i = 14; //If a unmarked is ever found, then kick out of loop and set ISFULL to NO (0)
            score_card[ISFULL] = NO;
        }
        else{
            score_card[ISFULL] = YES; //If unmarked is never found, then array is full
        }
    }
}

int determine_winner(int p1_score_card[], int p2_score_card[]){//Logic for comparing scores to get either 1, 2, or tie
    int winner = 0;
    if(p1_score_card[GRAND_TOTAL] > p2_score_card[GRAND_TOTAL]){
        winner = 1;
    }
    else if(p1_score_card[GRAND_TOTAL] > p2_score_card[GRAND_TOTAL]){
        winner = 2;
    }
    else if(p1_score_card[GRAND_TOTAL] == p2_score_card[GRAND_TOTAL]){
        winner = 3;
    }
    return winner;
}

void print_winner(int winner, int p1_score_card[], int p2_score_card[]){//Prints out winner w/ winning score
    switch(winner){
        case 1: printf("The winner is player %d with a score of %d!\n", winner, p1_score_card[GRAND_TOTAL]);
        break;
        case 2: printf("The winner is player %d with a score of %d!\n", winner, p2_score_card[GRAND_TOTAL]);
        break;
        case 3: printf("The game is a tie! Both players had a score of %d!\n", p1_score_card[GRAND_TOTAL]);
        break;
        default:printf("No winner...?\n");//Default incase something goes wrong w/ finding the winner
        break;
    }
}

int ask_play_again(void){//I
    char choice = '\0';
    printf("Would you like to play again?\n");
    do{
        scanf(" %c", &choice);
        if(choice != 'Y' && choice != 'y' && choice != 'N' && choice != 'n'){
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