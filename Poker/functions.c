#include "header.h"
// Authors: Deitel & Deitel - C How to Program
void shuffle (int wDeck[][13])
{
	int row = 0;    /* row number */
	int column = 0; /*column number */
	int card = 0;   /* card counter */

	/* for each of the 52 cards, choose slot of deck randomly */
	for (card = 1; card <= 52; card++)
	{
		/* choose new random location until unoccupied slot found */
		do
		{
			row = rand () % 4;
			column = rand () % 13;
		} while (wDeck[row][column] != 0);

		/* place card number in chosen slot of deck */
		wDeck[row][column] = card;
	}
}

// Authors: Deitel & Deitel - C How to Program
//Function is unchanged other than:
	//the removeal of the "CONST" infront of wDeck and the replacement of the printf to actually deal
		////This allows us to keep track of what cards have been drawn
	//the change of the main for conditions to cardsDrawn and cardsToDraw
void deal (int wDeck[][13], const char *wFace[], const char *wSuit[], Hand* inputHand, int cardsToDraw){
	int row = 0;    /* row number */
	int column = 0; /*column number */
	int card = 0;   /* card counter */
	int cardsDrawn = 0;
 
	/* deal each of the cards */
	for (card = 1; cardsDrawn < cardsToDraw; card++)
	{
		if(inputHand->current_cards[cardsDrawn].valueIndex == UNDRAWN){//Tests if we need to draw a card at current cardsDrawn
			/* loop through rows of wDeck */
			for (row = 0; row <= 3; row++)
			{
				/* loop through columns of wDeck for current row */
				for (column = 0; column <= 12; column++)
				{
					/* if slot contains current card, display card */
					if (wDeck[row][column] == card)
					{
						wDeck[row][column] = 0; //Setting the grabbed card to 0, so it wont be taken again
						inputHand->current_cards[cardsDrawn].valueIndex = column;
						inputHand->current_cards[cardsDrawn].faceIndex = row;
						++cardsDrawn;
					}
				}
			}
		}
		else{//If we dont need to draw one, "shift" both cardsDrawn and cardsToDraw to continue
			++cardsDrawn;
			++cardsToDraw;
		}
	}
} 

//Toolbox-functions
void clear_prev_lines(int lines){//Allows me to clear just the right amount of lines to keep the screen clean
    for(int i = 0; i < lines; ++i){
    printf("\x1b[1F");
    printf("\x1b[2K");
    }
}

//I put system("cls") in a function because it is system("cls") on windows and is system("clear") on MacOS
//Using a function makes it easy to just change it in 1 place when testing on other platforms
void clear_screen(void){
    system("clear");
}

void clear_input(char input[]){ //Sets all spaces in input to null
    for(int i = 0; i < 6; i++){
        input[i] = '\0';
    }
}

void flush_input_buffer(char input[]){//Flushes any extra characters left in STDIN after grabbing any input
    char ch;
	for(int i = 0; i < 10; i++){
		if(input[i] == '\n') return;
	}
	do{
	scanf("%c", &ch);;
	}while(ch != '\n'); //Tosses junk characters out until a new line is found
}

void prompt_enter(void){//Scans for enter key. Ignores junk characters
    printf("Press enter to continue!\n");
    char ch;
    do{
    scanf("%c", &ch);
    }while(ch != '\n');
}

void force_hand(Hand *inputHand, int requestedHand, int Deck[][13], const char *Face[], const char *Suit[]){
	//Forces hand by repeatidly drawing new cards until the desired hand is found
    int drawCtr = 0;
	bool isFaceDown = false;
	//Keeping track of flipped state
	if(inputHand->current_cards[0].faceDown == true){
		isFaceDown = true;
	}
	//Wiping deck and shuffling
	clear_deck(Deck);
	shuffle(Deck);
    do{
        init_hand(inputHand);
        if(drawCtr == 10){ //Draws 5 cards until 10 * 5 cards (50), then reshuffles
			clear_deck(Deck);
			shuffle(Deck);
			drawCtr = 0;
        }
        deal(Deck, Face, Suit, inputHand, 5);
		//Print statements for testing below.
        //printf("Hand so far:\n");
        //print_hand(Face, Suit, inputHand);
        evaluate_hand(inputHand);
        ++drawCtr;
    }while(inputHand->handEval != requestedHand);
	//printf("Forced hand of type '%d' found.\n", requestedHand);
	if(isFaceDown == true){//Once hand is found, flips cards that were flipped before
		flip_cards(inputHand);
	}
	clear_deck(Deck);
	shuffle(Deck);
}

