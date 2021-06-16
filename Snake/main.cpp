#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define ESCAPE 27

#include <iostream>
#include <list>
#include <thread>
#include <random>
#include <conio.h>
#include "utils.h"
#include <chrono>

using namespace std;
using namespace std::this_thread;

struct Snake {
    int x;
    int y;
    int id;
};

struct Fruit {
    int x;
    int y;
};

void start();

const int width = 40;
const int height = 20;
int lastDirection{0};
bool endlessMode{0};
bool gameover{0};
bool blinking{0};
list<Snake> snakeBody;
struct Fruit fruit{5, 5};

void addPart() {
    struct Snake s{snakeBody.back().x, snakeBody.back().y, static_cast<int>(snakeBody.size())};
    snakeBody.push_back(s);
}

bool snakeContains(int x, int y, bool excludeHead) {
    list<Snake>::iterator it;
    for (it = snakeBody.begin(); it != snakeBody.end(); ++it) {
        if (excludeHead && it->id == 0) {
            continue;
        }
        if (it->x == x && it->y == y) {
            return true;
        }
    }
    return false;
}

void newFruit() {
    while (true) {
        int x = randNum(1, width), y = randNum(1, height);
        if (!snakeContains(x, y, false)) {
            fruit.x = x;
            fruit.y = y;
            setCursorPosition(fruit.x, fruit.y);
            cout << "o";
            setCursorPosition(0, height);
            return;
        }
    }
}

void blinkGameover() {
    while (blinking) {
        setCursorPosition((width / 2) - 6, (height / 2) + 1);
        cout << "PRESS ESC TO END GAME";
        setCursorPosition(0, height);
        sleep_for(500ms);
        setCursorPosition((width / 2) - 6, (height / 2) + 1);
        cout << "                     ";
        setCursorPosition(0, height);
        sleep_for(500ms);
    }
}

void gameOver() {
    gameover = true;
    cls();
    setCursorPosition(width / 2, height / 2);
    cout << "GAME OVER";
    setCursorPosition((width / 2) - 6, (height / 2) + 1);
    cout << "PRESS ESC TO END GAME";
    setCursorPosition((width / 2) - 6, (height / 2) + 2);
    cout << "SCORE: " << snakeBody.size();
    thread t(blinkGameover);
    blinking = 1;
    t.detach();
}

void changeVal(Snake &sn, int direction) {
    switch (direction) {
        case 1:
            sn.x++;
            break;
        case 2:
            sn.y++;
            break;
        case 3:
            sn.x--;
            break;
        case 4:
            sn.y--;
            break;
    }
    if (endlessMode) {
        if (sn.x < 1) {
            sn.x = width;
        } else if (sn.x > width) {
            sn.x = 1;
        } else if (sn.y < 1) {
            sn.y = height;
        } else if (sn.y > height) {
            sn.y = 1;
        }
    } else {
        if (sn.x < 1 || sn.x > width || sn.y < 1 || sn.y > height || snakeContains(sn.x, sn.y, true)) {
            gameOver();
            return;
        }
    }
    if (fruit.x == sn.x && fruit.y == sn.y) {
        addPart();
        newFruit();
    }
}

void moveUp(int direction) {
    auto l_front = snakeBody.begin();
    int prevX = snakeBody.front().x;
    int prevY = snakeBody.front().y;
    list<Snake>::iterator it;
    for (it = snakeBody.begin(); it != snakeBody.end(); ++it) {
        int x = it->x, y = it->y;
        it->x = prevX;
        it->y = prevY;
        prevX = x;
        prevY = y;
    }
    changeVal(snakeBody.front(), direction);
}

void moveSnake(int direction) {
    if (direction != 0) {
        lastDirection = direction;
        setCursorPosition(snakeBody.back().x, snakeBody.back().y);
        cout << " ";
        moveUp(direction);
        if (gameover) {
            return;
        }
        setCursorPosition(snakeBody.front().x, snakeBody.front().y);
        cout << "+";
        setCursorPosition(0, height);
    }
}

void drawField(int height, int width) {
    cls();
    cout << "------------------------------------------" << endl;
    for (int i = 0; i < height; i++) {
        cout << "|";
        for (int ii = 0; ii < width; ii++) {
            cout << " ";
        }
        cout << "|" << endl;
    }
    cout << "------------------------------------------\n\n" << endl;
}

void buttonPressed(int key) {
    switch (key) {
        case KEY_UP:
            if (lastDirection == 0 || lastDirection == 3 || lastDirection == 1 || lastDirection == 4) {
                lastDirection = 4;
            }
            break;
        case KEY_DOWN:
            if (lastDirection == 0 || lastDirection == 3 || lastDirection == 1 || lastDirection == 2) {
                lastDirection = 2;
            }
            break;
        case KEY_LEFT:
            if (lastDirection == 0 || lastDirection == 2 || lastDirection == 4 || lastDirection == 3) {
                lastDirection = 3;
            }
            break;
        case KEY_RIGHT:
            if (lastDirection == 0 || lastDirection == 2 || lastDirection == 4 || lastDirection == 1) {
                lastDirection = 1;
            }
            break;
        default:
            break;
    }
}

void buttonPressListener() {
    while (1) {
        int c = getch();
        if (c == ESCAPE) {
            exit(1);
        }
        buttonPressed(c);
    }
}

void gameLoop() {
    while (1) {
        if (!gameover) {
            sleep_for(100ms);
            moveSnake(lastDirection);
        }
    }
}

void start() {
    snakeBody.clear();
    blinking = 0;
    gameover = 0;
    lastDirection = 1;
    struct Snake s1{randNum(1, width), randNum(1, height), 0};
    snakeBody.push_back(s1);
    addPart();
    addPart();
    addPart();
    drawField(height, width);
    newFruit();
    setCursorPosition(snakeBody.front().x, snakeBody.front().y);
    cout << "+";
    setCursorPosition(0, height);
    thread t1(gameLoop);
    t1.join();
}

int main() {
    thread t1(buttonPressListener);
    t1.detach();
    start();
    return 1;
}