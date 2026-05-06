#include "map.h"
#include "player.h"
#include <ncurses.h>
#include <stdlib.h>

int main(void) {
  initscr();
  curs_set(0);
  noecho();
  WINDOW *win = stdscr;
  keypad(win, TRUE);

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

  update_camera(&camera, &player, &window);
  mvaddch(player.current_row - camera.cam_row, player.current_col - camera.cam_col, player.symbol);

  char **map = init_map(&window);
  draw_map(map, &camera);
  int play = 1;
  while (play) {
    char c = getch();
    if (c == 'q') {
      play = 0;
      break;
    }

    move_player(&window, map, c, &player, &camera);
  }

  if (map) {
    for (int i = 0; i < total_rows; i++) {
      if (map[i]) {
        free(map[i]);
      }
    }
    free(map);
  }

  endwin();
  return 0;
}