//Menu functions
void print_menu(void){
    printf("---------------------------\n");
    printf("1. Display rules.\n");
    printf("2. Play 5 Draw Poker.\n");
    printf("3. Exit.\n");
    printf("---------------------------\n");
    return;
}

void print_game_rules (void){
    printf("Welcome to 5-card-draw poker!\n");
	printf("When you start the game, you and the dealer will both draw 5 cards.\n");
	printf("You will only be able to see your 5 cards. The dealer's card's will be hidden.\n");
	printf("After evaluating your hand, both you and the dealer will decide what cards to discard and redraw.\n");
	printf("You can only redraw up to 3 cards. You can also choose to redraw no cards and hold instead.\n");
	printf("After redrawing, the dealer will flip his cards and whoever has the better hand will win.\n");
	printf("The hands are as follows, from best to worst:");
	printf("Four of a kind > Full House > Flush > Straight > Three of a kind > Two Pairs > Pair.\n");
	printf("If you and the dealer have  the same hand, the game is a tie.\n");
	printf("Ready to play?\n");
return;
}

int get_choice(char input[]){
	//Gets choice and validates input
	bool valid;
	do{
	valid = true;
    clear_input(input);
	fgets(input, 3, stdin);
	flush_input_buffer(input);
	if(input[1] != '\n'){
		clear_prev_lines(1);
		valid = false;
	}
	else if(input[0] < '1' || input[0] > '3'){
		clear_prev_lines(1);
		valid = false;
	}
	}while(!valid);
	return input[0] - '0';
}

void execute_choice(int choice){
	//A if/else statement would work fine, but I wanted to be explicit about what numbers caused what
    switch(choice){
        case 1:
            clear_screen();
            print_game_rules();
            print_menu();
            break;
        case 3:
            printf("Goodbye!\n");
            exit(1);
            break;
    }
}

//Prep functions
void clear_deck (int wDeck[][13]){
	int row = 0;
	int column = 0;

	//Loops through and sets all cards to 0
	for (row = 0; row < 4; row++){
		for (column = 0; column < 13; column++){
				wDeck[row][column] = 0;
		}
	}
}


void init_hand(Hand *inputHand){
	//Sets a hand to a blankslate
	inputHand->handEval = 0;
	for(int i = 0; i < 5; ++i){
		inputHand->current_cards[i].valueIndex = UNDRAWN;
		inputHand->current_cards[i].faceIndex = UNDRAWN;
		inputHand->current_cards[i].faceDown = false;
	}
}

//Flips cards from one state to another
void flip_cards(Hand *inputHand){
	for(int i = 0; i < 5; ++i){
		if(inputHand->current_cards[i].faceDown == false){
			inputHand->current_cards[i].faceDown = true;
		}
		else{
			inputHand->current_cards[i].faceDown = false;
		}
	}
}

//Evaluation/Discard functions
void print_hand(const char *wFace[], const char *wSuit[], Hand *inputHand){
	//Prints corners via unicode characters
	printf("----  ----  ----  ----  ----\n");
	for(int i = 0; i < 5; ++i){
		if(inputHand->current_cards[i].faceDown){
			//Prints any facedown cards as ##
			printf("|##|  ");
		}
		else{
			printf("|%-2s|  ", wFace[inputHand->current_cards[i].valueIndex]);
		}
	}
	printf("\n");
	for(int i = 0; i < 5; ++i){
		if(inputHand->current_cards[i].faceDown){
			printf("|##|  ");
		}
		else if(inputHand->current_cards[i].faceIndex < 2){
			printf("|"RED"%-4s"RESET"|  ", wSuit[inputHand->current_cards[i].faceIndex]);
			//Prints the unicode char of the card face based on color
		}
		else{
			printf("|"CYN"%-4s"RESET"|  ", wSuit[inputHand->current_cards[i].faceIndex]);
		}
	}
	printf("\n");
	//Printing bottom + unicode corners
	printf("----  ----  ----  ----  ----\n");
}

