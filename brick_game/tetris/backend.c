#include "backend.h"

static GameInfo_t game_info;
static Shape_t shape_info;
static Shape_t shape_next_info;

GameInfo_t updateCurrentState() {
  if (game_info.level <= 10 && game_info.score >= 600 * game_info.level) {
    game_info.level += 1;
    game_info.speed -= 35;
  }

  if (can_move_down()) {
    shape_info.y++;
  } else {
    save_shape_on_field();
    shape_info = shape_next_info;
    generate_shape(&shape_next_info);
    for (int i = 0; i < SHAPE_SIZE; i++) {
      memcpy(game_info.next[i], shape_next_info.figure[i],
             SHAPE_SIZE * sizeof(int));
    }
  }

  int count_del_lines = delete_line();
  add_score(count_del_lines);

  return game_info;
}

Shape_t getShape() { return shape_info; }

Shape_t getNextShape() { return shape_next_info; }

void userInput(UserAction_t action) {
  switch (action) {
    case Start:
      processStart();
      break;
    case Pause:
      break;
    case Terminate:
      freeResources();
      break;
    case Left:
      if (can_move_left()) {
        shape_info.x--;
      }
      break;
    case Right:
      if (can_move_right()) {
        shape_info.x++;
      }
      break;
    case Up:
      break;
    case Down:
      while (can_move_down()) {
        shape_info.y++;
      }
      break;
    case Action:
      if (can_rotate()) rotate_shape();
      break;

    default:
      break;
  }
}

void processStart() {
  game_info.field = malloc(FIELD_HEIGHT * sizeof(int *));
  for (int i = 0; i < FIELD_HEIGHT; i++) {
    game_info.field[i] = malloc(FIELD_WIDTH * sizeof(int));
    for (int j = 0; j < FIELD_WIDTH; j++) {
      game_info.field[i][j] = 0;
    }
  }
  game_info.next = malloc(SHAPE_SIZE * sizeof(int *));
  for (int i = 0; i < 4; i++) {
    game_info.next[i] = malloc(SHAPE_SIZE * sizeof(int));
    for (int j = 0; j < 4; j++) {
      game_info.next[i][j] = 0;
    }
  }
  game_info.score = 0;
  game_info.high_score = high_score();
  game_info.level = 1;
  game_info.speed = DEFAULT_SPEED;
  game_info.pause = 0;

  generate_shape(&shape_info);
  generate_shape(&shape_next_info);
  for (int i = 0; i < SHAPE_SIZE; i++) {
    memcpy(game_info.next[i], shape_next_info.figure[i],
           SHAPE_SIZE * sizeof(int));
  }
}

void freeResources() {
  if (game_info.field != NULL) {
    for (int i = 0; i < FIELD_HEIGHT; i++) {
      free(game_info.field[i]);
    }
    free(game_info.field);
    game_info.field = NULL;
  }

  if (game_info.next != NULL) {
    for (int i = 0; i < SHAPE_SIZE; i++) {
      free(game_info.next[i]);
    }
    free(game_info.next);
    game_info.next = NULL;
  }
}

void generate_shape(Shape_t *shape) {
  int shape_index = rand() % SHAPE_COUNT;

  int shapes[SHAPE_COUNT][SHAPE_SIZE][2] = {I_SHAPE, J_SHAPE, L_SHAPE, O_SHAPE,
                                            S_SHAPE, T_SHAPE, Z_SHAPE};

  for (int i = 0; i < SHAPE_SIZE; i++) {
    for (int j = 0; j < SHAPE_SIZE; j++) {
      shape->figure[i][j] = shapes[shape_index][i][j];
    }
  }

  shape->x = FIELD_WIDTH / 2;
  shape->y = 0;

  shape->color = 1 + rand() % (6 - 1 + 1);
}

bool can_move_down() {
  bool flag = true;
  int y = shape_info.y + 1;
  int x = shape_info.x;
  for (int i = 0; i != SHAPE_SIZE; ++i) {
    if (!(y + shape_info.figure[i][0] < FIELD_HEIGHT &&
          game_info.field[y + shape_info.figure[i][0]]
                         [x + shape_info.figure[i][1]] == 0)) {
      flag = false;
    }
  }

  return flag;
}

