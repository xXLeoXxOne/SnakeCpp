#include "utils.h"
#include <iostream>
#include <thread>
#include <random>
#include <Windows.h>

using namespace std;

void cls() {
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    COORD topLeft = {0, 0};
    cout.flush();
    if (!GetConsoleScreenBufferInfo(hOut, &csbi)) {
        abort();
    }
    DWORD length = csbi.dwSize.X * csbi.dwSize.Y;
    DWORD written;
    FillConsoleOutputCharacter(hOut, TEXT(' '), length, topLeft, &written);
    FillConsoleOutputAttribute(hOut, csbi.wAttributes, length, topLeft, &written);
    SetConsoleCursorPosition(hOut, topLeft);
}


void setColor(int font, int background) {
    HANDLE hConsole;
    int colorcode = font + (background * 16);
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    FlushConsoleInputBuffer(hConsole);
    SetConsoleTextAttribute(hConsole, colorcode);
}

void setTitle(std::string title) {
    SetConsoleTitle(title.c_str());
}

void setCursorPosition(int x, int y) {
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    cout.flush();
    COORD coord = {(SHORT) x, (SHORT) y};
    SetConsoleCursorPosition(hOut, coord);
}

std::random_device rd;
std::mt19937 rng(rd());

int randNum(int min, int max) {
    std::uniform_int_distribution<int> uni(min, max);
    return uni(rng);
}
