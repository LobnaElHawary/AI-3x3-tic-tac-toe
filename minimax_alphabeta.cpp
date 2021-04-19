#include <iostream>
#include <algorithm>
#include <vector>
#include <array>
#include <iostream>
using namespace std;

/////////////////////////// global vars ////////////////////////////////////
//global vars
#define WIN 1000
#define DRAW 0
#define LOSS -1000

#define AI_SYMBOL 'O'
#define PLAYER_SYMBOL 'X'
#define SPACE '-'

#define START_DEPTH 0

//NOTE: first coordinate represents the row, the second represents the column

// global vector of all possible winning cooridnates
//vector of coordinate vectors
vector<vector<pair<int, int>>> winning_coordinates
{
    
    // Every straight row
        //1st element of vector
        { make_pair(0, 0), make_pair(0, 1), make_pair(0, 2) },
        //2nd element of vector
        { make_pair(1, 0), make_pair(1, 1), make_pair(1, 2) },
        //3rd element of vector
        { make_pair(2, 0), make_pair(2, 1), make_pair(2, 2) },
    
        // Every straight column
        { make_pair(0, 0), make_pair(1, 0), make_pair(2, 0) },
        { make_pair(0, 1), make_pair(1, 1), make_pair(2, 1) },
        { make_pair(0, 2), make_pair(1, 2), make_pair(2, 2) },
    
        // Every straight diagonal
        { make_pair(0, 0), make_pair(1, 1), make_pair(2, 2) },
        { make_pair(2, 0), make_pair(1, 1), make_pair(0, 2) }
    ;
};

/////////////////////////// functions ////////////////////////////////////

//prints the input numbers and their placement
void print_placements();

// Print game state (win, draw, loss)
void print_game_state(int state);

// Print the current board state
void print_board(char board[3][3]);

//initialize board so that each board position has an empty space
void initialize_board(char board[3][3]);

//converts numbers from 1 - 9 to row and col numbers
void num_RowCol(int &row, int &col, int num);

// Get all available valid moves (spaces that are not occupied)
vector<pair<int, int>> valid_moves(char board[3][3]);

// Check if a position is occupied
bool is_occupied(char board[3][3], pair<int, int> pos);

// Get all board positions occupied by the given symbol
vector<pair<int, int>> occupied_positions(char board[3][3], char symbol);

// Check if the board is full
bool board_is_full(char board[3][3]);

// Check if the game has been won
bool game_is_won(vector<pair<int, int>> occupied_pos);

//gets the playing symbol of opponent of given player
char get_opponent_symbol(char symbol);

// Check if someone has won or lost
int get_board_state(char board[3][3], char symbol);

// Apply the minimax game optimization algorithm (alpha beta prunning)
pair<int,pair<int, int>> alpha_beta(char board[3][3], char symbol, int depth, int alpha, int beta);

// Apply the minimax game  algorithm
pair<int,pair<int, int>> minimax(char board[3][3], char symbol, int depth);

//let the computer play using minimac
void do_move_minimax(char board[3][3]);

//let the computer play using alpha beta
void do_move_alphabeta(char board [3][3]);

// Check if the game is finished
bool game_is_done(char board[3][3]);

//main function that manages the game by asking repeatedly for moves from the player and computer, and printing the board between moves
void play();


/////////////////////////// main /////////////////////////////////////////////

int main()
{
    play();
    return 0;
    
}

/////////////////////////// function definitions ///////////////////////////

void print_placements()
{
    cout << endl;
    cout << 1 << " | " << 2 << " | " << 3 << endl;
    cout << "----------" << endl;
    cout << 4 << " | " << 5 << " | " << 6 << endl;
    cout << "----------" << endl;
    cout << 7 << " | " << 8 << " | " << 9 << endl << endl;
}
// Print game state
void print_game_state(int state)
{
    if (state == WIN) { cout << "WIN" << endl; }
    else if (state == DRAW) { cout << "DRAW" << endl; }
    else if (state == LOSS) { cout << "LOSS" << endl; }
}