void save_shape_on_field() {
  if (shape_info.y + shape_info.figure[0][0] < 0 ||
      shape_info.y + shape_info.figure[1][0] < 0 ||
      shape_info.y + shape_info.figure[2][0] < 0 ||
      shape_info.y + shape_info.figure[3][0] < 0) {
    end_game();
  } else {
    for (int i = 0; i != SHAPE_SIZE; ++i) {
      game_info.field[shape_info.y + shape_info.figure[i][0]]
                     [shape_info.x + shape_info.figure[i][1]] =
          shape_info.color;
    }
  }
}

void end_game() { game_info.pause = 2; }

bool can_move_left() {
  bool flag = true;
  int y = shape_info.y;
  int x = shape_info.x - 1;
  for (int i = 0; i != SHAPE_SIZE; ++i) {
    int field_x = x + shape_info.figure[i][1];
    int field_y = y + shape_info.figure[i][0];

    if (field_x <= 0 || field_y < 0 || field_y >= FIELD_HEIGHT ||
        (field_x >= 0 && game_info.field[field_y][field_x] != 0)) {
      flag = false;
      break;
    }
  }
  return flag;
}

bool can_move_right() {
  bool flag = true;
  int y = shape_info.y;
  int x = shape_info.x + 1;
  for (int i = 0; i != SHAPE_SIZE; ++i) {
    int field_x = x + shape_info.figure[i][1];
    int field_y = y + shape_info.figure[i][0];

    if (field_x >= FIELD_WIDTH || field_y < 0 || field_y >= FIELD_HEIGHT ||
        (field_x <= FIELD_WIDTH && game_info.field[field_y][field_x] != 0)) {
      flag = false;
    }
  }
  return flag;
}

bool can_rotate() {
  int temp[SHAPE_SIZE][2];
  for (int i = 0; i < SHAPE_SIZE; i++) {
    temp[i][0] = shape_info.figure[i][0];
    temp[i][1] = shape_info.figure[i][1];
  }
  for (int i = 0; i < SHAPE_SIZE; i++) {
    temp[i][0] = temp[i][1];
    temp[i][1] = -temp[i][0];
  }

  bool flag = true;
  for (int i = 0; i < SHAPE_SIZE; i++) {
    int x = shape_info.x + temp[i][1];
    int y = shape_info.y + temp[i][0];

    if (x < 0 || x >= FIELD_WIDTH || y < 0 || y >= FIELD_HEIGHT ||
        game_info.field[y][x] != 0) {
      flag = false;
    }
  }

  return flag;
}

void rotate_shape() {
  int temp[SHAPE_SIZE][2];
  for (int i = 0; i < SHAPE_SIZE; i++) {
    temp[i][0] = shape_info.figure[i][0];
    temp[i][1] = shape_info.figure[i][1];
  }
  for (int i = 0; i < SHAPE_SIZE; i++) {
    shape_info.figure[i][0] = temp[i][1];   // y
    shape_info.figure[i][1] = -temp[i][0];  // x
  }
}

int delete_line() {
  int count_del_lines = 0;
  for (int i = FIELD_HEIGHT - 1; i >= 0; --i) {
    bool is_full = true;
    for (int j = 1; j < FIELD_WIDTH; ++j) {
      if (game_info.field[i][j] == 0) {
        is_full = false;
        break;
      }
    }

    if (is_full) {
      for (int k = i; k > 0; --k) {
        for (int j = 0; j < FIELD_WIDTH; ++j) {
          game_info.field[k][j] = game_info.field[k - 1][j];
        }
      }

      for (int j = 0; j < FIELD_WIDTH; ++j) {
        game_info.field[0][j] = 0;
      }

      count_del_lines += 1;

      i++;
    }
  }
  return count_del_lines;
}

void add_score(int count_del_lines) {
  switch (count_del_lines) {
    case 1:
      game_info.score += 100;
      break;
    case 2:
      game_info.score += 300;
      break;
    case 3:
      game_info.score += 700;
      break;
    case 4:
      game_info.score += 1500;
      break;
    default:
      break;
  }

  if (game_info.score > game_info.high_score) {
    game_info.high_score = game_info.score;
    set_high_score();
  }
}

int high_score() {
  int score = 0;
  FILE *file = fopen("high_score.txt", "r");
  if (file == NULL) {
    file = fopen("high_score.txt", "w");
    if (file != NULL) {
      fprintf(file, "%d", score);
      fclose(file);
    }
    return score;
  } else {
    fscanf(file, "%d", &score);
    fclose(file);
  }
  return score;
}

void set_high_score() {
  FILE *file = fopen("high_score.txt", "w+");
  if (file != NULL) {
    fprintf(file, "%d", game_info.score);
    fclose(file);
  }
}
