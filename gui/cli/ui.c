#include "ui.h"

void initUI(bool flag) {
  if (flag) {
    initscr();
    keypad(stdscr, 1);
    noecho();
    curs_set(0);
    cbreak();
    nodelay(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
  } else {
    endwin();
  }
}

void field_borders() {
  for (int y = 0; y <= FIELD_HEIGHT + 1; y++) {
    for (int x = 0; x <= FIELD_WIDTH; x++) {
      mvaddch(y, x * 2, ACS_CKBOARD);
      mvaddch(y, x * 2 + 1, ACS_CKBOARD);
    }
  }

  for (int y = 1; y <= FIELD_HEIGHT; y++) {
    for (int x = 1; x <= FIELD_WIDTH - 1; x++) {
      mvaddch(y, x * 2, ' ');
      mvaddch(y, x * 2 + 1, ' ');
    }
  }
}

void field_menu(int high_score, int score, int level, int **shape,
                int color_shape) {
  for (int y = 0; y <= FIELD_HEIGHT + 1; y++) {
    for (int x = FIELD_WIDTH + 1; x < FIELD_WIDTH + 9; x++) {
      mvaddch(y, x * 2, ACS_CKBOARD);
      mvaddch(y, x * 2 + 1, ACS_CKBOARD);
    }
  }

  for (int y = 1; y <= FIELD_HEIGHT; y++) {
    for (int x = FIELD_WIDTH + 1; x < FIELD_WIDTH + 9 - 1; x++) {
      mvaddch(y, x * 2, ' ');
      mvaddch(y, x * 2 + 1, ' ');
    }
  }
  int FIELD_WIDTH_text = FIELD_WIDTH + 13;
  int FIELD_WIDTH_value = FIELD_WIDTH + 15;
  mvprintw(2, FIELD_WIDTH_text, "High score:");
  mvprintw(3, FIELD_WIDTH_value, "%d", high_score);

  mvprintw(5, FIELD_WIDTH_text, "Score:");
  mvprintw(6, FIELD_WIDTH_value, "%d", score);

  mvprintw(8, FIELD_WIDTH_text, "Level:");
  mvprintw(9, FIELD_WIDTH_value, "%d", level);

  mvprintw(11, FIELD_WIDTH_text, "Next:");
  if (shape == NULL) {
    mvprintw(12, FIELD_WIDTH_value, "FIGURE");
  } else {
    attron(COLOR_PAIR(color_shape));
    for (int i = 0; i != SHAPE_SIZE; ++i) {
      mvaddch(13 + shape[i][0], FIELD_WIDTH_value + 4 + shape[i][1] * 2,
              ACS_CKBOARD);
    }
    attroff(COLOR_PAIR(color_shape));
  }

  refresh();
}

void start_display() {
  field_borders();
  int **shape = NULL;
  field_menu(0, 0, 0, shape, 0);

  attron(COLOR_PAIR(1));
  mvprintw(FIELD_HEIGHT / 2, 0, "-Press Enter to Start-");
  attroff(COLOR_PAIR(1));
  refresh();
}

void start_load_display() {
  field_borders();
  int **shape = NULL;
  field_menu(0, 0, 0, shape, 0);

  for (int x = 1; x <= FIELD_WIDTH - 1; x++) {
    mvaddch(FIELD_HEIGHT / 2 - 1, x * 2, ACS_S3);
    mvaddch(FIELD_HEIGHT / 2 + 1, x * 2, ACS_S3);
  }
  attron(COLOR_PAIR(1));
  for (int x = 1; x <= FIELD_WIDTH - 1; x++) {
    mvaddch(FIELD_HEIGHT / 2, x * 2, ACS_CKBOARD);
    refresh();
    napms(140);
  }

  attroff(COLOR_PAIR(1));
  refresh();
}

void pause_display() {
  field_borders();
  int **shape = NULL;
  field_menu(0, 0, 0, shape, 0);

  attron(COLOR_PAIR(1));
  mvprintw(FIELD_HEIGHT / 2, 0, "Press 'p' to continue.");
  attroff(COLOR_PAIR(1));
  refresh();
}

void end_game_display() {
  field_borders();
  int **shape = NULL;
  field_menu(0, 0, 0, shape, 0);

  attron(COLOR_PAIR(1));
  mvprintw(FIELD_HEIGHT / 2, 0, "You're lost this game.");
  attroff(COLOR_PAIR(1));
  refresh();
}

void draw_field(int **field) {
  clear();
  field_borders();
  for (int y = 1; y < FIELD_HEIGHT; y++) {
    for (int x = 1; x < FIELD_WIDTH; x++) {
      if (field[y][x] >= 1 && field[y][x] <= 6) {
        attron(COLOR_PAIR(field[y][x]));
      }
      mvaddch(y + 1, x * 2, field[y][x] ? ACS_CKBOARD : ' ');
      if (field[y][x] >= 1 && field[y][x] <= 6) {
        attroff(COLOR_PAIR(field[y][x]));
      }
    }
  }
  refresh();
}

void draw_shape(int shape[4][2], int x, int y, int color) {
  attron(COLOR_PAIR(color));

  for (int i = 0; i != SHAPE_SIZE; ++i) {
    mvaddch(y + shape[i][0], ((x + shape[i][1]) * 2), ACS_CKBOARD);
  }

  attroff(COLOR_PAIR(color));
}
