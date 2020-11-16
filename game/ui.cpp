#include "ui.hpp"
#include "game.hpp"
#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void setWindow1(int height, int width, int x, int y) {
    WINDOW *my_win = newwin(HEIGHT, WIDTH, y, x);
    refresh();

    box(my_win, 0, 0);

    print_exit();
    print_rank();
    print_explain();
    print_start();

    wrefresh(my_win);
}

void setWindow2(int height, int width, int x, int y) {
    // WINDOW *my_win = newwin(HEIGHT, WIDTH, y, x);
    WINDOW *my_win = newwin(height, width, y, x);
    refresh();

    box(my_win, 0, 0);

    wrefresh(my_win);
}

void print_start() {
    mvprintw(HEIGHT / 2 - 3, (WIDTH - strlen("GAME START")) / 2, "GAME START");
}
void print_explain() {
    mvprintw(HEIGHT / 2 - 1, (WIDTH - strlen("GAME EXPLAIN")) / 2,
             "GAME EXPLAIN");
}
void print_rank() {
    mvprintw(HEIGHT / 2 + 1, (WIDTH - strlen("RANKING")) / 2, "RANKING");
}
void print_exit() {
    mvprintw(HEIGHT / 2 + 3, (WIDTH - strlen("GAME EXIT")) / 2, "GAME EXIT");
}
