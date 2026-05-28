#include "header.h"

int main (void)
{
    
	/* initialize suit array */
    const char *suit[5] = {HEART, DIAMOND, CLUB, SPADE}; //Changed to Unicode Macros
	/* initialize face array */
	const char *face[13] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"}; //Changed to small char strings
	/* initalize deck array */
	int deck[4][13] = {0}; //Unchanged

	srand ((unsigned) time (NULL)); /* seed random-number generator */
    
//Var defining
    int choice = 0, discardAmount = 0, winner = 0;
    char input[8] = {0};
    Hand playerHand;
    Hand dealerHand;
    bool playAgain;

//Menu prompting
    clear_screen();
    print_menu();
    do{
        choice = get_choice(input);
        execute_choice(choice);
    }while(choice != PLAY);

//Main game loop
    do{
//Hand prepwork
    init_hand(&playerHand);
    init_hand(&dealerHand);
    flip_cards(&dealerHand);
//Game prepwork
    clear_screen();
    clear_deck(deck);
	shuffle(deck);

//Inital deal and print of dealer hand
    deal(deck, face, suit, &dealerHand, 5);
    //force_hand(&dealerHand, FOUROFKIND, deck, face, suit);
        //For testing, feel free to use the above command and replace the FOUROFKIND with whatever you want the dealers hand to be.
        //Warning: this will reshuffle the deck, so you may find a duplicate card in the players hand if this is used.
    printf("Dealers Hand:\n");
    print_hand(face, suit, &dealerHand);
    evaluate_hand(&dealerHand);

//Inital deal and print of player hand
    deal(deck, face, suit, &playerHand, 5);
    //force_hand(&playerHand, FULLHOUSE, deck, face, suit);
        //The function can also be used with the playerhand (it keeps track of flipped status)
    printf("Your Hand:\n");
    print_hand(face, suit, &playerHand);
    evaluate_hand(&playerHand);

//Dealer discard process
    discardAmount = determine_discard(&dealerHand);
    deal(deck, face, suit, &dealerHand, discardAmount);
    printf("\nThe dealer is discarding %d card%c.\n", discardAmount, discardAmount == 1 ? '\0' : 's'); 

//Player discard process
    prompt_redraw(&playerHand, input);
    discardAmount = discard_inputted_cards(&playerHand, input);
    deal(deck, face, suit, &playerHand, discardAmount);
    printf("\nYou have discarded %d card%c.\n", discardAmount, discardAmount == 1 ? '\0' : 's'); 

//Final print of dealer hand
    flip_cards(&dealerHand);//Making dealers cards visable after the flip
    printf("Dealers New Hand:\n");
    print_hand(face, suit, &dealerHand);
    evaluate_hand(&dealerHand);

//Final print of player hand
    printf("Your New Hand:\n");
    print_hand(face, suit, &playerHand);
    evaluate_hand(&playerHand);

//Determining and printing the winner
    winner = determine_winner(playerHand.handEval, dealerHand.handEval);
    print_winner(winner);
    //force_hand(&dealerHand, FOUROFKIND, deck, face, suit);

//Ask if play again
    }while(get_play_again(input) == true);
    printf("Goodbye!\n");
	return 0;
}