void evaluate_hand(Hand *inputHand){
//Making two freq arrays for evaultion
	int valueFreqArray[13] = {0};
	int faceFreqArray[4] = {0};

	for(int i = 0; i < 13; ++i){
		for(int j = 0; j < 5; ++j){
			if(inputHand->current_cards[j].valueIndex == i){
				++valueFreqArray[i];
			}
		}
	}
	for(int i = 0; i < 4; ++i){
		for(int j = 0; j < 5; ++j){
			if(inputHand->current_cards[j].faceIndex == i){
				++faceFreqArray[i];
			}
		}
	}

//We check for the highest scoring hand down to the worst scoring card.
//(This is because a 3 of a kind would score for a pair and a 3 of a kind, so we must go top to bottom)

//Four of a kind checking
	for(int i = 0; i < 13; ++i){
		if(valueFreqArray[i] >= 4){
			inputHand->handEval = FOUROFKIND;
			return;
		}
	}

//Full house checking
	for(int i = 0; i < 13; ++i){
		if(valueFreqArray[i] == 3){
			for(int j = 0; j < 13; ++j){
				if(valueFreqArray[j] == 2){
					inputHand->handEval = FULLHOUSE;
					return;
				}
			}
		}
	}	

//Flush checking
	for(int i = 0; i < 13; ++i){
		if(faceFreqArray[i] == 5){
			inputHand->handEval = FLUSH;
			return;
		}
	}

//Straight Checking
	int strCtr = 0;
	for(int i = 0; i < 12; ++i){
		if(valueFreqArray[i] == 1 && valueFreqArray[i+1] == 1){
			++strCtr;
		}
	}
	if(strCtr == 4){
		inputHand->handEval = STRAIGHT;
	}

//Three of a kind checking
	for(int i = 0; i < 13; ++i){
		if(valueFreqArray[i] >= 3){
			inputHand->handEval = THREEOFKIND;
			return;
		}
	}

//Two Pair Checking
	for(int i = 0; i < 13; ++i){
		if(valueFreqArray[i] >= 2){
			for(int j = i+1; j < 13; ++j){
				if(valueFreqArray[j] >= 2){
					inputHand->handEval = TWOPAIR;
					return;
				}
			}
		}
	}	

//Pair Checking
	for(int i = 0; i < 13; ++i){
		if(valueFreqArray[i] >= 2){
			inputHand->handEval = PAIR;
			return;
		}
	}
//If we get to this point, then the hand is worthless
	inputHand->handEval = NOTHING;
	return;
}

int determine_discard(Hand *inputHand){
	//Logic: Reroll only the things that could *only* improve your hand.
		//Example: a 2 pair would redraw to try for a full house.
		//A 2 pair *wouldnt* reroll for a 4 of a kind

	//Nothing -> Try for anything
		//If the hand has nothing, then it redraws the max amount (which is 3)
		//Discard any 3 (first 3 for ease)

	//Pair -> Try for 3/4 of a kind or fullhouse
		//Discard 3
		//Keep pair

	//Three of a kind -> Try for Four of a kind
		//Discard 2
		//Keep 3 of Kind

	//Straight -> Hold (Would have to lose the Sraight in order to try for anything better)
		//Discard 0

	//Flush -> Hold (Would have to lose the Flush in order to try for anything better)
		//Discard 0

	//Fullhouse -> Hold (Would have to lose the Fullhouse in order to try for anything better)
		//Discard 0;

	//Four of a kind -> Hold (Nothing better)
		//Discard 0

//Freq array for values + variables
	int valueFreqArray[13] = {0};
	int n = 0, m = 0;
	for(int i = 0; i < 13; ++i){
		for(int j = 0; j < 5; ++j){
			if(inputHand->current_cards[j].valueIndex == i){
				++valueFreqArray[i];
			}
		}
	}
	
	switch(inputHand->handEval){
		case NOTHING:
			for(int i = 0; i < 3; ++i){
			inputHand->current_cards[i].valueIndex = UNDRAWN;
			inputHand->current_cards[i].faceIndex = UNDRAWN;
			}
			return 3;
		case PAIR:
			for(; valueFreqArray[n] != 2; ++n); //Searches through freq array to find what value causes the pair
			for(int i = 0; i < 5; ++i){
				//Goes through all 5 cards and resetting any that don't contribute to the 3 of a kind
				if(inputHand->current_cards[i].valueIndex != n){ 
					inputHand->current_cards[i].valueIndex = UNDRAWN;
					inputHand->current_cards[i].faceIndex = UNDRAWN;
				}
			}
			return 3;
		case TWOPAIR:
			for(; valueFreqArray[n] != 2; ++n); //Searches through freq array to find what value causes the first pair
			for(m = n+1; valueFreqArray[m] != 2; ++m); //Searches through freq array to find what value causes the second pair
			for(int i = 0; i < 5; ++i){
				//Goes through all 5 cards and resetting any that don't contribute to the 3 of a kind
				if(inputHand->current_cards[i].valueIndex != n && inputHand->current_cards[i].valueIndex != m){ 
					inputHand->current_cards[i].valueIndex = UNDRAWN;
					inputHand->current_cards[i].faceIndex = UNDRAWN;
				}
			}
			return 1;
		case THREEOFKIND:
			for(; valueFreqArray[n] != 3; ++n); //Searches through freq array to find what value causes the 3 of a kind
			for(int i = 0; i < 5; ++i){
				//Goes through all 5 cards and resetting any that don't contribute to the 3 of a kind
				if(inputHand->current_cards[i].valueIndex != n){ 
					inputHand->current_cards[i].valueIndex = UNDRAWN;
					inputHand->current_cards[i].faceIndex = UNDRAWN;
				}
			}
			return 2;
		default:
			return 0;
	}
}

