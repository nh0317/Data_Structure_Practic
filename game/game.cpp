#include "game.hpp"
#include "playerInfo.hpp"
#include "ui.hpp"
#include <array>
#include <iostream>
#include <list>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <vector>
using namespace std;

array<array<int, WIDTH>, HEIGHT> Board = {0};
int Obstacle::total = 0;

// 게임 화면 ui
void uiBox() {
    setWindow2(HEIGHT - 2, WIDTH + 2, 0, 3);
    setWindow2(4, WIDTH + 2, 0, 0);
}

void showObstacle(vector<Obstacle> &obstacle) {
    for (int i = 1; i < obstacle.size(); i++) {
        obstacle[i].show();
    }
}

void uiBox2(WINDOW *my_win) {
    my_win = newwin(HEIGHT - 3, WIDTH + 2, 0, 3);
    refresh();

    box(my_win, 0, 0);

    wrefresh(my_win);
}

// game 설명창
void explain() {
    curs_set(0);

    //특수키 사용 (방향키 등)
    keypad(stdscr, TRUE);
    //윈도우 테두리 시작위치
    int start_y = 0, start_x = 0;

    // 커서의 위치 확인
    int position = 1;

    setWindow2(UIHEIGHT, UIWIDTH, 0, 0);

    mvprintw(UIHEIGHT / 2 - 5,
             (UIWIDTH - strlen("1. ⇦ ⇨ 로 캐릭터를 조작하세요.")) / 2 + 7,
             "1. ⇦ ⇨ 로 캐릭터를 조작하세요.");
    mvprintw(
        UIHEIGHT / 2 - 3,
        (UIWIDTH - strlen("2. 다음 조작 전, 캐릭터가 움직이기를 기다리세요.")) /
                2 +
            10,
        "2. 다음 조작 전, 캐릭터가 움직이기를 기다리세요.");
    mvprintw(
        UIHEIGHT / 2 - 1,
        (UIWIDTH -
         strlen("3. 게임이 시작하면, 'SCORE','STAGE'가 상단에 표시됩니다.")) /
                2 +
            8,
        "3. 게임이 시작하면, 'SCORE','STAGE'가 상단에 표시됩니다.");
    mvprintw(UIHEIGHT / 2 + 1,
             (UIWIDTH - strlen("4. 장애물 10개당 1 STAGE씩 증가합니다.")) / 2 +
                 8,
             "4. 장애물 10개당 1 STAGE씩 증가합니다.");
    mvprintw(
        UIHEIGHT / 2 + 3,
        (UIWIDTH - strlen("5. 총 STAGE는 6개, CREAR 점수는 6000점입니다.")) /
                2 +
            6,
        "5. 총 STAGE는 6개, CREAR 점수는 6000점입니다.");
    mvprintw(
        UIHEIGHT / 2 + 5,
        (UIWIDTH - strlen("6. 게임 종료 후 RANKING을 확인해보세요:)")) / 2 + 7,
        "6. 게임 종료 후 RANKING을 확인해보세요:)");

    mvprintw(UIHEIGHT - 1, (UIWIDTH - strlen("[Press Enter]")) / 2,
             "[Press Enter]");

    refresh();
    // 엔터쳐서 돌아가기
}

