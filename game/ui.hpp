#ifndef __UI_H__
#define __UI_H__
#include <ncurses.h>

#define ENTER 10

//높이, 너비, 시작위치(x, y)를 입력받아서 테두리 형성
void setWindow(int height, int width, int x, int y);

void print_start();
void print_explain();
void print_rank();
void print_exit();

#endif
