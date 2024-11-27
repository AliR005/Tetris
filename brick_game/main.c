#include "../gui/cli/ui.h"
#include "tetris/backend.h"

void gameloop_settings(GameInfo_t *game_info, UserAction_t *action);
UserAction_t keys_input_controller(int ch);

int main(void) {
  srand(time(NULL));
  GameInfo_t game_info;
  UserAction_t action;

  initUI(true);
  start_display();

  timeout(100);
  gameloop_settings(&game_info, &action);

  initUI(false);

  return 0;
}

void gameloop_settings(GameInfo_t *game_info, UserAction_t *action) {
  bool flag_load_display = true;
  bool flag_Start = false;
  game_info->pause = 0;

  while (*action != Terminate) {
    int ch = getch();
    *action = keys_input_controller(ch);

    if (*action == Pause) {
      pause_display();
      game_info->pause = !game_info->pause;
      continue;
    } else if (game_info->pause == 2) {
      userInput(Terminate);
      end_game_display();
      napms(5000);
      break;
    }

    userInput(*action);
    if (game_info->pause == 0) {
      if (*action != Pause && *action != Terminate && flag_Start) {
        *game_info = updateCurrentState();
        Shape_t shape = getShape();
        Shape_t shape_next = getNextShape();
        draw_field(game_info->field);
        draw_shape(shape.figure, shape.x, shape.y, shape.color);
        field_menu(game_info->high_score, game_info->score, game_info->level,
                   game_info->next, shape_next.color);
        napms(game_info->speed);
      }
      if (*action == Start) {
        flag_Start = true;
        if (flag_load_display) {
          start_load_display();
          flag_load_display = false;
        }
      }
    }
  }
}

UserAction_t keys_input_controller(int ch) {
  UserAction_t action;
  switch (ch) {
    case '\n':
    case KEY_ENTER:
      action = Start;
      break;
    case 'p':
      action = Pause;
      break;
    case 27:
      action = Terminate;
      break;
    case KEY_LEFT:
      action = Left;
      break;
    case KEY_RIGHT:
      action = Right;
      break;
    case KEY_DOWN:
      action = Down;
      break;
    case KEY_UP:
      action = Up;
      break;
    case 32:
      action = Action;
      break;
  }

  return action;
}
