#include "map.h"
#include "player.h"
#include <ncurses.h>
#include <stdlib.h>

int count_neighbour_walls(char **map, window_t *win, int row, int col) {
  int neighbour_walls = 0;

  for (int di = -1; di <= 1; di++) {
    for (int dj = -1; dj <= 1; dj++) {
      if (di == 0 && dj == 0)
        continue; // skip self

      int ni = row + di;
      int nj = col + dj;

      // out of bounds counts as a wall
      if (ni < 0 || ni >= win->total_rows || nj < 0 || nj >= win->total_cols) {
        neighbour_walls++;
        continue;
      }

      if (map[ni][nj] == '#')
        neighbour_walls++;
    }
  }
  return neighbour_walls;
}

void smooth_map(char **map, window_t *win, int draw) {
  for (int i = 0; i < win->total_rows; i++) {
    for (int j = 0; j < win->total_cols; j++) {
      int neighbour_walls = count_neighbour_walls(map, win, i, j);
      if (neighbour_walls > 4) {
        map[i][j] = '#';
      }
      if (draw) {
        mvaddch(i, j, map[i][j]);
      }
    }
  }
}

void draw_map(char **map, window_t *win) {
  for (int i = 0; i < win->total_rows; i++) {
    for (int j = 0; j < win->total_cols; j++) {
      mvaddch(i, j, map[i][j]);
    }
  }
}

char **init_map(window_t *win) {
  char **map = malloc(sizeof(char *) * win->total_rows);

  for (int i = 0; i < win->total_rows; i++) {
    map[i] = malloc(sizeof(char) * win->total_cols);
    for (int j = 0; j < win->total_cols; j++) {
      int range = 100;
      int random_val = (rand() % range) + 1;
      char c;
      if (random_val > 80) {
        c = '#';
      } else {
        c = ' ';
      }
      map[i][j] = c;
    }
  }

  for (int i = 0; i < 5; i++) {
    smooth_map(map, win, 0);
  }

  return map;
}
