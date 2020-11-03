#include <array>
#include <iostream>
#include <list>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include <vector>

using namespace std;

int linux_kbhit(void) {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

const int WIDTH = 43;
const int HEIGHT = 20;
array<array<int, WIDTH>, HEIGHT> Board = {0};

class Character {

  public:
    Character() {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                Board[location[0] + i][location[1] + j] = 2;
            }
        }
    }
    void moveR() {
        if (location[1] >= WIDTH + 2) {
            return;
        }
        //이전 기록 지움
        for (int i = 0; i < 3; i++) {
            Board[location[0] + i][location[1]] = 0;
        }

        //움직인 좌표 저장&출력
        location[1] += 1;
        for (int i = 0; i < motionR.size(); i++) {
            move(location[0] + i, location[1] - 1);
            printw(" ");
            for (int j = 0; j < motionR[i].size(); j++) {
                printw("%c", motionR[i][j]);
                Board[location[0] + i][location[1] + j] = 2;
            }
            printw("\n");
        }
        printw("\n");
        refresh();
    }
    void moveL() {
        if (location[1] <= 0) {
            return;
        }
        //이전 기록 지움
        for (int i = 0; i < 3; i++) {
            Board[location[0] + i][location[1] + 2] = 0;
        }

        //움직인 좌표 저장&출력
        location[1] -= 1;
        for (int i = 0; i < motionL.size(); i++) {
            move(location[0] + i, location[1]);
            for (int j = 0; j < motionL[i].size(); j++) {
                printw("%c", motionL[i][j]);
                Board[location[0] + i][location[1] + j] = 2;
            }
            printw(" ");
            printw("\n");
        }
        printw("\n");
        refresh();
    }

    //처음 모습
    void showD() {
        for (int i = 0; i < motionD.size(); i++) {
            move(location[0] + i, location[1]);
            for (int j = 0; j < motionD[i].size(); j++) {
                printw("%c", motionD[i][j]);
            }
            printw("\n");
        }
        printw("\n");
        refresh();
    }

    int getX() { return location[1]; }
    int getY() { return location[0]; }

  private:
    array<array<char, 3>, 3> motionD = {' ',  'O', ' ', '/', 'T',
                                        '\\', '|', ' ', '|'};
    array<array<char, 3>, 3> motionR = {' ',  'O', ' ', '/', 'T',
                                        '\\', '|', ' ', '\\'};
    array<array<char, 3>, 3> motionL = {' ',  'O', ' ', '/', 'T',
                                        '\\', '/', ' ', '|'};
    array<int, 2> location = {HEIGHT - 3, (int)(WIDTH - 3) / 2};
};

class Obstacle {

  public:
    static int total;

    static int getTotal();
    Obstacle() {
        total += 1;
        srand(time(NULL));
        length = rand() % 5 + 1;
        location[1] = rand() % (WIDTH - length); // 0~width-1-length

        for (int i = 0; i < length; i++) {
            Board[0][location[1] + i] = 1;
        }
    }

    void show() {
        move(location[0], location[1]);
        for (int i = 0; i < length; i++) {
            printw("@");
        }
        refresh();
    }
    void moveO() {
        if (location[0] >= HEIGHT) {
            return;
        } else if (location[0] == HEIGHT - 1) {
            Board[location[0]].fill(0);

            move(location[0], location[1]);
            for (int i = 0; i < length; i++) {
                printw(" ");
            }
            location[0] += 1;
            refresh();
            return;
        }
        //이전 위치 지우기
        move(location[0], location[1]);
        for (int i = 0; i < length; i++) {
            Board[location[0]][location[1] + i] = 0;
            printw(" ");
        }
        //아래로 이동
        location[0] += 1;
        move(location[0], location[1]);
        for (int i = 0; i < length; i++) {
            Board[location[0]][location[1] + i] = 1;
            printw("@");
        }
        refresh();
    }

  private:
    int length;
    array<int, 2> location = {0, 0};
};
int Obstacle::total = 0;
int Obstacle::getTotal() { return total; }

void generateObstacle(vector<Obstacle> &obstacle) {
    if (obstacle.size() > HEIGHT) {
        obstacle.erase(obstacle.begin());
    }
    for (int i = 0; i < obstacle.size(); i++) {
        obstacle[i].moveO();
    }
    obstacle.push_back(Obstacle());
    obstacle[obstacle.size() - 1].show();
}

bool crash(int y, int x) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (Board[y + i][x + j] == 1) {
                return true;
            }
        }
    }
    return false;
}

void showBoard() {
    clear();
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printw("%d", Board[i][j]);
        }
        printw("\n");
    }
    refresh();
    sleep(2);
}

int nemberOfObstacle(vector<Obstacle> &obstacle) {
    int total = Obstacle::total;
    if (obstacle.size() == HEIGHT + 1) {
        return total - obstacle.size() + 1;
    }
    return total - obstacle.size();
}

int main(void) {
    vector<Obstacle> obstacle;
    char input;
    sleep(1);

    initscr();
    cbreak();
    noecho();
    scrollok(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    Character character = Character();
    obstacle.push_back(Obstacle());

    while (true) {
        // showBoard();
        if (!crash(character.getY() - 1, character.getX())) {
            character.showD();
        } else {
            character.showD();
            move(5, 5);
            printw("Fail");
            refresh();
            sleep(5);
            break;
        }
        generateObstacle(obstacle);
        move(0, WIDTH + 2);
        printw("%d %d", obstacle.size(), nemberOfObstacle(obstacle));
        refresh();
        usleep(1000000);

        input = getch();
        if (input == 'q') {
            break;
        }
        if (input == 'd') {
            if (!crash(character.getY() - 1, character.getX() + 1)) {
                character.moveR();
            } else {
                character.moveR();
                generateObstacle(obstacle);
                move(5, 5);
                printw("Fail");
                refresh();
                sleep(5);
                break;
            }
            generateObstacle(obstacle);
            move(0, WIDTH + 2);
            printw("%d %d", obstacle.size(), nemberOfObstacle(obstacle));
            refresh();
            usleep(1000000);
        } else if (input == 'a') {
            if (!crash(character.getY() - 1, character.getX() - 1)) {
                character.moveL();
            } else {
                character.moveL();
                generateObstacle(obstacle);
                move(5, 5);
                printw("Fail");
                refresh();
                sleep(5);
                break;
            }
            generateObstacle(obstacle);
            move(0, WIDTH + 2);
            printw("%d %d", obstacle.size(), nemberOfObstacle(obstacle));
            refresh();
            usleep(1000000);
        }
    }
    endwin();
    return 0;
}
