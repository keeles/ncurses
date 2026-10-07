#include "bullet.h"
#include "enemies.h"
#include "map.h"
#include "player.h"
#include "types.h"
#include <ncurses.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
  initscr();
  curs_set(0);
  noecho();
  WINDOW *win = stdscr;
  keypad(win, TRUE);

  if (has_colors() == TRUE) {
    start_color();

    // player color
    init_pair(1, COLOR_MAGENTA, COLOR_BLACK);

    // enemy color
    init_pair(2, COLOR_GREEN, COLOR_BLACK);

    // bullet color
    init_pair(3, COLOR_RED, COLOR_BLACK);
  }

  int viewport_rows, viewport_cols;
  getmaxyx(win, viewport_rows, viewport_cols);

  // total map
  int total_rows = viewport_rows * 4;
  int total_cols = viewport_cols * 4;

  // start player in center of map
  int current_row = total_rows / 2;
  int current_col = total_cols / 2;

  // camera view
  int camera_row = (total_rows / 2) - (viewport_rows / 2);
  int camera_col = (total_cols / 2) - (viewport_cols / 2);

  window_t window = {.win = win, .total_rows = total_rows, .total_cols = total_cols};
  player_t player = {.symbol = '@', .current_col = current_col, .current_row = current_row};
  camera_t camera = {.cam_row = camera_row,
                     .cam_col = camera_col,
                     .viewport_cols = viewport_cols,
                     .viewport_rows = viewport_rows};

  // init map
  char **map = init_map(&window);

  // cells for path finding
  cell_t *cells = malloc(window.total_cols * window.total_rows * sizeof(cell_t));

  cell_q_t open_set = {.num_entries = 0,
                       .capacity = window.total_cols * window.total_rows,
                       .items = malloc(sizeof(cell_t *) * window.total_cols * window.total_rows)};

  cell_q_t dirty_items = {.num_entries = 0,
                          .capacity = window.total_cols * window.total_rows,
                          .items =
                              malloc(sizeof(cell_t *) * window.total_cols * window.total_rows)};

  cell_state_t cell_state = {.cells = cells, .open_set = &open_set, .dirty_items = &dirty_items};

  for (int y = 0; y < window.total_rows; y++) {
    for (int x = 0; x < window.total_cols; x++) {
      if (x == player.current_col && y == player.current_row) {
        cell_state.cells[y * window.total_cols + x] = build_cell(NULL, &player, x, y);
      } else {
        cell_state.cells[y * window.total_cols + x] = build_cell(NULL, &player, x, y);
      }
    }
  }
  // bullets
  bullet_arr_t *bullet_arr = init_bullets();

  // draw initial state
  draw_map(map, &camera);

  update_camera(&camera, &player, &window);

  if (has_colors() == TRUE) {
    attron(COLOR_PAIR(1));
  }

  mvaddch(player.current_row - camera.cam_row, player.current_col - camera.cam_col, player.symbol);

  if (has_colors() == TRUE) {
    attroff(COLOR_PAIR(1));
  }

  if (has_colors() == TRUE) {
    attron(COLOR_PAIR(2));
  }
  // enemies
  enemy_arr_t *enemy_arr = init_enemies(map, &window);

  if (has_colors() == TRUE) {
    attroff(COLOR_PAIR(2));
  }

  // 8 minute timer
  time_t start_time = time(NULL);
  time_t end_time = start_time + (8 * 60);

  // game loop
  int play = 1;
  while (play) {
    timeout(100);
    char c = getch();

    if (c == 'q') {
      game_quit(&camera);
      break;
    }

    erase();

    if (c == 'f') {
      if (has_colors() == TRUE) {
        attron(COLOR_PAIR(3));
      }

      shoot_bullet(&player, map, bullet_arr, &window);

      if (has_colors() == TRUE) {
        attroff(COLOR_PAIR(3));
      }
    }

    draw_map(map, &camera);
    move_player(&window, map, c, &player, &camera);

    if (has_colors() == TRUE) {
      attron(COLOR_PAIR(3));
    }

    update_bullets(bullet_arr, map, &camera, &window, enemy_arr);

    if (has_colors() == TRUE) {
      attroff(COLOR_PAIR(3));
    }

    if (has_colors() == TRUE) {
      attron(COLOR_PAIR(2));
    }

    int remaining_enemies =
        update_enemies(enemy_arr, map, &cell_state, &camera, &window, play, &player);
    int remaining_time = print_status_line(viewport_cols, remaining_enemies, end_time);

    if (has_colors() == TRUE) {
      attroff(COLOR_PAIR(2));
    }

    if (has_colors() == TRUE) {
      attron(COLOR_PAIR(1));
    }

    mvaddch(player.current_row - camera.cam_row, player.current_col - camera.cam_col,
            player.symbol);

    if (has_colors() == TRUE) {
      attroff(COLOR_PAIR(1));
    }

    refresh();

    if (remaining_enemies == 0) {
      game_win(&camera);
      break;
    }

    if (remaining_time == 0) {
      game_lose(&camera);
      break;
    }

    if (play == 10) {
      play = 1;
    } else {
      play++;
    }
  }

  // free memory
  if (map) {
    for (int i = 0; i < total_rows; i++) {
      if (map[i]) {
        free(map[i]);
      }
    }
    free(map);
  }

  if (cells) {
    free(cells);
  }

  if (open_set.items) {
    free(open_set.items);
  }

  if (dirty_items.items) {
    free(dirty_items.items);
  }

  if (bullet_arr) {
    free(bullet_arr->bullets);
    free(bullet_arr);
  }

  if (enemy_arr) {
    free(enemy_arr->enemies);
    free(enemy_arr);
  }

  if (has_colors() == TRUE) {
    attroff(COLOR_PAIR(1));
    attroff(COLOR_PAIR(2));
    attroff(COLOR_PAIR(3));
  }

  endwin();
  return 0;
}
