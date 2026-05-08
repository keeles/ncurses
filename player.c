#include "player.h"

void update_camera(camera_t *cam, player_t *player, window_t *win) {
  int margin = 5;

  // scroll down
  if (player->current_row >= cam->cam_row + cam->viewport_rows - margin)
    cam->cam_row++;
  // scroll up
  if (player->current_row <= cam->cam_row + margin)
    cam->cam_row--;
  // scroll right
  if (player->current_col >= cam->cam_col + cam->viewport_cols - margin)
    cam->cam_col++;
  // scroll left
  if (player->current_col <= cam->cam_col + margin)
    cam->cam_col--;

  // clamp so camera never goes out of map bounds
  if (cam->cam_row < 0)
    cam->cam_row = 0;
  if (cam->cam_col < 0)
    cam->cam_col = 0;
  if (cam->cam_row > win->total_rows - cam->viewport_rows)
    cam->cam_row = win->total_rows - cam->viewport_rows;
  if (cam->cam_col > win->total_cols - cam->viewport_cols)
    cam->cam_col = win->total_cols - cam->viewport_cols;
}

void move_player(window_t *win, char **map, char input, player_t *player) {
  switch (input) {
  case 'h':
    if (player->current_col == 0 || map[player->current_row][player->current_col - 1] == '#') {
      break;
    }

    player->current_col--;
    player->last_movement = 'h';
    break;
  case 'j':
    if (player->current_row == (win->total_rows - 1) ||
        map[player->current_row + 1][player->current_col] == '#') {
      break;
    }

    player->current_row++;
    player->last_movement = 'j';
    break;
  case 'k':
    if (player->current_row == 0 || map[player->current_row - 1][player->current_col] == '#') {
      break;
    }

    player->current_row--;
    player->last_movement = 'k';
    break;
  case 'l':
    if (player->current_col == (win->total_cols - 1) ||
        map[player->current_row][player->current_col + 1] == '#') {
      break;
    }

    player->current_col++;
    player->last_movement = 'l';
    break;
  }
}
