#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>
#include <Windows.h>

using namespace std;

bool stagenotcomp = true;
#pragma comment(lib, "winmm.lib")  

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);  
}

void movement(char ch, int& a, int& b, char arr[20][20]) {
    if ((ch == 'd' || ch == 'D' || ch == 77) && b < 18 && arr[a][b + 1] != '#' && arr[a][b + 1] != 'O') {
        b++;
    }
    else if ((ch == 's' || ch == 'S' || ch == 80) && a < 18 && arr[a + 1][b] != '#' && arr[a + 1][b] != 'O') {
        a++;
    }
    else if ((ch == 'w' || ch == 'W' || ch == 72) && a > 1 && arr[a - 1][b] != '#' && arr[a - 1][b] != 'O') {
        a--;
    }
    else if ((ch == 'a' || ch == 'A' || ch == 75) && b > 1 && arr[a][b - 1] != '#' && arr[a][b - 1] != 'O') {
        b--;
    }
}

void mainscreen(char arr[20][20], int score, int bombs) {

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0 });
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            if (arr[i][j] == '#') {
                setColor(5);
                cout << arr[i][j] << " ";
            }
            else if (arr[i][j] == 'O') {
                setColor(7);
                cout << arr[i][j] << " ";
            }
            else if (arr[i][j] == 'P') {
                setColor(9);
                cout << arr[i][j] << " ";
            }
            else if (arr[i][j] == 'E') {
                setColor(12);
                cout << arr[i][j] << " ";
            }
            else if (arr[i][j] == 'X') {
                setColor(14);
                cout << arr[i][j] << " ";
            }
            else if (arr[i][j] == '+') {
                setColor(3);
                cout << arr[i][j] << " ";
            }
            else {
                setColor(7);
                cout << arr[i][j] << " ";
            }
        }
        cout << endl;
    }

    cout << "\nYour Score is = " << score << endl;
    cout << "You have " << bombs << " bombs left\n";

}

int menu(int& digit) {
    setColor(3);
    cout << "\t\t\t\t-----> WELCOME TO BOMBERMAN <-----\n";
    cout << "\t\t\t\t     ========================\n\n";
    cout << "\t\t\t\t----->  Select Your Choice  <-----\n";
    cout << "\t\t\t\t\t | 1. START \n";
    cout << "\t\t\t\t\t | 2. How to Play \n";
    cout << "\t\t\t\t\t | 3. Top three Scorer \n";
    cout << "\t\t\t\t\t | 4. Credits \n";
    cout << "\t\t\t\t\t | 5. Exit \n\n\n";
    cout << "\t\t\t\t\t |> Enter Your Choice = ";
  
    while (!(cin >> digit)) {
        cout << endl;
        cin.clear();
        cin.ignore(1000, '\n');// \n --->enter
        cout << "\t\t |>Please enter an integer (btw if you not now what is integer then go and study it first) = ";
    }
    while (digit < 1 || digit > 5) {
        cout << "\t\t\t\t\t | Invalid Input |\n";
        cout << "\t\t\t\t\t |> Enter Your Choice again = ";
        cin >> digit;
    }
    return digit;
}

void filesystem(string name, int score) {

    ofstream write("Game.txt", ios::app);
    write << name << ' ' << score << endl;
    write.close();
}

void displayTopThree() {
    string names[100];
    int scores[100];
    int count = 0;
    ifstream read("Game.txt");

    while (read >> names[count] >> scores[count]) {
        count++;
    }
    read.close();

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (scores[j] < scores[j + 1]) {

                int tempScore = scores[j];
                scores[j] = scores[j + 1];
                scores[j + 1] = tempScore;

                string tempName = names[j];
                names[j] = names[j + 1];
                names[j + 1] = tempName;
            }
        }
    }
    if (count == 0) {
        cout << "\t\t\tNo scores available yet.\n";
    }
    if (count > 3) {
        cout << "\t\t\t\t\t\t  " << "Names\t\t\t      scores\n\n\n";
        for (int j = 0; j < 3; j++) {
            cout << "\t\t\t\t\t\t" << j + 1 << ".  " << names[j] << "\t\t      " << scores[j] << endl;
        }
    }
    else {
        cout << "\t\t\t\t\t\t  " << "Names\t\t\t      scores\n\n\n";
        for (int j = 0; j < count; j++) {
            cout << "\t\t\t\t\t\t" << j + 1 << ".  " << names[j] << "\t\t      " << scores[j] << endl;
        }
    }
    cout << "\n\n\n";
}

void bombsystem(int a, int b, int& x, int& y, char arr[20][20], bool& hitBomb, int& score,
    bool& enemy1Alive, int& enemy1_X, int& enemy1_Y,
    bool& enemy2Alive, int& enemy2_X, int& enemy2_Y,
    bool& enemy3Alive, int& enemy3_X, int& enemy3_Y,
    bool& enemy4Alive, int& enemy4_X, int& enemy4_Y
) {
    PlaySound(TEXT("bomb.wav"), NULL, SND_FILENAME | SND_ASYNC);  
    if (a == x && b == y) {
        hitBomb = true;
    }
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int r = x + i, c = y + j;
            if (r > 0 && r < 19 && c > 0 && c < 19 && arr[r][c] != '#') {
                if (arr[r][c] == 'E') {
                    if (enemy1Alive && r == enemy1_X && c == enemy1_Y) {
                        score += 50;
                        enemy1Alive = false;
                        arr[r][c] = ' ';
                        enemy1_X = -1;
                        enemy1_Y = -1;
                    }
                    else if (enemy2Alive && r == enemy2_X && c == enemy2_Y) {
                        score += 50;
                        enemy2Alive = false;
                        arr[r][c] = ' ';
                        enemy2_X = -1;
                        enemy2_Y = -1;
                    }
                    else if (enemy3Alive && r == enemy3_X && c == enemy3_Y) {
                        score += 50;
                        enemy3Alive = false;
                        arr[r][c] = ' ';
                        enemy3_X = -1;
                        enemy3_Y = -1;
                    }
                    else if (enemy4Alive && r == enemy4_X && c == enemy4_Y) {
                        score += 50;
                        enemy4Alive = false;
                        arr[r][c] = ' ';
                        enemy4_X = -1;
                        enemy4_Y = -1;
                    }
                }
                if (arr[r][c] == 'O') {
                    score += 10;
                }
                if (arr[r][c] == 'P') {
                    hitBomb = true;
                }
                arr[r][c] = 'X';
            }
        }
    }
}