//$게임 시작 함수
int game(int time, Character &character, vector<Obstacle> &obstacle) {
    int score = 0;
    int input;
    score = numberOfObstacle() * 100;
    Board = {0};
    //화면 시작
    initscr();
    cbreak();
    noecho();
    scrollok(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);

    //초기 화면
    uiBox();
    showState(score, time);
    character.showD();

    if (numberOfObstacle() != 0) {
        //$ui와 관련된 부분으로 수정 가능
        showObstacle(obstacle);
        mvprintw(2, 1, "Game Start After 3 seconds ");
        printw("1.. ");
        refresh();
        sleep(1);
        printw("2.. ");
        refresh();
        sleep(1);
        printw("3.. ");
        refresh();
        sleep(1);
        mvprintw(2, 1, "                                        ");
    }

    //게임 시작
    while (true) {
        generateObstacle(obstacle);
        //$게임에 대한 정보가 출력됨
        //지금은 회피한 장애물의 수, time 출력
        score = numberOfObstacle() * 100;
        showState(score, time);
        usleep(time);

        input = getch();
        // q로 종료
        if (input == 'q') {
            break;
        }
        //오른쪽 이동
        if (input == KEY_RIGHT) {
            character.moveR();
        }
        //왼쪽 이동
        else if (input == KEY_LEFT) {
            character.moveL();
        } else {
            character.showD();
        }
        // uiBox();
        //$충돌했을 경우
        //목숨이 차감되는 함수 추가&fail판단 추가
        //지금은 한 번이라도 충돌시 FAIL 출력하고 종료
        if (crash(character.getY(), character.getX())) {
            break;
        }

        //$스테이지 1개 종료
        // 1개의 스테이지를 어떻게 구분할지에 관한 부분
        //지금은 10개 단위로 스테이지 구분.
        //지나간 장애물의 수가 10의 배수일 경우 지나간 장애물의 수를 반환
        if (numberOfObstacle() % MAXOB == 0 && numberOfObstacle() != 0) {
            endwin();
            return numberOfObstacle();
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
    }

    //움직인 좌표 저장&출력
    location[1] += 1;
    for (int i = 0; i < motionR.size(); i++) {
        for (int j = 0; j < motionR[i].size(); j++) {
            mvprintw(location[0] + i, location[1] + j, "%c", motionR[i][j]);
            //캐릭터가 있는 위치에는 2을 더함
            Board[location[0] + i][location[1] + j] += 2;
        }
    }
    refresh();
}
//왼쪽으로 이동
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
    }

    //움직인 좌표 저장&출력
    location[1] -= 1;
    for (int i = 0; i < motionL.size(); i++) {
        for (int j = 0; j < motionL[i].size(); j++) {
            mvprintw(location[0] + i, location[1] + j, "%c", motionL[i][j]);
            //캐릭터가 있는 위치에는 2을 더함
            Board[location[0] + i][location[1] + j] += 2;
        }
    }
    refresh();
}

//정지 모습
void Character::showD() {
    for (int i = 0; i < motionD.size(); i++) {
        for (int j = 0; j < motionD[i].size(); j++) {
            mvprintw(location[0] + i, location[1] + j, "%c", motionD[i][j]);
        }
    }
    refresh();
}

Obstacle::Obstacle() {
    total += 1;
    srand(time(NULL));
    length = rand() % MAX_LENGTH + 1;

    //출력 수정할 것
    location[1] = rand() % (WIDTH - length) + TOPX; // TOPX~width-1-length

    for (int i = 0; i < length; i++) {
        Board[0][location[1] + i] = 1;
    }
}

void Obstacle::show() {
    for (int i = 0; i < length; i++) {
        mvprintw(location[0], location[1] + i, "@");
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
        //장애물이 있는 위치에는 1을 더함
        Board[location[0]][location[1] + i] += 1;
        printw("@");
    }
    refresh();
}

//장애물 생성
void generateObstacle(vector<Obstacle> &obstacle) {
    if (obstacle.size() > HEIGHT - TOPY) {
        obstacle.erase(obstacle.begin());
    }
    for (int i = 0; i < obstacle.size(); i++) {
        obstacle[i].moveO();
        mvprintw(WIDTH + 3, i, "%d", i);
    }
    obstacle.push_back(Obstacle());
    obstacle[obstacle.size() - 1].show();
}

//충돌 판정 함수
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
    }
    refresh();
    sleep(2);
}
void showState(int score, int time) {
    int stage = (1000000 - time) / 10000 + 1;
    mvprintw(1, 1, "score: %d stage %d", score, stage);
    refresh();
}
int numberOfObstacle() {
    int total = Obstacle::total;
    if (total < HEIGHT - TOPY + 1) {
        return 0;
    }
    return total - HEIGHT + TOPY;
}
