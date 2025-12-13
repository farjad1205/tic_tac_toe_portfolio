#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <map>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <limits>

using namespace std;

// ==========================================
// HELPER FUNCTIONS (INPUT HANDLING)
// ==========================================

// Helper to safely get an integer from the user
// This replaces cin >> var; cin.clear(); logic
int get_int_input()
{
    string input;
    getline(cin, input);

    // Check if empty
    if (input.empty())
        return -999;

    // Check if it's a number
    try
    {
        return stoi(input);
    }
    catch (...)
    {
        return -999; // Return invalid code on failure
    }
}

// ==========================================
// CHARACTER CLASS
// ==========================================

class Character
{
public:
    string name;
    string role;
    char mark;
    int hp;
    int max_hp;
    int attack;
    int defense;
    int gold;
    int morality;

    Character()
    {
        name = "";
        role = "";
        mark = ' ';
        hp = 100;
        max_hp = 100;
        attack = 10;
        defense = 2;
        gold = 20;
        morality = 0;
    }

    Character(string n, string r, char s, int h, int a, int d, int g, int m)
    {
        name = n;
        role = r;
        mark = s;
        hp = h;
        max_hp = h;
        attack = a;
        defense = d;
        gold = g;
        morality = m;
    }

    void take_damage(int dmg)
    {
        hp -= dmg;
        if (hp < 0)
            hp = 0;
    }

    void heal(int amount)
    {
        hp += amount;
        if (hp > max_hp)
            hp = max_hp;
    }

    bool is_alive()
    {
        return hp > 0;
    }
};

// ==========================================
// UTILITIES & BOARD LOGIC
// ==========================================

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
                return true;
        }
    }
    return false;
}

void print_board(char board[3][3])
{
    cout << "\n---------" << endl;
    for (int i = 0; i < 3; i++)
    {
        cout << board[i][0] << " | " << board[i][1] << " | " << board[i][2] << endl;
        if (i < 2)
            cout << "---------" << endl;
    }
    cout << "---------" << endl;
}

void intro_board()
{
    cout << "Board Reference Keys:" << endl;
    cout << "1 | 2 | 3" << endl;
    cout << "---------" << endl;
    cout << "4 | 5 | 6" << endl;
    cout << "---------" << endl;
    cout << "7 | 8 | 9" << endl;
}

void get_coords(int loc, int &row, int &col)
{
    row = (loc - 1) / 3;
    col = (loc - 1) % 3;
}

char check_winner(char board[3][3])
{
    // Rows
    for (int i = 0; i < 3; i++)
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != ' ')
            return board[i][0];
    // Cols
    for (int i = 0; i < 3; i++)
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != ' ')
            return board[0][i];
    // Diagonals
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != ' ')
        return board[0][0];
    if (board[2][0] == board[1][1] && board[1][1] == board[0][2] && board[2][0] != ' ')
        return board[2][0];

    return ' ';
}

bool is_full(char board[3][3])
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == ' ')
                return false;
    return true;
}

// ==========================================
// ABILITIES
// ==========================================

bool perform_shift(char board[3][3])
{
    int from, to;
    cout << "Select a square to move FROM (1-9): ";
    from = get_int_input();

    if (from < 1 || from > 9)
    {
        cout << "Invalid input." << endl;
        return false;
    }

    int r1, c1;
    get_coords(from, r1, c1);
    if (board[r1][c1] == ' ')
    {
        cout << "That square is empty!" << endl;
        return false;
    }

    cout << "Select a square to move TO (1-9): ";
    to = get_int_input();

    if (to < 1 || to > 9)
    {
        cout << "Invalid input." << endl;
        return false;
    }

    int r2, c2;
    get_coords(to, r2, c2);
    if (board[r2][c2] != ' ')
    {
        cout << "That destination is occupied!" << endl;
        return false;
    }

    if (!isAdjacent(from, to))
    {
        cout << "Squares are not adjacent!" << endl;
        return false;
    }

    board[r2][c2] = board[r1][c1];
    board[r1][c1] = ' ';
    return true;
}

