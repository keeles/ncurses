#include <ncurses.h>
#pragma once

typedef struct Player {
  int current_col;
  int current_row;
  int active;
  char symbol;
  char last_movement;
} player_t;

typedef struct Enemy {
  int current_col;
  int current_row;
  int active;
  char symbol;
} enemy_t;

typedef struct {
  enemy_t *enemies;
  size_t capacity;
  size_t length;
} enemy_arr_t;

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

typedef struct Cell {
  struct Cell *parent;
  int x;
  int y;
  int g;
  int h;
  int f;
  int in_open;
  int in_closed;
} cell_t;

typedef struct Cell_Queue {
  cell_t **items;
  int num_entries;
  int capacity;
} cell_q_t;

typedef struct Cell_State {
  cell_t *cells;
  cell_q_t *open_set;
  cell_q_t *dirty_items;
} cell_state_t;
