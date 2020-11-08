#include "game.hpp"
#include <array>
#include <iostream>
#include <list>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <vector>
using namespace std;

array<array<int, WIDTH>, HEIGHT> Board = {0};
int Obstacle::total = 0;

//$게임 시작 함수
int game(int time, Character &character, vector<Obstacle> &obstacle) {
    char input;

    //화면 시작
    initscr();
    cbreak();
    noecho();
    scrollok(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);

    //초기 화면
    character.showD();

    //게임 시작
    while (true) {

        generateObstacle(obstacle);
        showState(obstacle, time);
        usleep(time);
        if (nemberOfObstacle(obstacle) % MAXOB == 1 &&
            nemberOfObstacle(obstacle) != 1) {
            mvprintw(1, WIDTH + 2,
                     "Game Start After 3 seconds.."); //$ui와 관련된 부분으로
                                                      //수정 가능
            refresh();
            sleep(3);
            clear();
        }
        input = getch();
        if (input == 'q') {
            break;
        }
        //오른쪽 이동
        if (input == 'd') {
            character.moveR();
        }
        //왼쪽 이동
        else if (input == 'a') {
            character.moveL();
        } else {
            character.showD();
        }
        //$충돌했을 경우
        //목숨이 차감되는 함수 추가&fail판단 추가
        //지금은 한 번이라도 충돌시 FAIL 출력하고 종료
        if (crash(character.getY(), character.getX())) {
            clear();
            mvprintw(5, 10, "FAIL");
            refresh();
            sleep(5);
            break;
        }

        //$스테이지 1개 종료
        // 1개의 스테이지를 어떻게 구분할지에 관한 부분
        //지금은 10개 단위로 스테이지 구분.
        //지나간 장애물의 수가 10의 배수일 경우 지나간 장애물의 수를 반환
        if (nemberOfObstacle(obstacle) % MAXOB == 0 &&
            nemberOfObstacle(obstacle) != 0) {
            endwin();
            return nemberOfObstacle(obstacle);
        }
    }
    endwin();
    return 0;
}
//캐릭터 클래스
Character::Character() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            Board[location[0] + i][location[1] + j] = 2;
        }
    }
}
void Character::moveR() {
    if (location[1] >= WIDTH + 2) {
        return;
    }
    //이전 기록 지움
    for (int i = 0; i < motionR.size(); i++) {
        for (int j = 0; j < motionR[i].size(); j++) {
            mvprintw(location[0] + i, location[1] + j, " ", motionR[i][j]);
            Board[location[0] + i][location[1] + j] = 0;
        }
        printw("\n");
    }
    printw("\n");

    //움직인 좌표 저장&출력
    location[1] += 1;
    for (int i = 0; i < motionR.size(); i++) {
        for (int j = 0; j < motionR[i].size(); j++) {
            mvprintw(location[0] + i, location[1] + j, "%c", motionR[i][j]);
            Board[location[0] + i][location[1] + j] += 2;
        }
        printw("\n");
    }
    printw("\n");
    refresh();
}
void Character::moveL() {
    if (location[1] <= 0) {
        return;
    }
    //이전 기록 지움
    for (int i = 0; i < motionL.size(); i++) {
        for (int j = 0; j < motionL[i].size(); j++) {
            mvprintw(location[0] + i, location[1] + j, " ", motionL[i][j]);
            Board[location[0] + i][location[1] + j] = 0;
        }
        printw("\n");
    }
    printw("\n");

    //움직인 좌표 저장&출력
    location[1] -= 1;
    for (int i = 0; i < motionL.size(); i++) {
        for (int j = 0; j < motionL[i].size(); j++) {
            mvprintw(location[0] + i, location[1] + j, "%c", motionL[i][j]);
            Board[location[0] + i][location[1] + j] = 2;
        }
        printw("\n");
    }
    printw("\n");
    refresh();
}

//처음 모습
void Character::showD() {
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

Obstacle::Obstacle() {
    total += 1;
    srand(time(NULL));
    length = rand() % MAX_LENGTH + 1;
    location[1] = rand() % (WIDTH - length); // 0~width-1-length

    for (int i = 0; i < length; i++) {
        Board[0][location[1] + i] = 1;
    }
}

void Obstacle::show() {
    move(location[0], location[1]);
    for (int i = 0; i < length; i++) {
        printw("@");
    }
    refresh();
}

void Obstacle::moveO() {
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
        Board[location[0]][location[1] + i] += 1;
        printw("@");
    }
    refresh();
}

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
            if (Board[y + i][x + j] >= 3) {
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
void showState(vector<Obstacle> &obstacle, int time) {
    mvprintw(0, WIDTH + 2, "%d %d %d", obstacle.size(),
             nemberOfObstacle(obstacle), time);
    refresh();
}
int nemberOfObstacle(vector<Obstacle> &obstacle) {
    int total = Obstacle::total;
    if (obstacle.size() == HEIGHT + 1) {
        return total - obstacle.size() + 1;
    }
    return total - obstacle.size();
}
