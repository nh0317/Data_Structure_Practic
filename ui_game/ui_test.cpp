#include "game.hpp"
#include <iostream>
#include <ncurses.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <vector>

#define ENTER 10 // enter입력값으로 사용
#define STAGE 6  //스테이지의 개수

int main(void) {
    Character character = Character(); // test.cpp
    vector<Obstacle> obstacle;         // test.cpp

    int time = 1000000; // test.cpp
    int cnt = 0;        // test.cpp

    //커서모드 시작
    initscr();

    // init 기본모드 설정
    noecho();
    cbreak();

    //색깔모드 시작
    // start_color();

    //커서 안보이게 (0~2)
    curs_set(2);

    //특수키 사용 (방향키 등)
    keypad(stdscr, TRUE);

    //윈도우 테두리 시작위치
    int start_y = 0, start_x = 0;

    // while문 입력확인에 사용
    int ch;

    // 커서의 위치 확인
    int position = 1;

    WINDOW *my_win = newwin(HEIGHT, WIDTH, start_y, start_x);
    refresh();

    box(my_win, 0, 0);

    mvwprintw(my_win, HEIGHT / 2 + 3, (WIDTH - strlen("GAME EXIT")) / 2,
              "GAME EXIT");
    mvwprintw(my_win, HEIGHT / 2 + 1, (WIDTH - strlen("RANKING")) / 2,
              "RANKING");
    mvwprintw(my_win, HEIGHT / 2 - 1, (WIDTH - strlen("GAME EXPLAIN")) / 2,
              "GAME EXPLAIN");
    init_pair(1, COLOR_WHITE, COLOR_BLACK); //현재 미사용
    init_pair(2, COLOR_BLACK, COLOR_WHITE); //현재 미사용
    // attron(COLOR_PAIR(1));
    mvwprintw(my_win, HEIGHT / 2 - 3, (WIDTH - strlen("GAME START")) / 2,
              "GAME START");

    wrefresh(my_win);

    // 키 입력시까지 반복
    while ((ch = getch()) != KEY_F(8)) {
        switch (ch) {
        case ENTER:
            if (position == 1) {
                // GAME START 출력  test.cpp
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
                // GAME EXPLAIN 출력
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
                // attroff(COLOR_PAIR(1));
                // attron(COLOR_PAIR(2));
                mvwprintw(my_win, HEIGHT / 2 - 1,
                          (WIDTH - strlen("GAME EXPLAIN")) / 2, "GAME EXPLAIN");
                wrefresh(my_win);
                break;
            } else if (position == 2) {
                ++position;
                // attroff(COLOR_PAIR(1));
                // attron(COLOR_PAIR(2));
                mvwprintw(my_win, HEIGHT / 2 + 1,
                          (WIDTH - strlen("RANKING")) / 2, "RANKING");
                wrefresh(my_win);
                break;
            } else if (position == 3) {
                ++position;
                // attroff(COLOR_PAIR(1));
                // attron(COLOR_PAIR(2));
                mvwprintw(my_win, HEIGHT / 2 + 3,
                          (WIDTH - strlen("GAME EXIT")) / 2, "GAME EXIT");
                wrefresh(my_win);
                break;
            } else if (position == 4) {
                position = 1;
                // attroff(COLOR_PAIR(1));
                // attron(COLOR_PAIR(2));
                mvwprintw(my_win, HEIGHT / 2 - 3,
                          (WIDTH - strlen("GAME START")) / 2, "GAME START");
                wrefresh(my_win);
                break;
            }
        case KEY_UP:
            if (position == 1) {
                position = 4;
                // attroff(COLOR_PAIR(1));
                // attron(COLOR_PAIR(2));
                mvwprintw(my_win, HEIGHT / 2 + 3,
                          (WIDTH - strlen("GAME EXIT")) / 2, "GAME EXIT");
                wrefresh(my_win);
                break;
            } else if (position == 2) {
                --position;
                // attroff(COLOR_PAIR(1));
                // attron(COLOR_PAIR(2));
                mvwprintw(my_win, HEIGHT / 2 - 3,
                          (WIDTH - strlen("GAME START")) / 2, "GAME START");
                wrefresh(my_win);
                break;
            } else if (position == 3) {
                --position;
                // attroff(COLOR_PAIR(1));
                // attron(COLOR_PAIR(2));
                mvwprintw(my_win, HEIGHT / 2 - 1,
                          (WIDTH - strlen("GAME EXPLAIN")) / 2, "GAME EXPLAIN");
                wrefresh(my_win);
                break;
            } else if (position == 4) {
                --position;
                // attroff(COLOR_PAIR(1));
                // attron(COLOR_PAIR(2));
                mvwprintw(my_win, HEIGHT / 2 + 1,
                          (WIDTH - strlen("RANKING")) / 2, "RANKING");
                wrefresh(my_win);
                break;
            }
        }
    }

    refresh();

    //커서모드 종료
    endwin();
    return 0;
}
