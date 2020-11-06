#include "game.hpp"
#include <iostream>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include <vector>

//게임 함수를 실행하기 위한 테스트 코드입니다.
//참고만하셔도 좋습니다.
// ui는 생각하지 않았습니다.
//지금은 스테이지가 이어서 시작됩니다.

#define STAGE 6 //스테이지의 개수

using namespace std;
int main(void) {

    //캐릭터와 장애물 클래스 생성
    Character character = Character();
    vector<Obstacle> obstacle;

    int time = 1000000; //초기 속도 1초
    int cnt = 0;
    //스테이지 6개
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
            break;
        }
    }
    return 0;
}
