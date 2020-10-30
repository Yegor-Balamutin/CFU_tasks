#include <iostream>
#include <map>
#include <string>

using namespace std;

void check(int i, int j);
void fill(int i, int j);

string maze[25] = {   // 25 str * 27 chr
    "####B######################",
    "# #       #   #      #    #",
    "# # # ###### #### ####### #",
    "# # # #       #   #       #",
    "#   # # ######### # ##### #",
    "# # # #   #       #     # #",
    "### ### ### ############# #",
    "# #   #     # #           #",
    "# # #   ####### ###########",
    "# # # #       # #         C",
    "# # ##### ### # # ####### #",
    "# # #     ### # #       # #",
    "#   ##### ### # ######### #",
    "######### ### #           #",
    "# ####### ### #############",
    "A           #   ###   #   #",
    "# ############# ### # # # #",
    "# ###       # # ### # # # #",
    "# ######### # # ### # # # F",
    "#       ### # #     # # # #",
    "# ######### # ##### # # # #",
    "# #######   #       #   # #",
    "# ####### ######### #######",
    "#         #########       #",
    "#######E############D######"
};
bool d[25][27];
map<char, bool> exits;
int y, x;

void fill(int i, int j)
{
    d[i][j] = false;
    if (i > 0) {
        y = i - 1;
        x = j;
        if (d[y][x]) {
            check(y, x);
        }
    }
    if (j < 26) {
        y = i;
        x = j + 1;
        if (d[y][x]) {
            check(y, x);
        }
    }
    if (i < 24) {
        y = i + 1;
        x = j;
        if (d[y][x]) {
            check(y, x);
        }
    }
    if (j > 0) {
        y = i;
        x = j - 1;
        if (d[y][x]) {
            check(y, x);
        }
    }
}

void check(int i, int j)
{
    if (maze[i][j] == '#') {
    }
    else {
        if (maze[i][j] == ' ') {
            fill(i, j);
        }
        else {
            exits[maze[i][j]] = true;
        }
    }
}

int main()
{
	setlocale(LC_ALL, "Russian");
    int i, j;
    string s = "";
    for (char a = 'A'; a <= 'F'; a++) {
        exits[a] = false;
    }
    for (int a = 0; a < 25; a++) {
        for (int b = 0; b < 27; b++) {
            d[a][b] = true;
        }
    }
    cin >> j >> i;
    if ((i < 0) || (i > 24) || (j < 0) || (j > 26)) {
        cout << "Неверный ввод: выход за границы лабиринта";
    }
    else {
        if (maze[i][j] == '#') {
            cout << "Неверный ввод: стена";
        }
        else {
            check(i, j);
            for (char a = 'A'; a <= 'F'; a++) {
                if (exits[a]) {
                    s = s + " " + a;
                }
            }
            if (s == "") {
                cout << "Выхода нет.";
            }
            else {
                cout << "Доступные выходы:" << s;
            }
        }
    }
    return 0;
}