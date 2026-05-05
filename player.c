#include "player.h"
#include <ncurses.h>

void move_player(window_t *win, char **map, char input, player_t *player) {
  switch (input) {
  case 'h':
    if (player->current_col == 0 || map[player->current_row][player->current_col - 1] == '#') {
      break;
    }

    mvaddch(player->current_row, player->current_col, ' ');
    player->current_col--;
    mvaddch(player->current_row, player->current_col, player->symbol);
    break;
  case 'j':
    if (player->current_row == (win->total_rows - 1) ||
        map[player->current_row + 1][player->current_col] == '#') {
      break;
    }

    mvaddch(player->current_row, player->current_col, ' ');
    player->current_row++;
    mvaddch(player->current_row, player->current_col, player->symbol);
    break;
  case 'k':
    if (player->current_row == 0 || map[player->current_row - 1][player->current_col] == '#') {
      break;
    }

    mvaddch(player->current_row, player->current_col, ' ');
    player->current_row--;
    mvaddch(player->current_row, player->current_col, player->symbol);
    break;
  case 'l':
    if (player->current_col == (win->total_cols - 1) ||
        map[player->current_row][player->current_col + 1] == '#') {
      break;
    }

    mvaddch(player->current_row, player->current_col, ' ');
    player->current_col++;
    mvaddch(player->current_row, player->current_col, player->symbol);
    break;
  }

  doupdate();
}
