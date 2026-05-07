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

typedef struct Bullet {
  int row, col;
  int row_delta, col_delta;
  int active;
} bullet_t;

typedef struct {
  bullet_t *bullets;
  size_t capacity;
  size_t length;
} bullet_arr_t;
