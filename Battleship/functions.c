#include "header.h"

//Toolbox Functions (See header.h)
void clear_prev_lines(int lines){//Alows me to clear just the right amount of lines to keep the screen clean
    for(int i = 0; i < lines; ++i){
    printf("\x1b[1F");
    printf("\x1b[2K");
    }
}

//I put system("cls") in a function because on windows, it is "cls" and macOS, it is system("clear").
//Using a function makes it easy to just change it in 1 place when testing on other platforms
void clear_screen(void){
    system("clear");
}

void flush_input_buffer(char input[], int expectedNewLinePos){//Flushes any extra characters left in STDIN after grabbing any input
    char ch;
    if(input[expectedNewLinePos] != '\n' && input[expectedNewLinePos] != '\0'){
        do{
        scanf("%c", &ch);
        }while(ch != '\n'); //Tosses junk characters out until a new line is found
    }
}

void clear_input(char input[]){ //Sets all spaces in input to null
    for(int i = 0; i < 16; i++){
        input[i] = '\0';
    }
}

//Function-functions 
void print_menu(void){
    printf("****************** Welcome to "RED"Battle"BLU"ship"RESET"! ******************\n");
    printf("This is a two player game. You are Player 1, and Player 2 is the computer.\n");
    printf("You will start by either placing each one of your ships manually, or letting them be randomly generated.\n");
    printf("Afterwards, Player 2 will have their board generated as well.\n");
    printf("Then, you and Player 2 will take turns 'shooting' at eachothers boards by typing out cordinates.\n");
    printf("If you hit a ship, it will be announced and a red mark will appear on the upper board.\n");
    printf("If you miss, it will be announced and a green 'm' will appear on the upper board.\n");
    printf("If the computer hits one of yours ships, a red mark will appear on your ship on the bottom board.\n");
    printf("If the computer misses, then a green 'm' will appear on bottom board.\n");
    printf("Once an entire ship has been hit, it is now 'sunk' and it will be announced as such.\n");
    printf("This will continue until either player has fully sunk the opponents fleet of ships.\n");
    printf("After the game, a winner will be announced and a log of all moves and other statistics will be generated.\n");
    printf("\n");
}   

void prompt_enter(void){//Scans for enter key. Ignores junk characters
    printf("Press enter to continue!\n");
    char ch;
    do{
    scanf("%c", &ch);
    }while(ch != '\n');
}

void init_board (char board[10][10], int rows, int cols){//Intitizes all boards to the "wave" symbol
    int row_index = 0, col_index = 0;
    for(; row_index < rows; ++row_index){
        for(col_index = 0; col_index < cols; ++col_index){
            board[row_index][col_index] = '~';
        }
    }
}

