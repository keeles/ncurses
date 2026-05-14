#include "enemies.h"
#include <stdlib.h>
#include <time.h>

const size_t MAX_ENEMIES = 30;

void increment_enemy_pos(enemy_t *enemy, char **map, camera_t *cam, window_t *win, int cycles) {
  // only move every 10 cycles (1 second)
  if (cycles != 10) {
    mvaddch(enemy->current_row - cam->cam_row, enemy->current_col - cam->cam_col, enemy->symbol);
    return;
  }

  int next_row, next_col;
  do {
    int col_delta = (rand() % 3) - 1;
    int row_delta = (rand() % 3) - 1;
    next_row = enemy->current_row + row_delta;
    next_col = enemy->current_col + col_delta;

    if (next_row <= 1 || next_row >= win->total_rows) {
      next_row = enemy->current_row;
    }

    if (next_col < 0 || next_col >= win->total_cols) {
      next_col = enemy->current_col;
    }

  } while (map[next_row][next_col] == '#');

  mvaddch(next_row - cam->cam_row, next_col - cam->cam_col, enemy->symbol);
  enemy->current_row = next_row;
  enemy->current_col = next_col;
}

int update_enemies(enemy_arr_t *enemies, char **map, camera_t *cam, window_t *win, int cycles) {
  if (enemies->length == 0) {
    return 0;
  }

  int remaining_enemies = 0;
  for (size_t i = 0; i < enemies->length; i++) {
    enemy_t *enemy = &enemies->enemies[i];
    if (enemy->active > 0) {
      increment_enemy_pos(enemy, map, cam, win, cycles);
      remaining_enemies++;
    }
  }

  return remaining_enemies;
}

void spawn_enemies(enemy_arr_t *enemies, char **map, window_t *win) {
  for (size_t i = 0; i < enemies->capacity; i++) {
    int rand_row, rand_col;
    do {
      rand_row = rand() % win->total_rows;
      if (rand_row == 0) {
        rand_row = 1;
      }
      rand_col = rand() % win->total_cols;
    } while (map[rand_row][rand_col] == '#');

    enemy_t new_enemy = {
        .active = 1, .current_col = rand_col, .current_row = rand_row, .symbol = '$'};
    enemies->enemies[i] = new_enemy;
    enemies->length++;
  }
}

enemy_arr_t *init_enemies(char **map, window_t *win) {
  enemy_t *enemies = malloc(sizeof(enemy_t) * MAX_ENEMIES);
  enemy_arr_t *enemy_arr = malloc(sizeof(enemy_arr_t));
  enemy_arr->enemies = enemies;
  enemy_arr->capacity = MAX_ENEMIES;
  enemy_arr->length = 0;

  spawn_enemies(enemy_arr, map, win);
  return enemy_arr;
}
