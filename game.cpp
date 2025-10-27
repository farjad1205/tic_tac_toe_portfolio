#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <map>

using namespace std;

bool isAdjacent(int fromLoc, int toLoc)
{
    map<int, vector<int>> adj;
    adj[1] = {2, 4, 5};
    adj[2] = {1, 3, 4, 5, 6};
    adj[3] = {2, 5, 6};
    adj[4] = {1, 2, 5, 7, 8};
    adj[5] = {1, 2, 3, 4, 6, 7, 8, 9};
    adj[6] = {2, 3, 5, 8, 9};
    adj[7] = {4, 5, 8};
    adj[8] = {4, 5, 6, 7, 9};
    adj[9] = {5, 6, 8};

    if (adj.count(fromLoc))
    {
        for (int loc : adj[fromLoc])
        {
            if (loc == toLoc)
            {
                return true;
            }
        }
    }
    return false;
}

bool validate(string type, string input)
{
    if (type == "gamemode")
        if (input == "original" || input == "battle")
            return true;

    if (type == "class")
        if (input == "paladin" || input == "alchemist")
            return true;

    if (type == "move")
        if (input == "yes" || input == "no" || input == "1" || input == "2")
            return true;

    if (type == "loc")
    {
        try
        {
            int loc = stoi(input);
            if (loc >= 1 && loc <= 9)
                return true;
        }
        catch (const std::invalid_argument &ia)
        {
            return false;
        }
        catch (const std::out_of_range &oor)
        {
            return false;
        }
    }
    return false;
}

string start_menu()
{
    cout << "Welcome to Tic Tac Toe" << endl;
    cout << "Would you like to play original or battle mode? (please type exactly \"original\" or \"battle\")" << endl;
    string mode;
    cin >> mode;
    cin.ignore(10000, '\n');
    while (!validate("gamemode", mode))
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "please type exactly \"original\" or \"battle\" to proceed" << endl;
        cin >> mode;
        cin.ignore(10000, '\n');
    }
    return mode;
}

void print_board(char board[3][3])
{
    cout << "---------" << endl;
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
    cout << "---------" << endl;
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
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != ' ')
            return true;

    for (int i = 0; i < 3; i++)
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != ' ')
            return true;

    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != ' ')
        return true;
    else if (board[2][0] == board[1][1] && board[1][1] == board[0][2] && board[2][0] != ' ')
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

bool player_move(char board[3][3], int count, char p1, char p2, string c1, string c2)
{
    char currentPlayerMarker = (count % 2 == 0) ? p1 : p2;
    string currentClass = (count % 2 == 0) ? c1 : c2;
    int playerNumber = (count % 2 == 0) ? 1 : 2;

    bool takeRegularMove = true;

    if (!currentClass.empty())
    {
        cout << "\nPlayer " << playerNumber << " (" << currentPlayerMarker << ") - " << currentClass << "'s Turn" << endl;
        cout << "Choose your action:" << endl;
        cout << "1. Make a regular move" << endl;
        cout << "2. Use Special Ability (" << currentClass << ")" << endl;

        string actionChoice;
        cin >> actionChoice;
        cin.ignore(10000, '\n');
        while (actionChoice != "1" && actionChoice != "2")
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid selection. Please enter '1' or '2'." << endl;
            cin >> actionChoice;
            cin.ignore(10000, '\n');
        }

        if (actionChoice == "2")
        {
            if (currentClass == "alchemist")
            {
                if (count < 2)
                {
                    cout << "Invalid move: Cannot use Alchemist ability on turn 1 or 2 (not enough marks)." << endl;
                    cout << "Defaulting to a regular move." << endl;
                    takeRegularMove = true;
                }
                else
                {
                    while (true)
                    {
                        cout << "Alchemist Power: Swap two marks." << endl;
                        cout << "Select the first mark to swap (1-9):" << endl;
                        string loc1_str;
                        cin >> loc1_str;
                        cin.ignore(10000, '\n');
                        while (!validate("loc", loc1_str))
                        {
                            cin.clear();
                            cin.ignore(10000, '\n');
                            cout << "Please make a valid selection (1-9)" << endl;
                            cin >> loc1_str;
                            cin.ignore(10000, '\n');
                        }

                        cout << "Select the second mark to swap (1-9):" << endl;
                        string loc2_str;
                        cin >> loc2_str;
                        cin.ignore(10000, '\n');
                        while (!validate("loc", loc2_str))
                        {
                            cin.clear();
                            cin.ignore(10000, '\n');
                            cout << "Please make a valid selection (1-9)" << endl;
                            cin >> loc2_str;
                            cin.ignore(10000, '\n');
                        }

                        int r1 = (stoi(loc1_str) - 1) / 3;
                        int c1 = (stoi(loc1_str) - 1) % 3;
                        int r2 = (stoi(loc2_str) - 1) / 3;
                        int c2 = (stoi(loc2_str) - 1) % 3;

                        if (board[r1][c1] == board[r2][c2])
                        {
                            cout << "Invalid move: Cannot swap two identical marks (e.g., 'X' and 'X' or ' ' and ' '). Try again." << endl;
                        }
                        else
                        {
                            char temp = board[r1][c1];
                            board[r1][c1] = board[r2][c2];
                            board[r2][c2] = temp;
                            takeRegularMove = false;
                            break;
                        }
                    }
                }
            }
            else if (currentClass == "paladin")
            {
                if (count < 1)
                {
                    cout << "Invalid move: Cannot use Paladin ability on turn 1 (no marks on board)." << endl;
                    cout << "Defaulting to a regular move." << endl;
                    takeRegularMove = true;
                }
                else
                {
                    while (true)
                    {
                        cout << "Paladin Power: Shift any mark to an adjacent, empty square." << endl;
                        cout << "Select the mark you want to shift (1-9):" << endl;
                        string from_str;
                        cin >> from_str;
                        cin.ignore(10000, '\n');
                        while (!validate("loc", from_str) || board[(stoi(from_str) - 1) / 3][(stoi(from_str) - 1) % 3] == ' ')
                        {
                            cin.clear();
                            cin.ignore(10000, '\n');
                            cout << "Invalid selection. You must pick a square with a mark on it (1-9)." << endl;
                            cin >> from_str;
                            cin.ignore(10000, '\n');
                        }

                        cout << "Select the empty square to move it to (1-9):" << endl;
                        string to_str;
                        cin >> to_str;
                        cin.ignore(10000, '\n');

                        if (!validate("loc", to_str))
                        {
                            cout << "Invalid input. Must be 1-9. Try again." << endl;
                        }
                        else if (board[(stoi(to_str) - 1) / 3][(stoi(to_str) - 1) % 3] != ' ')
                        {
                            cout << "Invalid move: Destination square is not empty. Try again." << endl;
                        }
                        else if (!isAdjacent(stoi(from_str), stoi(to_str)))
                        {
                            cout << "Invalid move: Destination is not adjacent. Try again." << endl;
                        }
                        else
                        {
                            int r1 = (stoi(from_str) - 1) / 3;
                            int c1 = (stoi(from_str) - 1) % 3;
                            int r2 = (stoi(to_str) - 1) / 3;
                            int c2 = (stoi(to_str) - 1) % 3;

                            board[r2][c2] = board[r1][c1];
                            board[r1][c1] = ' ';
                            takeRegularMove = false;
                            break;
                        }
                    }
                }
            }
        }
    }

    if (takeRegularMove)
    {
        if (currentClass.empty())
        {
            cout << "\nPlayer " << playerNumber << " (" << currentPlayerMarker << ") enter a location to mark: " << endl;
        }
        else
        {
            cout << "Player " << playerNumber << ", enter a location for your regular move: " << endl;
        }

        int turn;
        while (true)
        {
            if (cin >> turn)
            {
                if (check_loc(board, turn))
                {
                    cin.ignore(10000, '\n'); // Clear rest of line
                    break;                   // Good input
                }
                else
                {
                    cout << "Invalid move. That space is taken or out of bounds (1-9)." << endl;
                    cin.ignore(10000, '\n'); // Clear rest of line
                }
            }
            else
            {
                cout << "Invalid input. Enter an empty number between 1 and 9." << endl;
                cin.clear();
                cin.ignore(10000, '\n'); // Clear bad input
            }
        }

        int r = (turn - 1) / 3;
        int c = (turn - 1) % 3;
        board[r][c] = currentPlayerMarker;
        cout << "Turn successfully placed." << endl;
        print_board(board);
    }
    else
    {
        cout << "Special move successful!" << endl;
        print_board(board);
    }

    if (check_win(board))
    {
        cout << "\nPlayer " << playerNumber << " (" << currentPlayerMarker << ") wins!" << endl;
        return true;
    }

    return false;
}

