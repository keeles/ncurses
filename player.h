#include <ncurses.h>
#pragma once

typedef struct Player {
  char symbol;
  int current_col;
  int current_row;
} player_t;

typedef struct Window {
  WINDOW *win;
  int total_rows;
  int total_cols;
} window_t;

typedef struct Camera {
  int cam_row;
  int cam_col;
  int viewport_rows;
  int viewport_cols;
} camera_t;

void update_camera(camera_t *cam, player_t *player, window_t *win);
void move_player(window_t *window, char **map, char input, player_t *player,
                 camera_t *cam);
