#include "ui.hpp"
#include "game.hpp"
#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void setWindow(int height, int width, int x, int y) {
    WINDOW *my_win = newwin(HEIGHT, WIDTH, y, x);
    refresh();

    box(my_win, 0, 0);
    explain();
    mvwprintw(my_win, HEIGHT / 2 + 3, (WIDTH - strlen("GAME EXIT")) / 2,
              "GAME EXIT");
    mvwprintw(my_win, HEIGHT / 2 + 1, (WIDTH - strlen("RANKING")) / 2,
              "RANKING");
    mvwprintw(my_win, HEIGHT / 2 - 1, (WIDTH - strlen("GAME EXPLAIN")) / 2,
              "GAME EXPLAIN");
    mvwprintw(my_win, HEIGHT / 2 - 3, (WIDTH - strlen("GAME START")) / 2,
              "GAME START");

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