void bombsystem(int a, int b, int& x, int& y, char arr[20][20], bool& hitBomb, int& score,
    bool& enemy1Alive, int& enemy1_X, int& enemy1_Y,
    bool& enemy2Alive, int& enemy2_X, int& enemy2_Y,
    bool& enemy3Alive, int& enemy3_X, int& enemy3_Y,
    bool& enemy4Alive, int& enemy4_X, int& enemy4_Y,
    bool& enemy5Alive, int& enemy5_X, int& enemy5_Y,
    bool& enemy6Alive, int& enemy6_X, int& enemy6_Y
) {
    PlaySound(TEXT("bomb.wav"), NULL, SND_FILENAME | SND_ASYNC);
    if (a == x && b == y) {
        hitBomb = true;
    }
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int r = x + i, c = y + j;

            if (r > 0 && r < 19 && c > 0 && c < 19 && arr[r][c] != '#') {
                if (arr[r][c] == 'E') {
                    if (enemy1Alive && r == enemy1_X && c == enemy1_Y) {
                        score += 100;
                        enemy1Alive = false;
                        arr[r][c] = ' ';
                        enemy1_X = -1;
                        enemy1_Y = -1;
                    }
                    else if (enemy2Alive && r == enemy2_X && c == enemy2_Y) {
                        score += 100;
                        enemy2Alive = false;
                        arr[r][c] = ' ';
                        enemy2_X = -1;
                        enemy2_Y = -1;
                    }
                    else if (enemy3Alive && r == enemy3_X && c == enemy3_Y) {
                        score += 100;
                        enemy3Alive = false;
                        arr[r][c] = ' ';
                        enemy3_X = -1;
                        enemy3_Y = -1;
                    }
                    else if (enemy4Alive && r == enemy4_X && c == enemy4_Y) {
                        score += 100;
                        enemy4Alive = false;
                        arr[r][c] = ' ';
                        enemy4_X = -1;
                        enemy4_Y = -1;
                    }
                    else if (enemy5Alive && r == enemy5_X && c == enemy5_Y) {
                        score += 100;
                        enemy5Alive = false;
                        arr[r][c] = ' ';
                        enemy5_X = -1;
                        enemy5_Y = -1;
                    }
                    else if (enemy6Alive && r == enemy6_X && c == enemy6_Y) {
                        score += 100;
                        enemy6Alive = false;
                        arr[r][c] = ' ';
                        enemy6_X = -1;
                        enemy6_Y = -1;
                    }
                }
                if (arr[r][c] == 'O') {
                    score += 10;
                }
                if (arr[r][c] == 'P') {
                    hitBomb = true;
                }
                arr[r][c] = 'X';
            }

        }
    }
}

int bombHit(int& choice2, int score) {
    PlaySound(TEXT("dead.wav"), NULL, SND_FILENAME | SND_ASYNC); 

    setColor(14);
    cout << "\n\n\t\t\t  | :-( Player died (2 min silence plz). May his Soul rest in Peace|\n\n";
    cout << "\n\n\t\t\t\t\t  | Your score is = " << score << " |\n\n";
    cout << "\n\n\t\t\t  | His Friend Wants to take revenge from Enemy |\n\n";
    cout << "\t\t\t\t\t  | 1. Lets go to take Revenge \n\n";
    cout << "\t\t\t\t\t  | 2. Not ready yet \n\n";
    cout << "\t\t\t\t\t  | Enter Your Choice = ";
    while (!(cin >> choice2)) {
        cout << endl;
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "\t\t |>Please enter an integer (btw if you not now what is integer then go and study it first) = ";
    }
    return choice2;
}

void clearbomb(int& x, int& y, char arr[20][20]) {
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int r = x + i, c = y + j;
            if (r > 0 && r < 19 && c > 0 && c < 19 && arr[r][c] != '#') {
                arr[r][c] = ' ';
            }
        }
    }
    x = 0; y = 0;
}

void enemymovement(int ch, int& x, int& y, int a, int b, char arr[20][20], bool& hitEnemy) {
    if (x == a && y == b)
        hitEnemy = true;
    if (ch == 1 && y < 18 && arr[x][y + 1] != '#' && arr[x][y + 1] != 'O' && arr[x][y + 1] != '+') {
        y++;
        if (x == a && y == b)
            hitEnemy = true;
    }
    else if (ch == 2 && x < 18 && arr[x + 1][y] != '#' && arr[x + 1][y] != 'O' && arr[x + 1][y] != '+') {
        x++;
        if (x == a && y == b)
            hitEnemy = true;
    }
    else if (ch == 3 && x > 1 && arr[x - 1][y] != '#' && arr[x - 1][y] != 'O' && arr[x - 1][y] != '+') {
        x--;
        if (x == a && y == b)
            hitEnemy = true;
    }
    else if (ch == 4 && y > 1 && arr[x][y - 1] != '#' && arr[x][y - 1] != 'O' && arr[x][y - 1] != '+') {
        y--;
        if (x == a && y == b)
            hitEnemy = true;
    }
    else if (ch == 5 && y > 1 && arr[x][y - 1] != '#' && arr[x][y - 1] != 'O' && arr[x][y - 1] != '+') {
        y--;
        if (x == a && y == b)
            hitEnemy = true;
    }
    else if (ch == 6 && y < 18 && arr[x][y + 1] != '#' && arr[x][y + 1] != 'O' && arr[x][y + 1] != '+') {
        y++;
        if (x == a && y == b)
            hitEnemy = true;
    }

    arr[x][y] = 'E';
}

