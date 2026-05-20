#include "map.h"
#include "types.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

const char *win_screen[] = {
    "$       $     $       $      $",
    " $     $    $   $     $      $",
    "  $   $   $       $   $      $",
    "   $ $   $         $  $      $",
    "    $     $       $   $      $",
    "    $      $     $    $      $",
    "    $       $   $       $  $  ",
    "    $         $           $   ",
    "",
    "$       $     $       $      $",
    "$       $     $       $ $    $",
    "$   $   $     $       $  $   $",
    "$   $   $     $       $   $  $",
    "  $   $       $       $    $ $",
    "  $   $       $       $      $",
    NULL,
};

const char *lose_screen[] = {
    "$       $     $       $      $",
    " $     $    $   $     $      $",
    "  $   $   $       $   $      $",
    "   $ $   $         $  $      $",
    "    $     $       $   $      $",
    "    $      $     $    $      $",
    "    $       $   $       $  $  ",
    "    $         $           $   ",
    "",
    "$         $        $    $$$$$$",
    "$       $   $    $   $  $     ",
    "$     $       $   $     $     ",
    "$     $       $    $    $$$$$$",
    "$       $   $    $   $  $     ",
    "$$$$$     $        $    $$$$$$",
    NULL,
};

const char *quit_screen[] = {
    "$       $     $       $      $",
    " $     $    $   $     $      $",
    "  $   $   $       $   $      $",
    "   $ $   $         $  $      $",
    "    $     $       $   $      $",
    "    $      $     $    $      $",
    "    $       $   $       $  $  ",
    "    $         $           $   ",
    "",
    "   $     $     $    $    $$$$$",
    " $   $   $     $    $      $  ",
    "$     $  $     $    $      $  ",
    " $  $ $  $     $    $      $  ",
    "  $  $    $   $     $      $  ",
    "   $  $     $       $      $  ",
    NULL,
};

int count_neighbour_walls(char **map, window_t *win, int row, int col) {
  int neighbour_walls = 0;

  for (int di = -1; di <= 1; di++) {
    for (int dj = -1; dj <= 1; dj++) {
      if (di == 0 && dj == 0)
        continue; // skip self

      int ni = row + di;
      int nj = col + dj;

      // out of bounds counts as a wall
      if (ni < 1 || ni >= win->total_rows || nj < 0 || nj >= win->total_cols) {
        neighbour_walls++;
        continue;
      }

      if (map[ni][nj] == '#')
        neighbour_walls++;
    }
  }

  return neighbour_walls;
}

void smooth_map(char **map, window_t *win) {
  for (int i = 1; i < win->total_rows; i++) {
    for (int j = 0; j < win->total_cols; j++) {
      int neighbour_walls = count_neighbour_walls(map, win, i, j);
      if (neighbour_walls > 4) {
        map[i][j] = '#';
      }
    }
  }
}

void draw_map(char **map, camera_t *cam) {
  for (int row = 1; row < cam->viewport_rows; row++) {
    for (int col = 0; col < cam->viewport_cols; col++) {
      int world_row = cam->cam_row + row;
      int world_col = cam->cam_col + col;
      mvaddch(row, col, map[world_row][world_col]);
    }
  }
}

void game_win(camera_t *cam) {
  erase();
  int num_rows = sizeof(win_screen) / sizeof(win_screen[0]);
  int start_row = (cam->viewport_rows / 2) - (num_rows / 2);

  for (int i = 0; win_screen[i] != NULL; i++) {
    int len = strlen(win_screen[i]);
    int start_col = (cam->viewport_cols / 2) - (len / 2);
    mvprintw(start_row + i, start_col, "%s", win_screen[i]);
  }
  refresh();
  sleep(10);
}

void game_lose(camera_t *cam) {
  erase();
  int num_rows = sizeof(lose_screen) / sizeof(lose_screen[0]);
  int start_row = (cam->viewport_rows / 2) - (num_rows / 2);

  for (int i = 0; lose_screen[i] != NULL; i++) {
    int len = strlen(lose_screen[i]);
    int start_col = (cam->viewport_cols / 2) - (len / 2);
    mvprintw(start_row + i, start_col, "%s", lose_screen[i]);
  }
  refresh();
  sleep(10);
}

void game_quit(camera_t *cam) {
  erase();
  int num_rows = sizeof(quit_screen) / sizeof(quit_screen[0]);
  int start_row = (cam->viewport_rows / 2) - (num_rows / 2);

  for (int i = 0; quit_screen[i] != NULL; i++) {
    int len = strlen(quit_screen[i]);
    int start_col = (cam->viewport_cols / 2) - (len / 2);
    mvprintw(start_row + i, start_col, "%s", quit_screen[i]);
  }
  refresh();
  sleep(2);
}

char **init_map(window_t *win) {
  char **map = malloc(sizeof(char *) * win->total_rows);

  // start at 1 to leave room for info row
  for (int i = 1; i < win->total_rows; i++) {
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
    smooth_map(map, win);
  }

  return map;
}

int print_status_line(int viewport_cols, int remaining_enemies, time_t end_time) {
  time_t current_time = time(NULL);
  long remaining = (long)difftime(end_time, current_time);
  if (remaining < 0)
    remaining = 0; // clamp so it doesn't go negative
  long minutes = remaining / 60;
  long seconds = remaining % 60;

  char timer_str[12];
  snprintf(timer_str, sizeof(timer_str), "%02ld:%02ld", minutes, seconds);

  char enemy_str[22];
  snprintf(enemy_str, sizeof(enemy_str), "REMAINING ENEMIES: %d", remaining_enemies);

  char status_buffer[viewport_cols];
  snprintf(status_buffer, sizeof(status_buffer), "%s      %s", timer_str, enemy_str);
  mvprintw(0, 0, "%s", status_buffer);

  return remaining;
}
