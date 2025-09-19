#include <iostream>
#include <vector>
#include <string>
using namespace std;

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

void build_board(char board[3][3])
{
    cout << "Welcome to tic tac toe!" << endl;
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

void player_move(char board[3][3], int count)
{
    if (count % 2 == 0)
    {
        cout << "player X enter a location to mark " << endl;
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
        board[r][c] = 'X';
        cout << "turn sucessfully placed" << endl;
        print_board(board);
        check_win(board);
        if (check_win(board))
            cout << "player X wins" << endl;
    }
    else
    {
        cout << "player O enter a location to mark " << endl;
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
        board[r][c] = 'O';
        cout << "turn sucessfully placed" << endl;
        print_board(board);
        check_win(board);
        if (check_win(board))
            cout << "player O wins" << endl;
    }
}

int main()
{
    char start_board[3][3];
    build_board(start_board);
    print_board(start_board);
    int count = 0;
    cout << endl;

    char board[3][3] = {
        {' ', ' ', ' '},
        {' ', ' ', ' '},
        {' ', ' ', ' '}};

    print_board(board);

    cout << check_win(board) << endl;

    while (!check_win(board))
    {
        player_move(board, count);
        count++;
        if (count == 9)
        {
            cout << "draw" << endl;
            break;
        }
    }
    cout << "thanks for playing";
}