bool perform_swap(char board[3][3])
{
    int loc1, loc2;
    cout << "Select first square to swap (1-9): ";
    loc1 = get_int_input();
    cout << "Select second square to swap (1-9): ";
    loc2 = get_int_input();

    if (loc1 < 1 || loc1 > 9 || loc2 < 1 || loc2 > 9)
    {
        cout << "Invalid input." << endl;
        return false;
    }

    int r1, c1, r2, c2;
    get_coords(loc1, r1, c1);
    get_coords(loc2, r2, c2);
    char temp = board[r1][c1];
    board[r1][c1] = board[r2][c2];
    board[r2][c2] = temp;
    return true;
}

void ai_turn(char board[3][3], Character &enemy, int turn_count)
{
    cout << enemy.name << " is thinking..." << endl;
    bool ability_used = false;
    int roll = rand() % 100;

    if (roll < 30)
    {
        // Paladin/Boss Shift Logic
        if (enemy.role == "Paladin" || enemy.role == "Boss")
        {
            for (int i = 1; i <= 9 && !ability_used; i++)
            {
                int r1, c1;
                get_coords(i, r1, c1);
                if (board[r1][c1] != ' ')
                {
                    for (int j = 1; j <= 9 && !ability_used; j++)
                    {
                        if (isAdjacent(i, j))
                        {
                            int r2, c2;
                            get_coords(j, r2, c2);
                            if (board[r2][c2] == ' ')
                            {
                                board[r2][c2] = board[r1][c1];
                                board[r1][c1] = ' ';
                                cout << ">> " << enemy.name << " used SHIFT! Moved " << i << " to " << j << endl;
                                ability_used = true;
                            }
                        }
                    }
                }
            }
        }

        // Alchemist/Boss Swap Logic
        if (!ability_used && (enemy.role == "Alchemist" || enemy.role == "Boss"))
        {
            if (turn_count > 1)
            {
                int p1 = (rand() % 9) + 1;
                int p2 = (rand() % 9) + 1;
                int r1, c1, r2, c2;
                get_coords(p1, r1, c1);
                get_coords(p2, r2, c2);

                char temp = board[r1][c1];
                board[r1][c1] = board[r2][c2];
                board[r2][c2] = temp;
                cout << ">> " << enemy.name << " used SWAP! Swapped " << p1 << " and " << p2 << endl;
                ability_used = true;
            }
        }
    }

    if (!ability_used)
    {
        vector<int> open_spots;
        for (int i = 1; i <= 9; i++)
        {
            int r, c;
            get_coords(i, r, c);
            if (board[r][c] == ' ')
                open_spots.push_back(i);
        }

        if (!open_spots.empty())
        {
            int choice = open_spots[rand() % open_spots.size()];
            int r, c;
            get_coords(choice, r, c);
            board[r][c] = enemy.mark;
            cout << enemy.name << " placed mark at " << choice << endl;
        }
    }
}

// ==========================================
// GAME FUNCTIONS
// ==========================================

int play_round(Character &p, Character &e)
{
    char board[3][3] = {{' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '}};
    int turn = 0;
    bool p_turn = (rand() % 2 == 0);

    cout << (p_turn ? p.name : e.name) << " goes first!" << endl;
    intro_board();

    while (true)
    {
        print_board(board);
        char winner = check_winner(board);
        if (winner == p.mark)
            return 1;
        if (winner == e.mark)
            return 2;
        if (is_full(board))
            return 0;

        if (p_turn)
        {
            cout << "Your Turn (" << p.mark << "). Choose option:" << endl;
            cout << "1. Place Mark" << endl;
            cout << "2. Special Ability (" << p.role << ")" << endl;
            cout << "Choice: ";
            int choice = get_int_input();

            bool move_made = false;

            if (choice == 2)
            {
                if (p.role == "Paladin")
                {
                    if (perform_shift(board))
                        move_made = true;
                }
                else if (p.role == "Alchemist")
                {
                    if (perform_swap(board))
                        move_made = true;
                }
                else
                    cout << "No ability available!" << endl;
            }

            if (!move_made && choice != 2)
            {
                cout << "Enter location (1-9): ";
                int loc = get_int_input();

                if (loc < 1 || loc > 9)
                {
                    cout << "Invalid input." << endl;
                }
                else
                {
                    int r, c;
                    get_coords(loc, r, c);
                    if (board[r][c] == ' ')
                    {
                        board[r][c] = p.mark;
                        move_made = true;
                    }
                    else
                        cout << "Spot taken." << endl;
                }
            }

            if (move_made)
                p_turn = false;
        }
        else
        {
            ai_turn(board, e, turn);
            p_turn = true;
        }
        turn++;
    }
}