// Print the current board state
void print_board(char board[3][3])
{
    cout << endl;
    cout << board[0][0] << " | " << board[0][1] << " | " << board[0][2] << endl;
    cout << "----------" << endl;
    cout << board[1][0] << " | " << board[1][1] << " | " << board[1][2] << endl;
    cout << "----------" << endl;
    cout << board[2][0] << " | " << board[2][1] << " | " << board[2][2] << endl << endl;
}

//initializes all positions in the board = "-"
void initialize_board(char board[3][3])
{
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            board[i][j] = SPACE;
        }
    }
}

//converts input numbers from 1 - 9 to row and col numbers
void num_RowCol(int &row, int &col, int num)
{
    
    if((num >= 1) && (num <= 3))
    {
        row = 0;
    }
    
    if((num >= 4) && (num <= 6))
    {
        row = 1;
    }
    
    if((num >= 7) && (num <= 9))
    {
        row = 2;
    }
    
    col = (num - 1) % 3;
    
}

// Get all available valid moves (spaces that are not occupied)
vector<pair<int, int>> valid_moves(char board[3][3])
{
    vector<pair<int, int>> legal_moves;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (board[i][j] != AI_SYMBOL && board[i][j] != PLAYER_SYMBOL)
            {
                legal_moves.push_back(make_pair(i, j));
            }
        }
    }
    
    return legal_moves;
}

// Check if a position is occupied
bool is_occupied(char board[3][3], pair<int, int> pos)
{
    vector<pair<int, int>> legal_moves = valid_moves(board);
    
    for (int i = 0; i < legal_moves.size(); i++)
    {
        //if given coord is a valid_move (the position is empty), then it is not occupied
        if (pos.first == legal_moves[i].first && pos.second == legal_moves[i].second)
        {
            return false;
        }
    }
    //otherwise, it is occupied
    return true;
}

// Get all board positions occupied by the given symbol
vector<pair<int, int>> occupied_positions(char board[3][3], char symbol)
{
    vector<pair<int, int>> occupied_pos;
    
    //traversing 2D board array
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            //if there is a symbol in board position, then it is occupied
            if (symbol == board[i][j])
            {
                occupied_pos.push_back(make_pair(i, j));
            }
        }
    }
    
    return occupied_pos;
}

// check if the board is full
bool board_is_full(char board[3][3])
{
    //assume board is full
    bool full = true;
    
    //traverse 2D board array and check if there is an empty spot, if so mark full = false
    for(int i = 0 ; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            if((board[i][j]) == SPACE)
            {
                full = false;
            }
        }
    }
    
    return (full);
}

// check if the game has been won given the positions occupied
bool game_is_won(vector<pair<int, int>> occupied_pos)
{
    bool game_won;
    
    //travese through each possible coordinate that would mean someone won
    for (int i = 0; i < winning_coordinates.size(); i++)
    {
        game_won = true;
        vector<pair<int, int>> curr_win_state = winning_coordinates[i];
        //traverse through 3 coordinate line to see
        for (int j = 0; j < 3; j++)
        {

            if (!(find(begin(occupied_pos), end(occupied_pos), curr_win_state[j]) != end(occupied_pos)))
            {
                game_won = false;
                break;
            }
        }
        
        if (game_won)
        {
            break;
        }
    }
    return game_won;
}

//gets the symbol of the opponent
char get_opponent_symbol(char symbol)
{
    char opponent_symbol;
    if (symbol == PLAYER_SYMBOL)
    {
        opponent_symbol = AI_SYMBOL;
    }
    else
    {
        opponent_symbol = PLAYER_SYMBOL;
    }
    
    return opponent_symbol;
}

