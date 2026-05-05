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

  int total_rows, total_cols;
  getmaxyx(win, total_rows, total_cols);
  int current_row = total_rows / 2;
  int current_col = total_cols / 2;

  window_t window = {.win = win, .total_rows = total_rows, .total_cols = total_cols};
  player_t player = {.symbol = '@', .current_col = current_col, .current_row = current_row};

  mvaddch(player.current_row, player.current_col, player.symbol);

  char **map = init_map(&window);
  draw_map(map, &window);
  int play = 1;
  while (play) {
    char c = getch();
    if (c == 'q') {
      play = 0;
      break;
    }

    move_player(&window, map, c, &player);
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