void prompt_redraw(Hand *inputHand, char input[]){
	bool inputValid;
	printf("Please enter the cards you would like to redraw (max 3), or hold your cards with 'H'!\n");
	printf("E.G: '2 4 5' for replacing the 2nd, 4th, and 5th card.\n");
	do{
	clear_input(input);
	fgets(input, 7, stdin);
	flush_input_buffer(input);
	input[0] = toupper(input[0]);
	inputValid = validate_redraw_input(input);
	if(inputValid == false){
		clear_prev_lines(3);
		printf("Invalid input. Please enter the cards you would like to redraw (max 3), or hold your cards with 'H'!\n");
		printf("E.G: '2 4 5' for replacing the 2nd, 4th, and 5th card.\n");
	}
	}while(inputValid == false);
}

bool validate_redraw_input(char input[]){
	//Early checking for a fully valid input for Holding
	if(input[0] == 'H' && input[1] == '\n'){
		return true;
	}
	//We need to check for 3 main things in this loop:
	//Is the char a number?
	//If it is, then is new char a new line?
		//If so, then its good!
	//If new line isnt next, is it a space instaed space?
		//If so, then keep checking
	//Repeat
	//If we get to the end and it never kicked us out with a good input, then it must be bad
	for(int i = 0; i < 5; i+= 2){
		if((input[i] < '1' || input[i] > '5') && input[i] != '\0'){
			return false;
		}
		if(input[i+1] == '\n'){
			return true;
		}
		if(input[i+1] != ' '){
			return false;
		}
	}
	return false;
}

int discard_inputted_cards(Hand *inputHand, char input[]){
	int discardCount = 0, n = -2, end = 0;
	//We know that if we are holding, just return 0
	if(input[0] == 'H'){
		return 0;
	}

	do{
		n+= 2;
		inputHand->current_cards[input[n] - '0' - 1].valueIndex = UNDRAWN;
		inputHand->current_cards[input[n] - '0' - 1].faceIndex = UNDRAWN;
		++discardCount;
	}while(input[n+1] != '\n');

	return discardCount;
}

//Wrap up functions
int determine_winner(int player_hand_value, int dealer_hand_value){
    int winner = 0;
    if(player_hand_value > dealer_hand_value){
        winner = 1;
    }
    else if(player_hand_value < dealer_hand_value){
        winner = 2;
    }
    else if(player_hand_value == dealer_hand_value){
        winner = 3;
    }
    return winner;
}

void print_winner(int winner){
    switch(winner){
        case 1: printf("You won against the dealer!.\n");
        break;
        case 2: printf("The dealer won this game.\n");
        break;
        case 3: printf("The game is a tie!\n");
        break;
        default: printf("No winner...?\n");//Default incase something goes wrong w/ finding the winner
        break;
	}
}

bool get_play_again(char input[]){
	//Almost identical function to getting menu input, but it simply outputs TRUE or FALSE
	bool valid = true;
	printf("Would you like to play again?\n");
	do{
	valid = true;
    clear_input(input);
    fgets(input, 3, stdin);
    flush_input_buffer(input);
	input[0] = toupper(input[0]);
	if(input[1] != '\n'){
        clear_prev_lines(1);
		valid = false;
    }
	else if(input[0] != 'Y' && input[0] != 'N'){
        clear_prev_lines(1);
		valid = false;
	}
	}while(!valid);

	if(input[0] == 'Y'){
		return true;
	}
	return false;
}
