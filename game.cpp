#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
using namespace std;

bool validate(string type, string input)
{
    if (type == "gamemode")
        if (input == "original" || input == "battle")
            return true;

    if (type == "class")
        if (input == "paladin" || input == "chronomage" || input == "alchemist")
            return true;

    return false;
}

string start_menu()
{
    cout << "Welcome to Tic Tac Toe" << endl;
    cout << "Would you like to play original or battle mode? (please type exactly \"original\" or \"battle\")" << endl;
    string mode;
    cin >> mode;
    while (!validate("gamemode", mode))
    {
        cin.clear();
        cout << "please type exactly \"original\" or \"battle\" to proceed" << endl;
        cin >> mode;
    }

    return mode;
}

void print_board(char board[3][3])
{
    char display[5][5] = {
        {board[0][0], '|', board[0][1], '|', board[0][2]},
        {'-', '+', '-', '+', '-'},
        {board[1][0], '|', board[1][1], '|', board[1][2]},
        {'-', '+', '-', '+', '-'},
        {board[2][0], '|', board[2][1], '|', board[2][2]}};

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            cout << display[i][j] << " ";
        }
        cout << "\n";
    }
    return;
}

void intro_board(char board[3][3])
{
    cout << endl;
    cout << "Below is the locations of each square." << endl;
    cout << "If you would like to place your move in that location, enter that number on your turn." << endl;

    char direction_board[3][3] = {
        {'1', '2', '3'},
        {'4', '5', '6'},
        {'7', '8', '9'}};

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            board[i][j] = direction_board[i][j];
        }
    }
}

bool check_win(const char board[3][3])
{
    for (int i = 0; i < 3; i++)
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != ' ') // across
            return true;

    for (int i = 0; i < 3; i++)
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != ' ') // down
            return true;

    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != ' ') // across
        return true;
    else if (board[2][0] == board[1][1] && board[1][1] == board[0][2] && board[2][0] != ' ') // down
        return true;
    else
        return false;
}

bool check_loc(char board[3][3], int marker)
{
    if (marker > 9 || marker < 1)
        return false;

    int r = (marker - 1) / 3;
    int c = (marker - 1) % 3;
    if (board[r][c] != ' ')
        return false;
    else
        return true;
}

void player_move(char board[3][3], int count, char p1, char p2)
{
    if (count % 2 == 0)
    {
        cout << "player 1 enter a location to mark " << endl;
        int turn;
        cin >> turn;
        while (!check_loc(board, turn))
        {
            cout << "invalid input, enter a number between 1 and 9" << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            cin >> turn;
        }

        int r = (turn - 1) / 3;
        int c = (turn - 1) % 3;
        board[r][c] = p1;
        cout << "turn sucessfully placed" << endl;
        print_board(board);
        check_win(board);
        if (check_win(board))
            cout << "player 1 wins" << endl;
    }
    else
    {
        cout << "player 2 enter a location to mark " << endl;
        int turn;
        cin >> turn;
        while (!check_loc(board, turn))
        {
            cout << "invalid input, enter a number between 1 and 9" << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            cin >> turn;
        }

        int r = (turn - 1) / 3;
        int c = (turn - 1) % 3;
        board[r][c] = p2;
        cout << "turn sucessfully placed" << endl;
        print_board(board);
        check_win(board);
        if (check_win(board))
            cout << "player 2 wins" << endl;
    }
}

char p1, p2;
void char_select()
{
    cout << "In battle mode, you can choose your own marker (only the first character entered will be used)" << endl;
    cout << "player 1 enter your mark: ";
    char mark1;
    cin >> p1;
    cout << "player 2 enter your mark: ";
    char mark2;
    cin >> p2;
    cout << endl;
}

string c1, c2;
void class_select()
{
    cout << "player 1 please choose one of the following class types:" << endl;
    cout << " “paladin”, “alchemist”, or “chronomage” (must be spelled exactly)" << endl;
    cin >> c1;
    while (!validate("class", c1))
    {
        cout << "check your spelling and try again" << endl;
        cin.clear();
        cin >> c1;
    }
    cout << "player1 (" << p1 << ") is " << c1 << endl;
    cin.clear();

    cout << "player 2 please choose one of the following class types:" << endl;
    cout << " “paladin”, “alchemist”, or “chronomage” (must be spelled exactly)" << endl;
    cin >> c2;
    while (!validate("class", c2))
    {
        cout << "check your spelling and try again" << endl;
        cin.clear();
        cin >> c2;
    }

    cout << "player2 (" << p2 << ") is " << c1 << endl;
}

void run_game()
{
    char start_board[3][3];
    intro_board(start_board);
    print_board(start_board);
    int count = 0;
    cout << endl;

    char board[3][3] = {
        {' ', ' ', ' '},
        {' ', ' ', ' '},
        {' ', ' ', ' '}};

    print_board(board);

    while (!check_win(board))
    {
        player_move(board, count, 'X', 'O');
        count++;
        if (count == 10)
        {
            cout << "draw" << endl;
            break;
        }
    }
}

void run_battle(char p1, char p2, string c1, string c2)
{
    cout << p1 << p2 << endl;
    char start_board[3][3];
    intro_board(start_board);
    print_board(start_board);
    int count = 0;
    cout << endl;

    char board[3][3] = {
        {' ', ' ', ' '},
        {' ', ' ', ' '},
        {' ', ' ', ' '}};

    print_board(board);

    while (!check_win(board))
    {
        player_move(board, count, p1, p2);
        count++;
        if (count == 10)
        {
            cout << "draw" << endl;
            break;
        }
    }
}

int main()
{
    string mode = start_menu();

    if (mode == "original")
    {
        cout << "player 1 is X and player 2 is O" << endl;
        run_game();
    }
    else if (mode == "battle")
    {
        char_select();
        class_select();
        run_battle(p1, p2, c1, c2);
    }

    // cout << "thanks for playing";
}