bool run_battle(Character &p, Character e)
{
    cout << "\n========================================" << endl;
    cout << "BATTLE START: " << p.name << " VS " << e.name << endl;
    cout << "Enemy Stats: HP " << e.hp << " | ATK " << e.attack << " | DEF " << e.defense << endl;
    cout << "========================================" << endl;

    while (p.is_alive() && e.is_alive())
    {
        cout << "\n--- New Round ---" << endl;
        cout << p.name << " HP: " << p.hp << "  VS  " << e.name << " HP: " << e.hp << endl;

        int result = play_round(p, e);

        if (result == 1)
        {
            int dmg = p.attack - e.defense;
            if (dmg < 1)
                dmg = 1;
            e.take_damage(dmg);
            cout << "\n!!! VICTORY !!! You dealt " << dmg << " damage!" << endl;
        }
        else if (result == 2)
        {
            int dmg = e.attack - p.defense;
            if (dmg < 1)
                dmg = 1;
            p.take_damage(dmg);
            cout << "\n!!! DEFEAT !!! You took " << dmg << " damage!" << endl;
        }
        else
        {
            cout << "\nDRAW! No damage dealt." << endl;
        }
    }

    if (!p.is_alive())
    {
        cout << "You have died..." << endl;
        return false;
    }

    int gold_loot = (rand() % 20) + 10;
    p.gold += gold_loot;
    cout << "Enemy defeated! You found " << gold_loot << " gold." << endl;
    return true;
}

void visit_shop(Character &p)
{
    cout << "\n--- MERCHANT ---" << endl;
    cout << "Merchant: 'Welcome! Buy something?'" << endl;
    while (true)
    {
        cout << "Your Gold: " << p.gold << endl;
        cout << "1. Health Potion (Heal 20) - 15g" << endl;
        cout << "2. Whetstone (+1 Atk)      - 30g" << endl;
        cout << "3. Iron Plate (+1 Def)     - 30g" << endl;
        cout << "4. Leave" << endl;
        cout << "Choice: ";

        int c = get_int_input();

        if (c == 4)
            break;

        if (c == 1)
        {
            if (p.gold >= 15)
            {
                p.gold -= 15;
                p.heal(20);
                cout << "Healed!" << endl;
            }
            else
                cout << "Not enough gold!" << endl;
        }
        else if (c == 2)
        {
            if (p.gold >= 30)
            {
                p.gold -= 30;
                p.attack++;
                cout << "Attack up!" << endl;
            }
            else
                cout << "Not enough gold!" << endl;
        }
        else if (c == 3)
        {
            if (p.gold >= 30)
            {
                p.gold -= 30;
                p.defense++;
                cout << "Defense up!" << endl;
            }
            else
                cout << "Not enough gold!" << endl;
        }
        else if (c != 4)
        {
            cout << "Invalid choice." << endl;
        }
    }
}

void run_event(Character &p, int stage)
{
    cout << "\n--- STORY EVENT ---" << endl;
    if (stage == 2)
    {
        cout << "You find a mysterious chest in the woods." << endl;
        cout << "1. Open it (Chance for gold or trap)" << endl;
        cout << "2. Leave it" << endl;
        int c = get_int_input();
        if (c == 1)
        {
            if (rand() % 2 == 0)
            {
                cout << "It was trapped! You take 10 damage." << endl;
                p.take_damage(10);
            }
            else
            {
                cout << "It contained 30 gold!" << endl;
                p.gold += 30;
            }
        }
    }
    else if (stage == 4)
    {
        cout << "You see a beggar." << endl;
        cout << "1. Give 10 gold (Good Karma)" << endl;
        cout << "2. Ignore (Neutral)" << endl;
        cout << "3. Rob him (Bad Karma, +10 gold)" << endl;
        int c = get_int_input();

        if (c == 1 && p.gold >= 10)
        {
            p.gold -= 10;
            p.morality++;
            cout << "You feel righteous." << endl;
        }
        else if (c == 3)
        {
            p.gold += 10;
            p.morality--;
            cout << "You stole his coins." << endl;
        }
        else
            cout << "You walk away." << endl;
    }
    else if (stage == 6)
    {
        cout << "You find a cursed sword stuck in a stone." << endl;
        cout << "1. Take it (+2 Atk, -5 HP)" << endl;
        cout << "2. Leave it" << endl;
        int c = get_int_input();
        if (c == 1)
        {
            p.attack += 2;
            p.take_damage(5);
            cout << "Power surges through you!" << endl;
        }
    }
}

