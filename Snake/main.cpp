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
using namespace this_thread;

enum Color {
    black = 0,
    dark_blue = 1,
    dark_green = 2,
    light_blue = 3,
    dark_red = 4,
    magenta = 5,
    orange = 6,
    light_gray = 7,
    gray = 8,
    blue = 9,
    green = 10,
    cyan = 11,
    red = 12,
    pink = 13,
    yellow = 14,
    white = 15,
};

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

const int width = 28;
const int height = 28;
int highscore{0};
int lastDirection{0};
bool endlessMode{false};
bool gameover{false};
bool blinking{false};
int snakeColor{0};
list<Snake> snakeBody;
struct Fruit fruit{5, 5};

void addPart() {
    struct Snake s{snakeBody.back().x, snakeBody.back().y, static_cast<int>(snakeBody.size())};
    snakeBody.push_back(s);
}

void nextColor() {
    if (snakeColor == 15) {
        snakeColor = 2;
    } else {
        snakeColor++;
    }
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
        int x = randNum(1, width / 2) * 2, y = randNum(1, height);
        if (!snakeContains(x, y, false)) {
            fruit.x = x;
            fruit.y = y;
            setCursorPosition(fruit.x, fruit.y);
            setColor(Color::red, Color::black);
            cout << "o";
            setColor(Color::white, Color::black);
            setCursorPosition(0, 0);
            nextColor();
            return;
        }
    }
}

void blinkGameover() {
    while (blinking) {
        setCursorPosition((width) - 6, (height / 2) + 1);
        cout << "PRESS ESC TO END GAME";
        setCursorPosition((width) - 6, (height / 2) + 2);
        cout << "OR ANY KEY TO RETRY...";
        setCursorPosition(0, 00);
        sleep_for(500ms);
        setCursorPosition((width) - 6, (height / 2) + 1);
        cout << "                     ";
        setCursorPosition((width) - 6, (height / 2) + 2);
        cout << "                      ";
        setCursorPosition(0, 0);
        sleep_for(500ms);
    }
}

void gameOver() {
    list<Snake>::iterator it;
    for (it = snakeBody.end(); it != snakeBody.begin(); --it) {
        sleep_for(250ms);
        setCursorPosition(it->x, it->y);
        cout << " ";
        setCursorPosition(0, 0);
    }
    setTitle("GAME OVER");
    gameover = true;
    cls();
    setCursorPosition(width, height / 2);
    cout << "GAME OVER";
    setCursorPosition((width) - 6, (height / 2) + 1);
    cout << "PRESS ESC TO END GAME";
    setCursorPosition((width) - 6, (height / 2) + 2);
    cout << "OR ANY KEY TO RETRY...";
    setCursorPosition((width) - 6, (height / 2) + 3);
    setColor(Color::orange, Color::black);
    cout << "SCORE: " << snakeBody.size();
    if (snakeBody.size() > highscore) {
        highscore = snakeBody.size();
    }
    cout << " HIGHSCORE: " << highscore;
    setColor(Color::white, Color::black);
    thread t(blinkGameover);
    blinking = true;
    t.join();
    start();
}

void changeVal(Snake &sn, int direction) {
    switch (direction) {
        case 1:
            sn.x += 2;
            break;
        case 2:
            sn.y++;
            break;
        case 3:
            sn.x -= 2;
            break;
        case 4:
            sn.y--;
            break;
    }
    if (endlessMode) {
        if (sn.x < 1) {
            sn.x = width * 2;
        } else if (sn.x > width * 2) {
            sn.x = 1;
        } else if (sn.y < 1) {
            sn.y = height;
        } else if (sn.y > height) {
            sn.y = 1;
        }
    } else {
        if (sn.x < 1 || sn.x > width * 2 || sn.y < 1 || sn.y > height || snakeContains(sn.x, sn.y, true)) {
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
        setColor(snakeColor, Color::black);
        switch (direction) {
            case 1:
                cout << ">";
                break;
            case 2:
                cout << "v";
                break;
            case 3:
                cout << "<";
                break;
            case 4:
                cout << "^";
                break;
        }
        setColor(Color::white, Color::black);
        setCursorPosition(0, 0);
    }
}

void drawField() {
    cls();
    setColor(Color::orange, Color::black);
    cout << "----------------------------------------------------------" << endl;
    for (int i = 0; i < height; i++) {
        setColor(Color::orange, Color::black);
        cout << "|";
        for (int ii = 0; ii < width; ii++) {
            setColor(Color::white, Color::black);
            cout << "  ";
        }
        setColor(Color::orange, Color::black);
        cout << "|" << endl;
    }
    cout << "----------------------------------------------------------" << endl;
    setColor(Color::white, Color::black);
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
    sleep_for(50ms);
}

void buttonPressListener() {
    while (true) {
        int c = _getch();
        if (c == ESCAPE) {
            exit(1);
        }
        blinking = false;
        buttonPressed(c);
    }
}

void gameLoop() {
    while (!gameover) {
        sleep_for(125ms);
        moveSnake(lastDirection);
    }
}

void start() {
    setTitle("Snake");
    snakeBody.clear();
    blinking = false;
    gameover = false;
    lastDirection = 0;
    struct Snake s1{randNum(1, width) * 2, randNum(1, height), 0};
    snakeBody.push_back(s1);
    addPart();
    addPart();
    addPart();
    drawField();
    newFruit();
    setCursorPosition(snakeBody.front().x, snakeBody.front().y);
    setColor(Color::white, Color::black);
    cout << ">";
    setColor(Color::white, Color::black);
    setCursorPosition(0, 0);
    thread t1(gameLoop);
    t1.join();
}

int main() {
    thread t1(buttonPressListener);
    t1.detach();
    start();
    return 1;
}