// Check if someone has won or lost
int get_board_state(char board[3][3], char symbol)
{
    
    char opponent_symbol = get_opponent_symbol(symbol);
    
    //check if input symbol player won, if so return win
    vector<pair<int, int>> occupied_pos = occupied_positions(board, symbol);
    
    bool is_won = game_is_won(occupied_pos);
    
    if (is_won)
    {
        return WIN;
    }
    
    //if input symbol's player did not win then it was either a draw or they lost
    //check if opponent lost or got a draw
    
    //if based the input's symbol's opponent won, then the player lost
    occupied_pos = occupied_positions(board, opponent_symbol);
    bool is_lost = game_is_won(occupied_pos);
    
    if (is_lost)
    {
        return LOSS;
    }
    
    //if neither case and the board is full, then there was a draw
    bool is_full = board_is_full(board);
    if (is_full)
    {
        return DRAW;
    }
    
    return DRAW;
    
}

// Apply the alpha beta prunning algorithm
//returns (best_score, (coordinates of the best move))
pair<int,pair<int, int>> alpha_beta(char board[3][3], char symbol, int depth, int alpha, int beta)
{
    // make the best move a move that cannot take place initially just as initialization
    pair<int, int> best_move = make_pair(-1, -1);
    int best_score;
    
    //AI's turn, then pick the min.
    if(symbol == AI_SYMBOL)
    {
        best_score = LOSS;
    }
    
    //Player
    else
    {
        best_score = WIN;
    }
    
    // If we hit a terminal state (leaf node), return the best score and move
    if (board_is_full(board) || DRAW != get_board_state(board, AI_SYMBOL))
    {
        //check if board state is LOSS or WIN
        best_score = get_board_state(board, AI_SYMBOL);
        return make_pair(best_score, best_move);
    }
    
    //legal_moves vector contains the empty coordinates on the board
    vector<pair<int, int>> legal_moves = valid_moves(board);
    
    //traverse through all the legal moves
    for (int i = 0; i < legal_moves.size(); i++)
    {
        
        pair<int, int> current_move = legal_moves[i];
        board[current_move.first][current_move.second] = symbol;
        
        // Maximizing player's turn
        if (symbol == AI_SYMBOL)
        {
            int score = alpha_beta(board, PLAYER_SYMBOL, depth + 1, alpha, beta).first;
            
            // Get the best scoring move
            if (best_score < score)
            {
                best_score = score - depth * 10;
                best_move = current_move;
                
                // Check if this branch's best move is worse than the best
                // option of a previously search branch. If it is, skip it
                alpha = max(alpha, best_score);
                board[current_move.first][current_move.second] = SPACE;
                if (beta <= alpha)
                {
                    break;
                }
            }
            
        } // Minimizing opponent's turn
        else
        {
            int score = alpha_beta(board, AI_SYMBOL, depth + 1, alpha, beta).first;
            
            if (best_score > score)
            {
                best_score = score + depth * 10;
                best_move = current_move;
                
                // Check if this branch's best move is worse than the best
                // option of a previously search branch. If it is, skip it
                beta = min(beta, best_score);
                board[current_move.first][current_move.second] = SPACE;
                if (beta <= alpha)
                {
                    break;
                }
            }
            
        }
        
        board[current_move.first][current_move.second] = SPACE; // Undo move
        
    }
    
    return make_pair(best_score, best_move);
}

// Apply the minimax game  algorithm
pair<int,pair<int, int>> minimax(char board[3][3], char symbol, int depth)
{
    // Initialize best move
    pair<int, int> best_move = make_pair(-1, -1);
  
    int best_score;
    if(symbol == AI_SYMBOL)
    {
        best_score = LOSS;
    }
    else
    {
        best_score = WIN;
    }
    
    // If we hit a terminal state (leaf node), return the best score and move
    if (board_is_full(board) || DRAW != get_board_state(board, AI_SYMBOL))
    {
        best_score = get_board_state(board, AI_SYMBOL);
        return make_pair(best_score, best_move);
    }
    
    vector<pair<int, int>> legal_moves = valid_moves(board);
    
    for (int i = 0; i < legal_moves.size(); i++)
    {
        pair<int, int> current_move = legal_moves[i];
        board[current_move.first][current_move.second] = symbol;
        
        // Maximizing player's turn
        if (symbol == AI_SYMBOL)
        {
            int score = minimax(board, PLAYER_SYMBOL, depth + 1).first;
            
            // Get the best scoring move
            if (best_score < score)
            {
                best_score = score - depth * 10;
                best_move = current_move;
                
                board[current_move.first][current_move.second] = SPACE;
            }
            
        }
        // Minimizing opponent's turn
        else
        {
            int score = minimax(board, AI_SYMBOL, depth + 1).first;
            
            if (best_score > score)
            {
                best_score = score + depth * 10;
                best_move = current_move;
                
            }
            
        }
        
        board[current_move.first][current_move.second] = SPACE; // Undo move
        
    }
    
    return make_pair(best_score, best_move);
}

