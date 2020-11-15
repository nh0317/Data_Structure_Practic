// 게임 함수를 실행하기 위한 테스트 코드입니다.
// 참고만하셔도 좋습니다.
// ui는 생각하지 않았습니다.
// 지금은 스테이지가 이어서 시작됩니다.

using namespace std;
#include "game.hpp"
#include "ui.hpp"
#include <iostream>
#include <ncurses.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <vector>

#define ENTER 10 // enter입력값으로 사용
#define STAGE 6  //스테이지의 개수

int main(void) {
    Character character = Character();
    vector<Obstacle> obstacle;

    int time = 1000000;
    int cnt = 0;

    //커서모드 시작
    initscr();

    // init 기본모드 설정
    noecho();
    cbreak();

    //특수키 사용 (방향키 등)
    keypad(stdscr, TRUE);

    //윈도우 테두리 시작위치
    int start_y = 0, start_x = 0;

    // while문 입력확인에 사용
    int ch;

    // 커서의 위치 확인
    int position = 1;

    setWindow1(HEIGHT, WIDTH, start_x, start_y);

    // 키 입력시까지 반복
    while ((ch = getch()) != KEY_F(8)) {
        switch (ch) {
        case ENTER:
            if (position == 1) {
                // GAME START 출력
                clear();
                for (int i = 0; i < STAGE; i++) {
                    cnt++;
                    if (cnt == STAGE) {
                        if (game(time, character, obstacle) != 0) {
                            //스테이지를 모두 클리어하는 경우
                            initscr();
                            clear();
                            mvprintw(HEIGHT / 2, WIDTH / 2, "CLEAR!");
                            refresh();
                            sleep(3);
                            endwin();
                            time -= 10000; // 속도 증가 적당히 설정했습니다.
                        } else
                            break;
                    }

                    //장애물에 충돌하지 않으면 게임 실행
                    else if (game(time, character, obstacle) != 0) {
                        initscr();
                        clear();
                        mvprintw(HEIGHT / 2, WIDTH / 2, "next..");
                        refresh();
                        sleep(1);
                        clear();
                        endwin();
                        time -= 5000; // 속도 증가 적당히 설정했습니다.
                    }
                    //장애물에 충돌하여 게임이 종료되는 경우
                    else {

                        endwin();
                        return 0;
                    }
                }
                endwin();
                return 0;
            } else if (position == 2) {
                explain();
                // GAME EXPLAIN 출력
                while (getch() != ENTER) {
                    sleep(0);
                }
                setWindow1(HEIGHT, WIDTH, start_x, start_y);
                print_explain();
                refresh();
                break;
            } else if (position == 3) {
                // RANKING 출력
                break;
            } else if (position == 4) {
                //프로그램 종료
                endwin();
                return 0;
            }
        case KEY_DOWN:
            if (position == 1) {
                ++position;
                print_explain();
                break;
            } else if (position == 2) {
                ++position;
                print_rank();
                break;
            } else if (position == 3) {
                ++position;
                print_exit();
                break;
            } else if (position == 4) {
                position = 1;
                print_start();
                break;
            }
        case KEY_UP:
            if (position == 1) {
                position = 4;
                print_exit();
                break;
            } else if (position == 2) {
                --position;
                print_start();
                break;
            } else if (position == 3) {
                --position;
                print_explain();
                break;
            } else if (position == 4) {
                --position;
                print_rank();
                break;
            }
        }
    }

    refresh();

    //커서모드 종료
    endwin();
    return 0;
}