void save_game(Character &p, int stage)
{
    ofstream file("save.txt");
    if (file.is_open())
    {
        file << stage << endl;
        file << p.name << endl;
        file << p.role << endl;
        file << p.mark << endl;
        file << p.hp << endl;
        file << p.max_hp << endl;
        file << p.attack << endl;
        file << p.defense << endl;
        file << p.gold << endl;
        file << p.morality << endl;
        file.close();
        cout << "[Game Saved]" << endl;
    }
}

bool load_game(Character &p, int &stage)
{
    ifstream file("save.txt");
    if (file.is_open())
    {
        file >> stage;
        file.ignore(); // Skip newline after stage number
        getline(file, p.name);
        getline(file, p.role);
        file >> p.mark;
        file >> p.hp;
        file >> p.max_hp;
        file >> p.attack;
        file >> p.defense;
        file >> p.gold;
        file >> p.morality;
        file.close();
        return true;
    }
    return false;
}

// ==========================================
// MAIN
// ==========================================

int main()
{
    srand(time(0));
    Character player;
    int stage = 0;

    cout << "=== TIC TAC TOE: RPG CAMPAIGN ===" << endl;
    cout << "1. New Game" << endl;
    cout << "2. Load Game" << endl;

    int start_choice = get_int_input();

    if (start_choice == 2 && load_game(player, stage))
    {
        cout << "Save loaded! Welcome back " << player.name << endl;
    }
    else
    {
        cout << "Enter Name: ";
        getline(cin, player.name);

        while (true)
        {
            cout << "Class (Paladin/Alchemist): ";
            getline(cin, player.role);

            // Convert to lowercase
            for (int i = 0; i < player.role.length(); i++)
                player.role[i] = tolower(player.role[i]);

            if (player.role == "paladin" || player.role == "alchemist")
                break;
            cout << "Invalid class." << endl;
        }
        player.role[0] = toupper(player.role[0]);

        string mark_input;
        while (true)
        {
            cout << "Enter mark (1 char): ";
            getline(cin, mark_input);
            if (mark_input.length() == 1)
            {
                player.mark = mark_input[0];
                break;
            }
            cout << "Invalid. Must be exactly one character." << endl;
        }

        player.hp = 100;
        player.max_hp = 100;
        player.attack = 10;
        player.defense = 2;
        player.gold = 20;
        player.morality = 0;
        stage = 0;
    }

    // Campaign Loop
    while (true)
    {
        save_game(player, stage);

        if (stage == 0)
        {
            Character slime("Slime", "Paladin", 'S', 40, 5, 0, 0, 0);
            if (!run_battle(player, slime))
                break;
        }
        else if (stage == 1)
            visit_shop(player);
        else if (stage == 2)
            run_event(player, stage);
        else if (stage == 3)
        {
            Character wolf("Dire Wolf", "Alchemist", 'W', 50, 7, 1, 0, 0);
            if (!run_battle(player, wolf))
                break;
        }
        else if (stage == 4)
            run_event(player, stage);
        else if (stage == 5)
        {
            Character goblin("Goblin", "Alchemist", 'G', 60, 8, 2, 0, 0);
            if (!run_battle(player, goblin))
                break;
        }
        else if (stage == 6)
            run_event(player, stage);
        else if (stage == 7)
        {
            Character knight("Dark Knight", "Paladin", 'K', 80, 12, 4, 0, 0);
            if (!run_battle(player, knight))
                break;
        }
        else if (stage == 8)
            visit_shop(player);
        else if (stage == 9)
        {
            cout << "\n!!! FINAL BOSS !!!" << endl;
            Character dragon("Dragon", "Boss", 'D', 150, 15, 5, 0, 0);
            if (run_battle(player, dragon))
            {
                cout << "\nCONGRATULATIONS!" << endl;
                cout << "You have finished the campaign." << endl;
                if (player.morality > 0)
                    cout << "Ending: The Saint." << endl;
                else if (player.morality < 0)
                    cout << "Ending: The Tyrant." << endl;
                else
                    cout << "Ending: The Wanderer." << endl;

                remove("save.txt");
                break;
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }

        stage++;
    }

    return 0;
}