int win(int& choice4, int score) {

    for (int i = 0; i < 30; i++) {
        cout << "-";
        if (i == 29) {
            cout << ">|";
        }
        Sleep(100);
    }
    cout << " CONGRATULATIONS YOU WON ";
    for (int i = 0; i < 30; i++) {
        if (i == 0) {
            cout << "|<";
        }
        cout << "-";
        Sleep(100);
    }
    if (stagenotcomp) {
        cout << "\n\n1. Move to stage 2\n";
        cout << "2. Exit";
    }
    else {
        cout << "\n\n1. Replay\n";
        cout << "2. Exit";
    }

    cout << "\n\nEnter your choice = ";
    while (!(cin >> choice4)) {
        cout << endl;
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "\t\t |>Please enter an integer (btw if you not now what is integer then go and study it first) = ";
    }

    return choice4;

}

void stage1(char arr[][20]) {
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            if (i == 0 || i == 19 || j == 0 || j == 19 || (i % 2 == 0 && j % 2 == 0)) {
                arr[i][j] = '#';
            }
            else if (i % 3 == 0)
                arr[i][j] = 'O';
            else if (i % 2 == 0 && j > 9)
                arr[i][j] = 'O';
            else {
                arr[i][j] = ' ';
            }
        }
    }
}

void stage2map(char arr[][20]) {
    char map2[20][20] = {
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
        {'#', ' ', 'O', 'O', 'O', 'O', 'O', ' ', ' ', ' ', 'O', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', ' ', 'O', 'O', '#', 'O', ' ', ' ', '#', ' ', ' ', 'O', 'O', 'O', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', 'O', ' ', ' ', ' ', 'O', 'O', 'O', 'O', '#', ' ', ' ', ' ', 'O', 'O', 'O', ' ', ' ', '#'},
        {'#', ' ', ' ', 'O', ' ', ' ', ' ', '#', 'O', '#', '#', ' ', ' ', ' ', ' ', ' ', ' ', 'O', ' ', '#'},
        {'#', ' ', ' ', ' ', ' ', ' ', ' ', '#', 'O', ' ', ' ', 'O', 'O', 'O', ' ', ' ', '#', '#', ' ', '#'},
        {'#', ' ', ' ', 'O', 'O', '#', 'O', '#', '#', ' ', ' ', ' ', '#', 'O', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', '#', '#', ' ', 'O', ' ', 'O', 'O', 'O', ' ', ' ', 'O', ' ', 'O', 'O', 'O', 'O', '#', '#'},
        {'#', 'O', '#', 'O', ' ', ' ', ' ', '#', ' ', 'O', 'O', ' ', '#', 'O', '#', '#', 'O', 'O', 'O', '#'},
        {'#', ' ', 'O', 'O', 'O', ' ', ' ', ' ', 'O', 'O', 'O', 'O', ' ', ' ', 'O', ' ', 'O', ' ', ' ', '#'},
        {'#', '#', 'O', '#', 'O', '#', 'O', ' ', ' ', 'O', 'O', 'O', ' ', 'O', 'O', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', 'O', 'O', 'O', ' ', ' ', 'O', 'O', '0', 'O', 'O', '#', ' ', ' ', ' ', ' ', 'O', ' ', '#'},
        {'#', ' ', 'O', 'O', 'O', ' ', 'O', 'O', 'O', ' ', ' ', ' ', 'O', 'O', ' ', ' ', 'O', ' ', ' ', '#'},
        {'#', ' ', 'O', 'O', 'O', ' ', '#', 'O', '#', '#', ' ', ' ', 'O', 'O', ' ', ' ', ' ', ' ', ' ', '#'},
        {'#', ' ', '#', 'O', '#', '#', ' ', ' ', 'O', 'O', ' ', ' ', 'O', 'O', ' ', ' ', 'O', ' ', ' ', '#'},
        {'#', ' ', 'O', 'O', 'O', ' ', ' ', 'O', 'O', ' ', ' ', ' ', 'O', 'O', ' ', ' ', 'O', ' ', ' ', '#'},
        {'#', ' ', 'O', 'O', '#', 'O', '#', '#', 'O', 'O', ' ', ' ', 'O', 'O', ' ', ' ', 'O', ' ', 'O', '#'},
        {'#', '#', 'O', '#', ' ', ' ', 'O', 'O', ' ', 'O', '#', 'O', '#', '#', ' ', ' ', ' ', 'O', ' ', '#'},
        {'#', '#', 'O', '#', ' ', ' ', 'O', 'O', ' ', 'O', '#', 'O', '#', '#', ' ', ' ', ' ', 'O', ' ', '#'},
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
    };
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            arr[i][j] = map2[i][j];
        }
    }
}

void bombPlus(char arr[20][20], int& x, int& y, bool& plusNotplaced) {
    do {
        x = rand() % 16 + 2;
        y = rand() % 16 + 2;
    } while (arr[x][y] == 'P' || arr[x][y] == 'E' || arr[x][y] == 'O' || arr[x][y] == '#');
    arr[x][y] = '+';
    plusNotplaced = false;
}

void powerup(char arr[20][20], int& x, int& y, bool& plusNotplaced) {
    do {
        x = rand() % 16 + 2;
        y = rand() % 16 + 2;
    } while (arr[x][y] == 'P' || arr[x][y] == 'E' || arr[x][y] == 'O' || arr[x][y] == '#' || arr[x][y] == '+');
    arr[x][y] = '?';
    plusNotplaced = false;
}

void functions(bool& plusfunction, bool& plusNotplaced, char arr[20][20], int& do_timer, int a, int b,
    int& x, int& y, int& bombs, int& start) {
    if (plusfunction) {
        if (plusNotplaced) {
            bombPlus(arr, x, y, plusNotplaced);
            do_timer = 0;
        }

        if (!(plusNotplaced)) {
            do_timer++;

            if (a == x && b == y) {
                PlaySound(TEXT("bombplus.wav"), NULL, SND_FILENAME | SND_ASYNC);  

                bombs++;
                x = -1; y = -1;
            }
            if (do_timer == 220) {
                do_timer = 0;
                arr[x][y] = ' ';
                x = -1; y = -1;
                plusNotplaced = true;
                plusfunction = false;
            }

        }
    }
    else {
        if (start == 200) {
            plusfunction = true;
            start = 0;
        }
        start++;
    }
}

void powerFunctions(bool& powerfunction, bool& powerNotplaced, char arr[20][20], int& power_timer, int a, int b,
    int& x, int& y, bool& bomb, int& start) {
    if (powerfunction) {
        if (powerNotplaced) {
            powerup(arr, x, y, powerNotplaced);
            power_timer = 0;
        }

        if (!(powerNotplaced)) {
            power_timer++;

            if (a == x && b == y) {
                PlaySound(TEXT("powerup.wav"), NULL, SND_FILENAME | SND_ASYNC);  

                bomb = true;
                x = -1; y = -1;
            }
            if (power_timer == 170) {
                power_timer = 0;
                arr[x][y] = ' ';
                x = -1; y = -1;
                powerNotplaced = true;
                powerfunction = false;
            }

        }
    }
    else {
        if (start == 600) {
            powerfunction = true;
            start = 0;
        }
        start++;
    }
}


void poweraction(int a, int b, char arr[20][20], int& score,
    bool& enemy1Alive, int& enemy1_X, int& enemy1_Y,
    bool& enemy2Alive, int& enemy2_X, int& enemy2_Y,
    bool& enemy3Alive, int& enemy3_X, int& enemy3_Y,
    bool& enemy4Alive, int& enemy4_X, int& enemy4_Y) {
    PlaySound(TEXT("bomb.wav"), NULL, SND_FILENAME | SND_ASYNC);  

    for (int i = 1; i <= 20; i++) {
        int r = a - i;
        if (r == 0) {
            break;
        }
        if (arr[r][b] != '#') {
            if (arr[r][b] == 'E') {
                if (enemy1Alive && r == enemy1_X && b == enemy1_Y) {
                    score += 50;
                    enemy1Alive = false;
                    arr[r][b] = ' ';
                    enemy1_X = -1;
                    enemy1_Y = -1;
                }
                else if (enemy2Alive && r == enemy2_X && b == enemy2_Y) {
                    score += 50;
                    enemy2Alive = false;
                    arr[r][b] = ' ';
                    enemy2_X = -1;
                    enemy2_Y = -1;
                }
                else if (enemy3Alive && r == enemy3_X && b == enemy3_Y) {
                    score += 50;
                    enemy3Alive = false;
                    arr[r][b] = ' ';
                    enemy3_X = -1;
                    enemy3_Y = -1;
                }
                else if (enemy4Alive && r == enemy4_X && b == enemy4_Y) {
                    score += 50;
                    enemy4Alive = false;
                    arr[r][b] = ' ';
                    enemy4_X = -1;
                    enemy4_Y = -1;
                }
            }
            if (arr[r][b] == 'O') {
                score += 10;
            }
            arr[r][b] = 'X';
        }
    }
    for (int i = 1; i <= 20; i++) {
        int r = a + i;
        if (r == 20) {
            break;
        }
        if (arr[r][b] != '#') {
            if (arr[r][b] == 'E') {
                if (enemy1Alive && r == enemy1_X && b == enemy1_Y) {
                    score += 50;
                    enemy1Alive = false;
                    arr[r][b] = ' ';
                    enemy1_X = -1;
                    enemy1_Y = -1;
                }
                else if (enemy2Alive && r == enemy2_X && b == enemy2_Y) {
                    score += 50;
                    enemy2Alive = false;
                    arr[r][b] = ' ';
                    enemy2_X = -1;
                    enemy2_Y = -1;
                }
                else if (enemy3Alive && r == enemy3_X && b == enemy3_Y) {
                    score += 50;
                    enemy3Alive = false;
                    arr[r][b] = ' ';
                    enemy3_X = -1;
                    enemy3_Y = -1;
                }
                else if (enemy4Alive && r == enemy4_X && b == enemy4_Y) {
                    score += 50;
                    enemy4Alive = false;
                    arr[r][b] = ' ';
                    enemy4_X = -1;
                    enemy4_Y = -1;
                }
            }
            if (arr[r][b] == 'O') {
                score += 10;
            }
            arr[r][b] = 'X';
        }
    }
    for (int i = 1; i <= 20; i++) {
        int r = b - i;
        if (r == 0) {
            break;
        }
        if (arr[a][r] != '#') {
            if (arr[a][r] == 'E') {
                if (enemy1Alive && a == enemy1_X && r == enemy1_Y) {
                    score += 50;
                    enemy1Alive = false;
                    arr[a][r] = ' ';
                    enemy1_X = -1;
                    enemy1_Y = -1;
                }
                else if (enemy2Alive && a == enemy2_X && r == enemy2_Y) {
                    score += 50;
                    enemy2Alive = false;
                    arr[a][r] = ' ';
                    enemy2_X = -1;
                    enemy2_Y = -1;
                }
                else if (enemy3Alive && a == enemy3_X && r == enemy3_Y) {
                    score += 50;
                    enemy3Alive = false;
                    arr[a][r] = ' ';
                    enemy3_X = -1;
                    enemy3_Y = -1;
                }
                else if (enemy4Alive && a == enemy4_X && r == enemy4_Y) {
                    score += 50;
                    enemy4Alive = false;
                    arr[a][r] = ' ';
                    enemy4_X = -1;
                    enemy4_Y = -1;
                }
            }
            if (arr[a][r] == 'O') {
                score += 10;
            }
            arr[a][r] = 'X';
        }
    }
    for (int i = 1; i <= 20; i++) {
        int r = b + i;
        if (r == 20) {
            break;
        }
        if (arr[a][r] != '#') {
            if (arr[a][r] == 'E') {
                if (enemy1Alive && a == enemy1_X && r == enemy1_Y) {
                    score += 50;
                    enemy1Alive = false;
                    arr[a][r] = ' ';
                    enemy1_X = -1;
                    enemy1_Y = -1;
                }
                else if (enemy2Alive && a == enemy2_X && r == enemy2_Y) {
                    score += 50;
                    enemy2Alive = false;
                    arr[a][r] = ' ';
                    enemy2_X = -1;
                    enemy2_Y = -1;
                }
                else if (enemy3Alive && a == enemy3_X && r == enemy3_Y) {
                    score += 50;
                    enemy3Alive = false;
                    arr[a][r] = ' ';
                    enemy3_X = -1;
                    enemy3_Y = -1;
                }
                else if (enemy4Alive && a == enemy4_X && r == enemy4_Y) {
                    score += 50;
                    enemy4Alive = false;
                    arr[a][r] = ' ';
                    enemy4_X = -1;
                    enemy4_Y = -1;
                }
            }
            if (arr[a][r] == 'O') {
                score += 10;
            }
            arr[a][r] = 'X';
        }
    }
}

void poweraction(int a, int b, char arr[20][20], int& score,
    bool& enemy1Alive, int& enemy1_X, int& enemy1_Y,
    bool& enemy2Alive, int& enemy2_X, int& enemy2_Y,
    bool& enemy3Alive, int& enemy3_X, int& enemy3_Y,
    bool& enemy4Alive, int& enemy4_X, int& enemy4_Y,
    bool& enemy5Alive, int& enemy5_X, int& enemy5_Y,
    bool& enemy6Alive, int& enemy6_X, int& enemy6_Y) {
    PlaySound(TEXT("bomb.wav"), NULL, SND_FILENAME | SND_ASYNC); 

    for (int i = 1; i <= 20; i++) {
        int r = a - i;
        if (r == 0) {
            break;
        }
        
        if (arr[r][b] != '#') {
            if (arr[r][b] == 'E') {
                if (enemy1Alive && r == enemy1_X && b == enemy1_Y) {
                    score += 100;
                    enemy1Alive = false;
                    arr[r][b] = ' ';
                    enemy1_X = -1;
                    enemy1_Y = -1;
                }
                else if (enemy2Alive && r == enemy2_X && b == enemy2_Y) {
                    score += 100;
                    enemy2Alive = false;
                    arr[r][b] = ' ';
                    enemy2_X = -1;
                    enemy2_Y = -1;
                }
                else if (enemy3Alive && r == enemy3_X && b == enemy3_Y) {
                    score += 100;
                    enemy3Alive = false;
                    arr[r][b] = ' ';
                    enemy3_X = -1;
                    enemy3_Y = -1;
                }
                else if (enemy4Alive && r == enemy4_X && b == enemy4_Y) {
                    score += 100;
                    enemy4Alive = false;
                    arr[r][b] = ' ';
                    enemy4_X = -1;
                    enemy4_Y = -1;
                }
                else if (enemy5Alive && r == enemy5_X && b == enemy5_Y) {
                    score += 100;
                    enemy5Alive = false;
                    arr[r][b] = ' ';
                    enemy5_X = -1;
                    enemy5_Y = -1;
                }
                else if (enemy6Alive && r == enemy6_X && b == enemy6_Y) {
                    score += 100;
                    enemy6Alive = false;
                    arr[r][b] = ' ';
                    enemy6_X = -1;
                    enemy6_Y = -1;
                }
            }
            if (arr[r][b] == 'O') {
                score += 10;
            }
            arr[r][b] = 'X';
        }
        
    }
    for (int i = 1; i <= 20; i++) {
        int r = a + i;
        if (r == 20) {
            break;
        }
        if (arr[r][b] != '#') {
            if (arr[r][b] == 'E') {
                if (enemy1Alive && r == enemy1_X && b == enemy1_Y) {
                    score += 100;
                    enemy1Alive = false;
                    arr[r][b] = ' ';
                    enemy1_X = -1;
                    enemy1_Y = -1;
                }
                else if (enemy2Alive && r == enemy2_X && b == enemy2_Y) {
                    score += 100;
                    enemy2Alive = false;
                    arr[r][b] = ' ';
                    enemy2_X = -1;
                    enemy2_Y = -1;
                }
                else if (enemy3Alive && r == enemy3_X && b == enemy3_Y) {
                    score += 100;
                    enemy3Alive = false;
                    arr[r][b] = ' ';
                    enemy3_X = -1;
                    enemy3_Y = -1;
                }
                else if (enemy4Alive && r == enemy4_X && b == enemy4_Y) {
                    score += 100;
                    enemy4Alive = false;
                    arr[r][b] = ' ';
                    enemy4_X = -1;
                    enemy4_Y = -1;
                }
                else if (enemy5Alive && r == enemy5_X && b == enemy5_Y) {
                    score += 100;
                    enemy5Alive = false;
                    arr[r][b] = ' ';
                    enemy5_X = -1;
                    enemy5_Y = -1;
                }
                else if (enemy6Alive && r == enemy6_X && b == enemy6_Y) {
                    score += 100;
                    enemy6Alive = false;
                    arr[r][b] = ' ';
                    enemy6_X = -1;
                    enemy6_Y = -1;
                }
            }
            if (arr[r][b] == 'O') {
                score += 10;
            }
           
            arr[r][b] = 'X';
        }
    }
    for (int i = 1; i <= 20; i++) {
        int r = b - i;
        if (r == 0) {
            break;
        }
        if (arr[a][r] != '#') {
            if (arr[a][r] == 'E') {
                if (enemy1Alive && a == enemy1_X && r == enemy1_Y) {
                    score += 100;
                    enemy1Alive = false;
                    arr[a][r] = ' ';
                    enemy1_X = -1;
                    enemy1_Y = -1;
                }
                else if (enemy2Alive && a == enemy2_X && r == enemy2_Y) {
                    score += 100;
                    enemy2Alive = false;
                    arr[a][r] = ' ';
                    enemy2_X = -1;
                    enemy2_Y = -1;
                }
                else if (enemy3Alive && a == enemy3_X && r == enemy3_Y) {
                    score += 100;
                    enemy3Alive = false;
                    arr[a][r] = ' ';
                    enemy3_X = -1;
                    enemy3_Y = -1;
                }
                else if (enemy4Alive && a == enemy4_X && r == enemy4_Y) {
                    score += 100;
                    enemy4Alive = false;
                    arr[a][r] = ' ';
                    enemy4_X = -1;
                    enemy4_Y = -1;
                }
                else if (enemy5Alive && a == enemy5_X && r == enemy5_Y) {
                    score += 100;
                    enemy5Alive = false;
                    arr[a][r] = ' ';
                    enemy5_X = -1;
                    enemy5_Y = -1;
                }
                else if (enemy6Alive && a == enemy6_X && r == enemy6_Y) {
                    score += 100;
                    enemy6Alive = false;
                    arr[a][r] = ' ';
                    enemy6_X = -1;
                    enemy6_Y = -1;
                }
            }
            if (arr[a][r] == 'O') {
                score += 10;
            }
            
            arr[a][r] = 'X';
        }
    }
    for (int i = 1; i <= 20; i++) {
        int r = b + i;
        if (r == 20) {
            break;
        }
        
        if (arr[a][r] != '#') {
            if (arr[a][r] == 'E') {
                if (enemy1Alive && a == enemy1_X && r == enemy1_Y) {
                    score += 100;
                    enemy1Alive = false;
                    arr[a][r] = ' ';
                    enemy1_X = -1;
                    enemy1_Y = -1;
                }
                else if (enemy2Alive && a == enemy2_X && r == enemy2_Y) {
                    score += 100;
                    enemy2Alive = false;
                    arr[a][r] = ' ';
                    enemy2_X = -1;
                    enemy2_Y = -1;
                }
                else if (enemy3Alive && a == enemy3_X && r == enemy3_Y) {
                    score += 100;
                    enemy3Alive = false;
                    arr[a][r] = ' ';
                    enemy3_X = -1;
                    enemy3_Y = -1;
                }
                else if (enemy4Alive && a == enemy4_X && r == enemy4_Y) {
                    score += 100;
                    enemy4Alive = false;
                    arr[a][r] = ' ';
                    enemy4_X = -1;
                    enemy4_Y = -1;
                }
                else if (enemy5Alive && a == enemy5_X && r == enemy5_Y) {
                    score += 100;
                    enemy5Alive = false;
                    arr[a][r] = ' ';
                    enemy5_X = -1;
                    enemy5_Y = -1;
                }
                else if (enemy6Alive && a == enemy6_X && r == enemy6_Y) {
                    score += 100;
                    enemy6Alive = false;
                    arr[a][r] = ' ';
                    enemy6_X = -1;
                    enemy6_Y = -1;
                }
            }
            if (arr[a][r] == 'O') {
                score += 10;
            }
          
            arr[a][r] = 'X';
        }
    }
}

void clearpoweraction(int a, int b, char arr[20][20]) {
    for (int i = 1; i <= 20; i++) {
        int r = a - i;
        if (r == 0) {
            break;
        }
        if (arr[r][b] != '#')
            arr[r][b] = ' ';
    }
    for (int i = 1; i <= 20; i++) {
        int r = a + i;
        if (r == 20) {
            break;
        }
        if (arr[r][b] != '#')
            arr[r][b] = ' ';
    }
    for (int i = 1; i <= 20; i++) {
        int r = b - i;
        if (r == 0) {
            break;
        }
        if (arr[a][r] != '#')
            arr[a][r] = ' ';
    }
    for (int i = 1; i <= 20; i++) {
        int r = b + i;
        if (r == 20) {
            break;
        }
        if (arr[a][r] != '#')
            arr[a][r] = ' ';
    }
}

void game() {
    
        char arr[20][20];
        int score = 0;
        int bombs = 7;
        string name;
        cout << "\t\t\t\t--->| Enter your username (without space) : ";
        cin >> name;
        int a, b;
        if (stagenotcomp) {
            stage1(arr);
            a = 17; b = 15; 
        }
        else {
            stage2map(arr);
            a = 1; b = 18;
        }

        arr[a][b] = 'P';
        char ch;
        int enemy1_X = 1, enemy1_Y = 2;
        int enemy2_X = 5, enemy2_Y = 9;
        int enemy3_X = 11, enemy3_Y = 5;
        int enemy4_X = 13, enemy4_Y = 2;
        int enemy5_X = -1, enemy5_Y = -1;
        int enemy6_X = -1, enemy6_Y = -1;

        if (!(stagenotcomp)) {
            enemy5_X = 1; enemy5_Y = 8;
            enemy6_X = 17; enemy6_Y = 15;
        }

        bool enemy1Alive = true;
        bool enemy2Alive = true;
        bool enemy3Alive = true;
        bool enemy4Alive = true;
        bool enemy5Alive = true;
        bool enemy6Alive = true;

        bool hitEnemy = false;

        bool moveRight1 = true;
        bool moveRight2 = true;
        bool moveRight3 = true;
        bool moveRight4 = true;

        bool bombActive = false;
        bool hitBomb = false;
        bool moveEnemy = true;
        bool timer = true;
        int z = 10;
        if (!(stagenotcomp)) z = 6;
        int bomb_row = 0, bomb_col = 0;
        int bomb_timer = 0;

        int x = -1, y = -1;
        int do_timer = 0;
        bool plusNotplaced = true;
        bool plusfunction = false;
        int start = 0;

        bool powerNotplaced = true;
        int px = -1, py = -1;
        int power_timer = 0;
        bool powerfunction = false;
        int power = 0;

        bool bombpower = false;

        system("cls");
        PlaySound(TEXT("start.wav"), NULL, SND_FILENAME | SND_ASYNC);

        while (true) {
            if (_kbhit()) {
                ch = _getch();

                if (ch == 'q' || ch == 'Q') {
                    char quit;
                    system("cls");
                    cout << "Do you want to Exit\nPress any key to exit\nPress n to resume";
                    quit = _getch();
                    if (quit == 'n') {
                        continue;
                    }
                    else {
                        system("cls");
                        return;
                    }
                }

                mainscreen(arr, score, bombs);
                if (a != bomb_row || b != bomb_col)
                    arr[a][b] = ' ';
                else
                    arr[a][b] = 'B';

                if (a == enemy6_X && b == enemy6_Y) {
                    filesystem(name, score);
                    int choice3;
                    system("cls");
                    bombHit(choice3, score);
                    system("cls");
                    if (choice3 == 1) {
                        game();
                    }
                    else if (choice3 == 2);
                    return;
                }
                if (a == enemy5_X && b == enemy5_Y) {
                    filesystem(name, score);
                    int choice3;
                    system("cls");
                    bombHit(choice3, score);
                    system("cls");
                    if (choice3 == 1) {
                        game();
                    }
                    else if (choice3 == 2);
                    return;
                }
                if (a == enemy4_X && b == enemy4_Y) {
                    filesystem(name, score);
                    int choice3;
                    system("cls");
                    bombHit(choice3, score);
                    system("cls");
                    if (choice3 == 1) {
                        game();
                    }
                    else if (choice3 == 2);
                    return;
                }
                if (a == enemy3_X && b == enemy3_Y) {
                    filesystem(name, score);
                    int choice3;
                    system("cls");
                    bombHit(choice3, score);
                    system("cls");
                    if (choice3 == 1) {
                        game();
                    }
                    else if (choice3 == 2);
                    return;
                }
                if (a == enemy2_X && b == enemy2_Y) {
                    filesystem(name, score);

                    int choice3;
                    system("cls");
                    bombHit(choice3, score);
                    system("cls");
                    if (choice3 == 1) {
                        game();
                    }
                    else if (choice3 == 2);
                    return;
                }
                if (a == enemy1_X && b == enemy1_Y) {
                    filesystem(name, score);
                    int choice3;
                    system("cls");
                    bombHit(choice3, score);
                    system("cls");
                    if (choice3 == 1) {
                        game();
                    }
                    else if (choice3 == 2);
                    return;
                }

                movement(ch, a, b, arr);
                arr[a][b] = 'P';
                mainscreen(arr, score, bombs);

                if (ch == ' ' && !bombActive) {
                    if (bombs > 0) {
                        PlaySound(TEXT("run.wav"), NULL, SND_FILENAME | SND_ASYNC); 

                        bombActive = true;
                        bomb_row = a;
                        bomb_col = b;
                        arr[bomb_row][bomb_col] == 'B';
                        bombs--;
                        bomb_timer = 70;
                    }
                }
                if ((ch == 'p' || ch == 'P') && bombpower) {
                    if (stagenotcomp) {
                        poweraction(a, b, arr, score,
                            enemy1Alive, enemy1_X, enemy1_Y,
                            enemy2Alive, enemy2_X, enemy2_Y,
                            enemy3Alive, enemy3_X, enemy3_Y,
                            enemy4Alive, enemy4_X, enemy4_Y);
                    }
                    else {
                        poweraction(a, b, arr, score,
                            enemy1Alive, enemy1_X, enemy1_Y,
                            enemy2Alive, enemy2_X, enemy2_Y,
                            enemy3Alive, enemy3_X, enemy3_Y,
                            enemy4Alive, enemy4_X, enemy4_Y,
                            enemy5Alive, enemy5_X, enemy5_Y,
                            enemy6Alive, enemy6_X, enemy6_Y);
                    }
                    mainscreen(arr, score, bombs);
                    Sleep(200);
                    clearpoweraction(a, b, arr);
                    bombpower = false;

                }
            }

            if (bombActive) {
                if (bomb_timer == 70) {
                    arr[bomb_row][bomb_col] = 'B';
                }
                else if (bomb_timer == 1) {
                    if (stagenotcomp) {
                        bombsystem(a, b, bomb_row, bomb_col, arr, hitBomb, score,
                            enemy1Alive, enemy1_X, enemy1_Y,
                            enemy2Alive, enemy2_X, enemy2_Y,
                            enemy3Alive, enemy3_X, enemy3_Y,
                            enemy4Alive, enemy4_X, enemy4_Y);
                    }
                    else {
                        bombsystem(a, b, bomb_row, bomb_col, arr, hitBomb, score,
                            enemy1Alive, enemy1_X, enemy1_Y,
                            enemy2Alive, enemy2_X, enemy2_Y,
                            enemy3Alive, enemy3_X, enemy3_Y,
                            enemy4Alive, enemy4_X, enemy4_Y,
                            enemy5Alive, enemy5_X, enemy5_Y,
                            enemy6Alive, enemy6_X, enemy6_Y);
                    }
                    if (hitBomb) {
                        filesystem(name, score);

                        system("cls");
                        int choice2;
                        bombHit(choice2, score);
                        system("cls");
                        if (choice2 == 1) {
                            game();
                        }
                        else if (choice2 == 2);
                        return;
                    }
                }
                else if (bomb_timer == 0) {
                    clearbomb(bomb_row, bomb_col, arr);
                    bombActive = false;
                }
                bomb_timer--;
            }

            functions(plusfunction, plusNotplaced, arr, do_timer, a, b, x, y, bombs, start);
            powerFunctions(powerfunction, powerNotplaced, arr, power_timer, a, b, px, py, bombpower, power);


            if (moveEnemy) {

                srand(time(0));
                int ch1;
                if (enemy1Alive) {
                    ch = rand() % 5 + 1;
                    arr[enemy1_X][enemy1_Y] = ' ';
                    enemymovement(ch, enemy1_X, enemy1_Y, a, b, arr, hitEnemy);
                    if (hitEnemy) {
                        filesystem(name, score);
                        int choice3;
                        system("cls");
                        bombHit(choice3, score);
                        system("cls");
                        if (choice3 == 1) {
                            game();
                        }
                        else if (choice3 == 2);
                        return;
                    }
                }
                if (enemy2Alive) {
                    srand(time(0));
                    ch = rand() % 5 + 1;
                    arr[enemy2_X][enemy2_Y] = ' ';
                    enemymovement(ch, enemy2_X, enemy2_Y, a, b, arr, hitEnemy);
                    if (hitEnemy) {
                        filesystem(name, score);
                        int choice3;
                        system("cls");
                        bombHit(choice3, score);
                        system("cls");
                        if (choice3 == 1) {
                            game();
                        }
                        else if (choice3 == 2);
                        return;
                    }
                }
                if (enemy3Alive) {
                    srand(time(0));
                    ch = rand() % 5 + 1;
                    arr[enemy3_X][enemy3_Y] = ' ';
                    enemymovement(ch, enemy3_X, enemy3_Y, a, b, arr, hitEnemy);
                    if (hitEnemy) {
                        filesystem(name, score);
                        int choice3;
                        system("cls");
                        bombHit(choice3, score);
                        system("cls");
                        if (choice3 == 1) {
                            game();
                        }
                        else if (choice3 == 2);
                        return;
                    }
                }
                if (enemy4Alive) {
                    srand(time(0));
                    ch = rand() % 5 + 1;
                    arr[enemy4_X][enemy4_Y] = ' ';
                    enemymovement(ch, enemy4_X, enemy4_Y, a, b, arr, hitEnemy);
                    if (hitEnemy) {
                        filesystem(name, score);
                        int choice3;
                        system("cls");
                        bombHit(choice3, score);
                        system("cls");
                        if (choice3 == 1) {
                            game();
                        }
                        else if (choice3 == 2);
                        return;
                    }
                }
                if (!(stagenotcomp)) {
                    if (enemy5Alive) {
                        srand(time(0));
                        ch = rand() % 5 + 1;
                        arr[enemy5_X][enemy5_Y] = ' ';
                        enemymovement(ch, enemy5_X, enemy5_Y, a, b, arr, hitEnemy);
                        if (hitEnemy) {
                            filesystem(name, score);
                            int choice3;
                            system("cls");
                            bombHit(choice3, score);
                            system("cls");
                            if (choice3 == 1) {
                                game();
                            }
                            else if (choice3 == 2);
                            return;
                        }
                    }
                    if (enemy6Alive) {
                        srand(time(0));
                        ch = rand() % 5 + 1;
                        arr[enemy6_X][enemy6_Y] = ' ';
                        enemymovement(ch, enemy6_X, enemy6_Y, a, b, arr, hitEnemy);
                        if (hitEnemy) {
                            filesystem(name, score);
                            int choice3;
                            system("cls");
                            bombHit(choice3, score);
                            system("cls");
                            if (choice3 == 1) {
                                game();
                            }
                            else if (choice3 == 2);
                            return;
                        }
                    }
                }
            }
            if (stagenotcomp) {
                if (z == 10) {
                    moveEnemy = true;
                    timer = true;
                }
                else {
                    moveEnemy = false;
                }
            }
            else {
                if (z == 6) {
                    moveEnemy = true;
                    timer = true;
                }
                else {
                    moveEnemy = false;
                }
            }

            if (z == 0 && timer) {
                timer = false;
            }
            if (timer) {
                z--;
            }
            else {
                z++;
            }


            if (!enemy1Alive && !enemy2Alive && !enemy3Alive && !enemy4Alive && stagenotcomp) {
                system("cls");
                filesystem(name, score);
                int choice4;
                win(choice4, score);
                system("cls");
                stagenotcomp = false;
                if (choice4 == 1) {
                    stagenotcomp = false;
                    game();
                }
                else if (choice4 == 2)
                    return;
            }
            else {
                if (!enemy1Alive && !enemy2Alive && !enemy3Alive && !enemy4Alive && !enemy5Alive && !enemy6Alive) {
                    system("cls");
                    filesystem(name, score);
                    stagenotcomp = false;
                    int choice4;
                    win(choice4, score);
                    system("cls");
                    if (choice4 == 1) {
                        game();
                    }
                    else if (choice4 == 2)
                        return;
                }
            }
            mainscreen(arr, score, bombs);

        }

}

void howToplay() {
    setColor(9);
    char back;
    cout << "\t\t\t\t\t\t   | How to Play | \n\n";
    cout << "\t\t\t\t\t> Use Arrow keys or wasd to move player 'P'\n";
    cout << "\t\t\t\t\t> Use Space to place a bomb 'B'\n";
    cout << "\t\t\t\t\t> Press 'Q' to Quit game and go to main menu\n";
    cout << "\t\t\t\t\t> Protect Player P from bomb blast\n";
    cout << "\t\t\t\t\t> You can't destroy walls '#' \n";
    cout << "\t\t\t\t\t> Protect Player P from E (Enemy)\n";
    cout << "\t\t\t\t\t> Use Bomb to clear Obstacles 'O' or kill Enemy 'E'\n";
    cout << "\t\t\t\t\t> You will win if you kill all Enemies\n\n\n\n";
    cout << "\t\t\t\t\t> You will have limited bombs \n\n\n\n";
    cout << "\t\t\t\t\t> After some time a '+' icon will appear on screen, collect it to increase no. of bombs \n\n\n\n";
    cout << "\t\t\t\t\t> After some time a '?' icon will appear on screen, collect it to get power (press 'P' to use power)\n\n\n\n";
    cout << "\t\t\t\t\t> Power will destroy and kill all enemies present in that row and column\n\n\n\n";

    cout << "\t\t\t\t\t> press B to go back \n\n\n\n";
    back = _getch();
    while (!(back == 'B' || back == 'b')) {
        back = _getch();
    }
    system("cls");
}

void topThree() {
    displayTopThree();
    char back;
    cout << "\t\t\t\t\t> press B to go back \n\n\n\n";
    back = _getch();
    while (!(back == 'B' || back == 'b')) {
        back = _getch();
    }
    system("cls");
}

void credit() {
    setColor(7);
    cout << "\t\t\t\t\t\t      Created By \n";
    cout << "\t\t\t\t\t\t      ========== \n\n";
    cout << "\t\t\t\t\t\t-->| Muhammad Saad Ali    |<--\n\n\n";
    char back; cout << "\t\t\t\t\t> press B to go back \n\n\n\n";
    back = _getch();
    while (!(back == 'B' || back == 'b')) {
        back = _getch();
    }
    system("cls");
}

int main() {

    int choice;
    do {
        menu(choice);
        system("cls");
        if (choice == 1) {
            game();
        }
        else if (choice == 2) {
            howToplay();
        }
        else if (choice == 3) {
            topThree();
        }
        else if (choice == 4) {
            credit();
        }
        else {
            cout << "\t\t\t\t\t\t  THANK YOU FOR PLAYING\n\n\n";
        }
    } while (choice != 5);
    return 0;
}