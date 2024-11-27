#ifndef BACKEND_H
#define BACKEND_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FIELD_HEIGHT 20
#define FIELD_WIDTH 10
#define SHAPE_SIZE 4
#define SHAPE_COUNT 7
#define DEFAULT_SPEED 200

typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

void userInput(UserAction_t action);

GameInfo_t updateCurrentState();

typedef struct {
  int figure[4][2];
  int x;
  int y;
  int color;
} Shape_t;

Shape_t getShape();
Shape_t getNextShape();

#define I_SHAPE                        \
  {                                    \
    {0, -1}, {0, -2}, {0, 0}, { 0, 1 } \
  }  // +

#define J_SHAPE                       \
  {                                   \
    {0, 0}, {0, 1}, {0, 2}, { -1, 2 } \
  }  // +

#define L_SHAPE                         \
  {                                     \
    {0, 0}, {0, -1}, {0, 1}, { -1, -1 } \
  }  // +

#define O_SHAPE                          \
  {                                      \
    {0, -1}, {-1, -1}, {-1, 0}, { 0, 0 } \
  }  // +

#define S_SHAPE                         \
  {                                     \
    {-1, 0}, {-1, 1}, {0, 0}, { 0, -1 } \
  }  // +

#define T_SHAPE                        \
  {                                    \
    {0, 0}, {0, -1}, {0, 1}, { -1, 0 } \
  }  // +

#define Z_SHAPE                         \
  {                                     \
    {0, 0}, {-1, 0}, {0, 1}, { -1, -1 } \
  }  // +

void generate_shape(Shape_t *shape);

void processStart();
void freeResources();

bool can_move_down();
bool can_move_left();
bool can_move_right();
bool can_rotate();
void rotate_shape();
void save_shape_on_field();
int delete_line();
void add_score(int count_del_lines);
void end_game();

int high_score();
void set_high_score();

#endif
