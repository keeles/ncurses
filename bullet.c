#include "bullet.h"
#include <stdlib.h>

const size_t MAX_BULLETS = 10;

void increment_bullet_pos(bullet_t *bullet, char **map, camera_t *cam, window_t *win) {
  int next_row = bullet->row + bullet->row_delta;
  int next_col = bullet->col + bullet->col_delta;

  if (map[next_row][next_col] == '#' || next_row == win->total_rows) {
    map[next_row][next_col] = ' ';
    bullet->active = 0;
    return;
  }

  mvaddch(next_row - cam->cam_row, next_col - cam->cam_col, '*');
  bullet->row += bullet->row_delta;
  bullet->col += bullet->col_delta;
}

void update_bullets(bullet_arr_t *bullets, char **map, camera_t *cam, window_t *win) {
  if (bullets->length == 0) {
    return;
  }

  for (size_t i = 0; i < bullets->length; i++) {
    bullet_t *bullet = &bullets->bullets[i];
    if (bullet->active > 0) {
      increment_bullet_pos(bullet, map, cam, win);
    }
  }
}

void shoot_bullet(player_t *player, char **map, bullet_arr_t *bullets) {
  timeout(1000); // wait up to 1000ms for input
  int dir = getch();
  timeout(-1); // restore blocking input

  int row_delta = 0, col_delta = 0;
  switch (dir) {
  case 'h':
    col_delta = -1;
    break;
  case 'l':
    col_delta = 1;
    break;
  case 'k':
    row_delta = -1;
    break;
  case 'j':
    row_delta = 1;
    break;
  default:
    return; // ERR or unrecognized key, cancel shot
  }

  int next_row = player->current_row + row_delta;
  int next_col = player->current_col + col_delta;

  // check if player right next to wall to avoid adding bullet to mem
  if (map[next_row][next_col] == '#') {
    map[next_row][next_col] = ' ';
    return;
  }

  bullet_t new_bullet = {.col = next_col,
                         .row = next_row,
                         .col_delta = col_delta,
                         .row_delta = row_delta,
                         .active = 1};

  if (bullets->length == bullets->capacity) {
    bullets->bullets[0] = new_bullet;
    bullets->length = 1;
    return;
  }

  bullets->bullets[bullets->length] = new_bullet;
  bullets->length++;
}

bullet_arr_t *init_bullets() {
  bullet_t *bullets = malloc(sizeof(bullet_t) * MAX_BULLETS);
  bullet_arr_t *bullet_arr = malloc(sizeof(bullet_arr_t));
  bullet_arr->bullets = bullets;
  bullet_arr->capacity = MAX_BULLETS;
  bullet_arr->length = 0;

  return bullet_arr;
}
