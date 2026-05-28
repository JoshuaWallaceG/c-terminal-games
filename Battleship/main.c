#include "header.h"
int main(void){
    srand((unsigned int)time(NULL)); //Seeding random
    FILE *outfile = fopen("battleship.log", "w"); //Opening up log file

    //Creating a ship variable for each type of ship. Carries all needed information
    Ship car = {"Carrier", 'c', 5};
    Ship bat = {"Battleship", 'b', 4};
    Ship crus = {"Cruiser", 'r', 3};
    Ship sub = {"Submarine", 's', 3};
    Ship des = {"Destroyer", 'd', 2};
    Ship shipArray[5] = {car, bat, crus, sub, des}; //Putting it into an array for easy passing

    //Creating the stats page for each player, then making an array out of them
    Stats player1 = {0, 0, 0, 0.0, 0.0, false};
    Stats player2 = {0, 0, 0, 0.0, 0.0, false};
    Stats playerStats[2] = {player1, player1};
    
    //Creating each players board, putting them into an array, and then initializing
    char p1_board[10][10] = {{'\0'}};
    char p2_board[10][10] = {{'\0'}};
    PlayerBoards boardArray[2] = {p1_board[10][10], p2_board[10][10]};
    init_board(boardArray[0].board, 10, 10);
    init_board(boardArray[1].board, 10, 10);

    //Delcaring all other variables
    int starterPlayer = 0, nextPlayer = 0, currentPlayer = 0, turn = 0, sunkenShip = 0;
    bool won = false;
    char choice = '\0', result = '\0', input[16] = {};

//This marks the start of the main game
    clear_screen();
    print_menu();
    prompt_enter();
    clear_screen();
    choice = prompt_rand_or_man(input);

    switch(choice){ //A switch choice for manual or random creation (from prompt_rand_or_man)
        case 'M':
            for(int i = 0; i < 5; ++i){ //Passes in the 'i' from the loop to keep track
                clear_screen();
                print_ship_board(boardArray[0].board, 10, 10);
                prompt_place(input, shipArray, boardArray[0].board, i);
                update_ship_placement(input, shipArray, boardArray[0].board, i);
            }
            clear_screen();
            break;
        case 'R':
            clear_screen();
            rand_place(boardArray[0].board, shipArray);
            printf("Player 1's board has been generated...\n");
            break;
    }

    //Generating player 2's board
    print_ship_board(boardArray[0].board, 10, 10);
    rand_place(boardArray[1].board, shipArray);
    printf("Player 2's board has been generated...\n");

    starterPlayer =  rand() % 2; //Determines a starter player from coinflip
    printf("Player %d has been randomly selected to go first!\n", starterPlayer+1);
    prompt_enter();

    if(starterPlayer == 1) ++turn; //Adds +1 to turn flip counter to start at the computer
    while(!won){ //Loops while nobody has won
        currentPlayer = turn % 2;
        nextPlayer = (turn+1) % 2;
        sunkenShip = -1; //Resetting sunkenship var
        clear_screen();
        print_result_board(boardArray[1].board, 10, 10);
        print_ship_board(boardArray[0].board, 10, 10);
        switch(currentPlayer){//Switch statement to either promp user or generate random
            case 0: prompt_target(input, boardArray[nextPlayer].board);
                break;
            case 1: rand_target(input, boardArray[nextPlayer].board);
                break;
        }
        result = check_hit_or_miss(input, boardArray[nextPlayer].board); //Checks hit or miss and output results
        if(result == '*'){ //Only checks if a ship is sunkend if the result is a hit
            sunkenShip = check_if_sunk(input, boardArray[nextPlayer].board);
        }
        update_hit_or_miss(input, boardArray[nextPlayer].board, result); //Updates bored based on result
        clear_screen();
        print_result_board(boardArray[1].board, 10, 10); //Printing updated versions of the boards
        print_ship_board(boardArray[0].board, 10, 10);
        if(currentPlayer == 1){ //Announces the computers choice
            printf("Computer has chosen %c%c!\n", input[0], input[1]);
        }
        print_hit_or_miss(input, result); //Prints result
        print_if_sunk(sunkenShip, shipArray); //Prints any sunken ships
        prompt_enter();
        won = check_if_winner(boardArray[nextPlayer].board); //Checks if the current player has won
        update_stats(&playerStats[currentPlayer], result, won); //Update stats for current player
        print_turn_to_file(outfile, input, result, sunkenShip, shipArray, currentPlayer+1, nextPlayer+1); //Logs the turn
        ++turn;
    }
    printf("All of player %d's ships have been sunk!\n", (turn % 2) + 1); //Prints who lost
    printf("Player %d is the winner!\n", ((turn+1)%2)+1); //Prints who won
    print_stats_to_file(outfile, playerStats, (((turn+1)%2)+1)); //Prints out final statistics and winning player to log file
    printf("Statistics outputted to logfile successfully!\n");
    fclose(outfile); //Closing log file before finishing
    return 0;
}