void print_ship_board (char board[10][10], int rows, int cols){//Prints out the ship board unchanged
    int row_index = 0, col_index = 0;
    printf("-----YOUR BOARD------\n");
    printf("%3d%2d%2d%2d%2d%2d%2d%2d%2d%2d\n", 0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
    for(; row_index < rows; ++row_index){
        printf("%c", 'A' + row_index);
        for(col_index = 0; col_index < cols; ++col_index){
            switch(board[row_index][col_index]){//Uses color codes to make it eaiser to see where you hit/missed
                case 'm': printf(GRN " m" RESET);
                    break;
                case '*': printf(RED " *" RESET);
                    break;
                case '~': printf(BLU " ~" RESET);
                    break;
                default: printf("%2c", board[row_index][col_index]);
                    break;
            }
        }
        printf("\n");
    }
    printf("\n");
}

void print_result_board (char board[10][10], int rows, int cols){//The same algorithm as above, but prints out a "result" version, hiding the ships
    int row_index = 0, col_index = 0;
    printf("-----THEIR BOARD-----\n");
    printf("%3d%2d%2d%2d%2d%2d%2d%2d%2d%2d\n", 0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
    for(; row_index < rows; ++row_index){
        printf("%c", 'A' + row_index);
        for(col_index = 0; col_index < cols; ++col_index){
            switch(board[row_index][col_index]){
                case 'm': printf(GRN " m" RESET);
                    break;
                case '*': printf(RED " *" RESET);
                    break;
                default: printf(BLU " ~" RESET);
                    break;
            }
        }
        printf("\n");
    }
    printf("\n");
}

char prompt_rand_or_man(char input[]){ //Prompts the user to either randomly generate board or manually place them
    bool check;
    do{
        printf("Would you like to randomize your ships (R), or manually place them (M)?\n");
        clear_input(input);
        fgets(input, 3, stdin);
        flush_input_buffer(input, 1);//
        input[0] = toupper(input[0]);
        if((input[0] == 'R' || input[0] == 'M') && input[1] == '\n'){ //Very basic input validation loop 
            check = true;
        }
        else{
        clear_prev_lines(2);//Keeps it pretty incase they misinput
        }
    }while(!check);
    return input[0];//Returns choice
}

void prompt_place(char input[], Ship shipArray[], char board[10][10], int i){ //Prompts user to place a ship
    int errorCheck;
    int desiredLength = shipArray[i].size * 3;
    printf("Please enter the %d cells to place the %s across:\n", shipArray[i].size, shipArray[i].name);
    printf("(Input cells as XY XY XY... E.G: B2 B3 B4)\n");
    do{
        clear_input(input);
        fgets(input, desiredLength+1, stdin);
        flush_input_buffer(input, desiredLength-1);
        for(int i = 0; i < strlen(input); i++){
            input[i] = toupper(input[i]);//Standardizing the input to all capitals
        }
        errorCheck = validate_cell_input(input, board, shipArray[i].size, desiredLength);
        if(errorCheck != 0){
            clear_prev_lines(3);
            print_input_error(errorCheck);
            printf("Please enter the %d cells the %s again.\n", shipArray[i].size, shipArray[i].name);
            printf("(Input cells as XY XY XY... E.G: B2 B3 B4)\n");
    }
    }while(errorCheck != 0);
}

int validate_cell_input(char input[], char board[10][10], int size,  int desiredLength){
/*
A proper input would look like this:
(|X|Y| |X|Y| |X|Y| |X|Y|\n|\0|)
With X being a letter from A-J, and Y being a number from 0-9;
Desired length is simply size of ship * 3, as each cordinate takes up 3 char (X)(Y)(SPACEorNEWLINE)
I have to check hella inputs cause of how much can go wrong. I broke it down into the follow conditions:
    1)Length of string has to be the  (AKA position of desiredLength -1 has to be newline)
    2)Must be in format of (Letter)(Number)(Space)...(Letter)(Number)(NewLine)
    3)Numbers and Letters must be within (0-9) and (A-J)
    4)All the letters must be the same OR all the numbers must be the same
    5)Whatever is diffrent from condition 5 must be sequential with no duplicates
    6)The ship can not overlap another players ship
*/
    //1) Checks if the input is the correct length. Returns 1 if not.
    if(input[desiredLength-1] != '\n') return 1;

    //2&3) Increments through the string till desired length is met.
    for(int i = 0; i < desiredLength; ++i){
        switch(i % 3){
            case 0: if(input[i] < 'A' || input[i] > 'J') return 2;//Checks if letter position is a valid letter
            break;
            case 1: if(input[i] < '0' || input[i] > '9') return 3; //Checks if the number position is a valid number
            break;
            case 2: if(input[i] != ' ' && input[i] != '\n') return 4; //Checks if the blank position is a space or newline
            break;
        }    
    }

    //4) Checks if all letters or all numbers are the same
    for(int i = 0; i < desiredLength-3; i += 3){
        if((input[i] != input[i+3]) && (input[i+1] != input[i+4])) return 5;
    }

    //At this point, we can finally check if the cordinates are next to eachother
    //I first start by checking if this is a horizonal or veritcal ship and set a type associated with it
    int freq_array[10] = {'\0'};
    int type = 0, sub = 0, seqcounter = 0;
    if(input[0] == input[3]) type = 1, sub = '0';
    else type = 0, sub = 'A';
    
    //Then I make a freq array, starting e at whatever type it is, subtracting the corresponding subtraction value, and adding +1 to freq array if equal
    for(int i = 0; i < 10; ++i){
        for(int e = type; e < desiredLength; e+= 3){
            if((input[e]-sub) == i){
                ++freq_array[i];
            }
        }
    }

    //Counting how many are in a sequence
    for(int i = 0; i < 9; i++){
        if(freq_array[i] >= 1 && freq_array[i+1] >= 1){
            ++seqcounter;
        }
    }

    //5) Checks if all ships are in a line
    if(seqcounter != size - 1) return 6;

    //6) Iterates through each cordinate to make sure it doesnt overlap with another ship
    for(int i = 0; i < desiredLength-1; i+= 3){
        if(board[(input[i]-'A')][(input[i+1]-'0')] != '~') return 7;
    }

    return 0;
}

void print_input_error(int errorType){ //Uses the "error codes" from prev input validation to tell user what they did wrong
    switch(errorType){
        case 1: printf("ERROR: Input is not correct size. ");
            break;
        case 2: printf("ERROR: Invalid row. ");
            break; 
        case 3: printf("ERROR: Invalid column. ");
            break;
        case 4: printf("ERROR: Format not correct. (Spacing error) ");
            break;
        case 5: printf("ERROR: Cell placement is not in a straight line. ");
            break;
        case 6: printf("ERROR: Ship's cells have gaps in it. ");
            break;
        case 7: printf("ERROR: Ship overlaps with another ship. ");
            break;
        case 8: printf("ERROR: Position has already been shot. ");//This is used only for grabbing targets
            break;
    }
}

void update_ship_placement(char input[], Ship shipArray[], char board[10][10], int i){
    for(int e = 0; e < shipArray[i].size * 3; e += 3){
        board[input[e]-'A'][input[e+1]-'0'] = shipArray[i].letter; //Places each cordinate down after passing through input validation
    }
}

void rand_place(char board[10][10], Ship ship_array[]){ //Random placement of all ships
    for(int i = 0; i < 5; ++i){
        int HorV = rand() % 2; // 50/50 if it is horizontal or vertical
        int row, col;
        bool overLapped = false;
        switch(HorV){
            case 0:
                row = rand() % 10;
                col = rand() % (11 - ship_array[i].size); //Only generates valid starting cordinates
                for(int e = 0, tempcol = col; e < ship_array[i].size && !overLapped; ++e, ++tempcol){ //Checks for overlapping before placing
                    if(board[row][tempcol] != '~'){
                        --i;
                        overLapped = true;
                    }
                }
                if(!overLapped){ //If no overlaps, then it places
                    for(int e = 0, tempcol = col; e < ship_array[i].size; e++, ++tempcol){
                            board[row][tempcol] = ship_array[i].letter;
                    }
                }
                break;
            case 1: //Same idea, just swapping the rows and columns
                row = rand() % (11 - ship_array[i].size);
                col = rand() % 10;
                for(int e = 0, temprow = row; e < ship_array[i].size && !overLapped; ++e, ++temprow){
                    if(board[temprow][col] != '~'){
                        --i;
                        overLapped = true;
                    }
                }
                if(!overLapped){
                    for(int e = 0, temprow = row; e < ship_array[i].size; e++, ++temprow){
                            board[temprow][col] = ship_array[i].letter;
                    }
                }
            break;
        }
    }
}

void prompt_target(char input[], char board[10][10]){ //Prompts user for a cordinate to shoot at
    int errorCheck;
    printf("Enter a target:\n");
    printf("(Input cells as XY... E.G: B2)\n");
    do{
        clear_input(input);
        fgets(input, 4, stdin);//Always fgets the amount if chars I want + 1 (for null). (I want 2 char + new line, so 4))
        flush_input_buffer(input, 2);
        input[0] = toupper(input[0]);
        errorCheck = validate_target_input(input, board);
        if(errorCheck != 0){
            clear_prev_lines(3);
            print_input_error(errorCheck);
            printf("Please enter a target again:\n");
            printf("(Input cells as XY... E.G: B2)\n");
        }
    }while(errorCheck != 0);
}

void rand_target(char input[], char board[10][10]){
    bool overlap = true;
    do{
    clear_input(input); 
    input[0] = (char)(rand() % 10) + 'A'; //Generates a fake user input that is 100% random
    input[1] = (char)(rand() % 10) + '0';
    input[2] = '\n';
    if(board[input[0]-'A'][input[1]-'0'] != 'm' && board[input[0]-'A'][input[1]-'0'] != '*'){//Checks for overlap
        overlap = false;
    }
    }while(overlap);
}

int validate_target_input(char input[], char board[10][10]){ //Validates user input using same error codes as before
    if(input[2] != '\n') return 1;
    if(input[0] < 'A' || input[0] > 'J') return 2;
    if(input[1] < '0' || input[1] > '9') return 3;
    if(board[input[0]-'A'][input[1]-'0'] == 'm' || board[input[0]-'A'][input[1]-'0'] == '*') return 8; //Another check for if it has already been shot at
    return 0;
}

char check_hit_or_miss(char input[], char board[10][10]){
    char result = '0'; //Checks if they hit a wave. If they didnt hit a wave, it is assumed that they mustve hit a ship
    if(board[input[0]-'A'][input[1]-'0'] == '~'){
        result = 'm';
    }
    else{
        result = '*';
    }
    return result;   
}


void update_hit_or_miss(char input[], char board[10][10], char result){
    board[input[0]-'A'][input[1]-'0'] = result; //Updates the cord input for whatever position it is
}

int check_if_sunk(char input[], char board[10][10]){///Checks if sunk by determining if there are any other parts of the ship left
    for(int row_index = 0; row_index < 10; ++row_index){
        for(int col_index = 0; col_index < 10; ++col_index){
            if(board[row_index][col_index] == board[input[0]-'A'][input[1]-'0'] && (row_index != input[0]-'A' || col_index != input[1]-'0')){
                return -1; //Early return if another part of the hit ship is found
            }
        }
    }
    //A little bit messy, but we are returning a integer depending on what type of ship is now sunk
    if(board[input[0]-'A'][input[1]-'0'] == 'c') return 0;
    if(board[input[0]-'A'][input[1]-'0'] == 'b') return 1;
    if(board[input[0]-'A'][input[1]-'0'] == 'r') return 2;
    if(board[input[0]-'A'][input[1]-'0'] == 's') return 3;
    else return 4; //The only way to get to here is if it is a destroyer, so we dont even check for it.
}

void print_hit_or_miss(char input[], char result){
    switch(result){ //Prints a colored response based on if the result is a hit or miss
    case '*': printf("%c%c is a "RED"hit"RESET"!\n", input[0], input[1]);
        break;
    case 'm': printf("%c%c is a "GRN"miss"RESET"...\n", input[0], input[1]);
        break;
    }
}

void print_if_sunk(int sunkenShip, Ship shipArray[]){
    if(sunkenShip == -1) return; //Prints the resulting ship name if the ship was sunken
    printf("The %s has been "RED"sunk"RESET"!\n", shipArray[sunkenShip].name);
}

bool check_if_winner(char board[10][10]){ //Iterates through every cordinate
    for(int row_index = 0; row_index < 10; ++row_index){
        for(int col_index = 0; col_index < 10; ++col_index){ //If a ship is found, then returns that there is no winner
            if(board[row_index][col_index] >='a' && board[row_index][col_index] <= 'z' && board[row_index][col_index] != 'm'){
                return false;
            }
        }
    }
    return true; //If no ships are found, then we have a winner
}

void update_stats(Stats *player, char result, bool won){ //Updates the stats of the inputed user
    ++player->shots;
    switch(result){
        case '*':
            ++player->hits;
            break;
        case 'm':
            ++player->misses;
    }
    if(player->misses > 0){//Checks for division by 0 (if their first shot happens to hit)
        player->hitsToMiss = (double)player->hits / (double)player->misses; //Type casting to avoid integer division
    }

    player->hitsToShots = (double)player->hits / (double)player->shots * 100.0; // Type casting and making it a percentage

    if(won == true){ // Checks if the current player from the loop won
        player->won = true; 
    }
}

//Outputs each turn to the .log file
void print_turn_to_file(FILE *output_stream, char input[], char result, int sunkenShip, Ship shipArray[], int currentPlayer, int nextPlayer){
    fprintf(output_stream, "Player %d's turn:\n", currentPlayer);
    fprintf(output_stream, "Player %d chose '%c%c'\n", currentPlayer, input[0], input[1]);
    switch(result){
    case '*': 
        fprintf(output_stream, "'%c%c' is a hit.\n", input[0], input[1]);
        break;
    case 'm': 
        fprintf(output_stream, "'%c%c' is a miss.\n", input[0], input[1]);
        break;
    }
    if(sunkenShip != -1){//Prints only if a ship was sunken
        fprintf(output_stream, "Player %d's %s has been sunk.\n", nextPlayer, shipArray[sunkenShip].name);
    }
    fprintf(output_stream, "\n");//Spacing
    fflush(output_stream); //Makes the log file "live" instead of only outputting once the game has completed
}

void print_stats_to_file(FILE *output_stream, Stats playerStats[], int winner){
    fprintf(output_stream, "The winner of this game was Player %d\n", winner);
    for(int i = 0; i < 2; ++i){ //Prints stats for each player
        fprintf(output_stream, "---------Player %d's Stats---------\n", i+1);
        fprintf(output_stream, "Player %d hit %d shots.\n", i+1, playerStats[i].hits);
        fprintf(output_stream, "Player %d missed %d shots.\n", i+1, playerStats[i].misses);
        fprintf(output_stream, "Player %d shot a total of %d times.\n", i+1, playerStats[i].shots);
        fprintf(output_stream, "Player %d had a ratio of %.2lf of hits to misses\n", i+1, playerStats[i].hitsToMiss);
        fprintf(output_stream, "Player %d hit a total of %.2lf%% of their shots\n", i+1, playerStats[i].hitsToShots);
        fprintf(output_stream, "\n");
    }
}