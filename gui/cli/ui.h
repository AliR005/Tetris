#ifndef UI_H
#define UI_H

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20
#define SHAPE_SIZE 4

#include <ncurses.h>
#include <stdbool.h>

void initUI(bool flag);
void field_borders();
void field_menu(int high_score, int score, int level, int **shape,
                int color_shape);
void start_display();
void pause_display();
void end_game_display();
void draw_field(int **field);
void draw_shape(int shape[4][2], int x, int y, int color);

void start_load_display();
#endif