//function to let the computer play using minimax
void do_move_minimax(char board[3][3])
{
    pair<int, pair<int, int>> do_move = minimax(board, AI_SYMBOL, START_DEPTH);
    board[do_move.second.first][do_move.second.second] = AI_SYMBOL;
}

//function to let the computer play using alphabeta prunning
void do_move_alphabeta(char board [3][3])
{
    pair<int, pair<int, int>> do_move = alpha_beta(board, AI_SYMBOL, START_DEPTH, LOSS, WIN);
    board[do_move.second.first][do_move.second.second] = AI_SYMBOL;
}

// Check if the game is finished
bool game_is_done(char board[3][3])
{
    if (board_is_full(board))
    {
        return true;
    }
    
    if (DRAW != get_board_state(board, AI_SYMBOL))
    {
        return true;
    }
    
    return false;
}

//main function that asks repeatedly for moves from the player and computer, and printing the board between moves
void play()
{
    //to symbolize if the user wants minimax used or alpha-beta-prunning
    int method;
    char board[3][3];
    
    //make all board spaces = "-"
    initialize_board(board);
    
    
    cout << "********************************\n\n\tTic Tac Toe!\n\n "<<"You = X\t Opponent = O\n"<<endl<<"********************************" <<endl << endl;
    
    //prompts the user for input as long as input is incorrect for which method to use
    do{
        cout<<" Enter 1 to use MiniMax and Enter 2 to use Alpha-Beta-Pruning";
        cin>>method;
    } while ((method != 1) && (method != 2));
    
    //shows the user what each input number corresponds to on board 2D array
    cout<<"The number you enter next from 1 - 9 corresponds to the following positions in the table: "<<endl;
    print_placements();
    
    //prints current state of the board
    cout<<"The board now: "<<endl;
    print_board(board);
    
    //as long as game is not done
    while (!game_is_done(board))
    {
        int row, col;
        int num;
        
        //prompts the user for input as long as input is incorrect
        do{
            cout<<"enter a number from 1 - 9"<<endl;;
            cout<<":"<<endl;
            cin>>num;
        }while((num < 1)||(num > 9));
        
        //converts the user input of num from 1 - 9 to coordinates in that corresponds to row, col coordinates
        num_RowCol(row, col, num);
        
        //in case user enters occupied position prompt error message
        if (is_occupied(board, make_pair(row, col)))
        {
            //error message
            cout << "The position (" << row << ", " << col << ") is occupied. Try another one..." << endl;
            //forces the execution of the next iteration of the loop
            continue;
        }
        else
        {
            //print board after player move
            cout<<"Your Move"<<endl;
            board[row][col] = PLAYER_SYMBOL;
            print_board(board);
        }
        
        //use minimax
        if(method == 1)
        {
            do_move_minimax(board);
        }
        
        //use alpha beta prunning
        else
        {
            do_move_alphabeta(board);
        }
        
        //print board after AI move
        cout<<"AI Move"<<endl;
        print_board(board);
    }
    
    cout << "********** GAME FINISHED **********" << endl << endl;
    
    int player_state = get_board_state(board, PLAYER_SYMBOL);
    //couts if the player won or lost or had a draw
    cout << "PLAYER "; print_game_state(player_state);
}