char p1, p2;
void char_select()
{
    cout << "In battle mode, you can choose your own marker (only the first character entered will be used)" << endl;
    cout << "Player 1 enter your mark: ";
    cin >> p1;
    cin.ignore(10000, '\n');
    cout << "Player 2 enter your mark: ";
    cin >> p2;
    cin.ignore(10000, '\n');
    while (p1 == p2)
    {
        cout << "Markers cannot be the same. Player 2, please enter a different mark: ";
        cin >> p2;
        cin.ignore(10000, '\n');
    }
    cout << endl;
}

string c1, c2;
void class_select()
{
    cout << "Player 1 please choose one of the following class types:" << endl;
    cout << " “paladin” or “alchemist” (must be spelled exactly)" << endl;
    cin >> c1;
    cin.ignore(10000, '\n');
    while (!validate("class", c1))
    {
        cout << "Check your spelling and try again" << endl;
        cin.clear();
        cin.ignore(10000, '\n');
        cin >> c1;
        cin.ignore(10000, '\n');
    }
    cout << "Player 1 (" << p1 << ") is " << c1 << endl;
    cin.clear();

    cout << "Player 2 please choose one of the following class types:" << endl;
    cout << " “paladin” or “alchemist” (must be spelled exactly)" << endl;
    cin >> c2;
    cin.ignore(10000, '\n');
    while (!validate("class", c2))
    {
        cout << "Check your spelling and try again" << endl;
        cin.clear();
        cin.ignore(10000, '\n');
        cin >> c2;
        cin.ignore(10000, '\n');
    }

    cout << "Player 2 (" << p2 << ") is " << c2 << endl;
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

    while (true)
    {
        if (player_move(board, count, 'X', 'O', "", ""))
        {
            break;
        }

        count++;

        if (count == 9)
        {
            cout << "Draw!" << endl;
            break;
        }
    }
}

void run_battle(char p1, char p2, string c1, string c2)
{
    cout << p1 << " (" << c1 << ") vs " << p2 << " (" << c2 << ")" << endl;
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

    while (true)
    {
        if (player_move(board, count, p1, p2, c1, c2))
        {
            break;
        }

        count++;

        if (count == 9)
        {
            cout << "Draw!" << endl;
            break;
        }
    }
}

int main()
{
    string mode = start_menu();

    if (mode == "original")
    {
        cout << "Player 1 is X and Player 2 is O" << endl;
        run_game();
    }
    else if (mode == "battle")
    {
        char_select();
        class_select();
        run_battle(p1, p2, c1, c2);
    }

    cout << "\nThanks for playing!" << endl;
}
