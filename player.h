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

void move_player(window_t *window, char **map, char input, player_t *player);
