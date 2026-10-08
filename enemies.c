#include "enemies.h"
#include "types.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

const size_t MAX_ENEMIES = 30;

int heuristic(int x1, int y1, int x2, int y2) { return abs(x1 - x2) + abs(y1 - y2); }

int is_empty(cell_q_t *p) { return p->num_entries == 0; }

cell_t *return_next_pos(cell_t *goal) {
  if (goal->parent == NULL) {
    return goal;
  }

  cell_t *step = goal;
  cell_t *parent = goal->parent;

  while (parent->parent != NULL) {
    step = parent;
    parent = parent->parent;
  }

  return step;
}

cell_t build_cell(cell_t *parent, player_t *player, int x, int y) {
  cell_t cell = {.x = x,
                 .y = y,
                 // start with expensive value
                 .g = 100,
                 .h = heuristic(x, y, player->current_col, player->current_row),
                 .f = 0 + heuristic(x, y, player->current_col, player->current_row),
                 .parent = parent,
                 .in_open = 0,
                 .in_closed = 0};
  return cell;
}

void try_neighbour(cell_state_t *cell_state, cell_t *current, char **map, window_t *win, int nx,
                   int ny) {
  // map[0][n] is for status bar
  if (nx < 0 || nx == win->total_cols || ny < 1 || ny == win->total_rows || map[ny][nx] == '#') {
    return;
  }

  cell_t *n = &cell_state->cells[ny * win->total_cols + nx];
  if (n->in_closed) {
    return;
  }

  // tentative_g = current->g + cost_of_moving(current, neighbor)
  // in my current 4 directional movement, cost_of_moving always is 1
  int tentative_g = current->g + 1;

  if (n->in_open == 0 || tentative_g < n->g) {
    n->g = tentative_g;
    n->f = n->g + n->h;
    n->parent = current;

    if (n->in_open == 0) {
      n->in_open = 1;
      if (cell_state->open_set->num_entries == 0) {
        cell_state->open_set->items[0] = n;
      } else {
        cell_state->open_set->items[cell_state->open_set->num_entries] = n;
      }
      cell_state->open_set->num_entries++;
    }
  }
}

// TODO: enemies seem to find path at first but as they get close they stop moving
cell_t *calculate_path(window_t *win, cell_state_t *cell_state, player_t *player, enemy_t *enemy,
                       char **map) {
  // reset shared open_set
  cell_state->open_set->num_entries = 0;

  for (int i = 0; i < cell_state->dirty_items->num_entries; i++) {
    int x = cell_state->dirty_items->items[i]->x;
    int y = cell_state->dirty_items->items[i]->y;

    if (x == player->current_col && y == player->current_row) {
      cell_state->cells[y * win->total_cols + x] = build_cell(NULL, player, x, y);
    } else {
      cell_state->cells[y * win->total_cols + x] = build_cell(NULL, player, x, y);
    }
  }

  // reset dirty items set
  cell_state->dirty_items->num_entries = 0;

  cell_t *start = &cell_state->cells[enemy->current_row * win->total_cols + enemy->current_col];
  start->g = 0;
  start->f = start->h;
  start->in_open = 1;
  cell_state->open_set->items[0] = start;
  cell_state->open_set->num_entries++;

  // make sure this cell gets reset next search too
  cell_state->dirty_items->items[0] = start;
  cell_state->dirty_items->num_entries++;

  while (!is_empty(cell_state->open_set)) {
    int best_i = 0;

    for (int i = 0; i < cell_state->open_set->num_entries; i++) {
      if (cell_state->open_set->items[i]->f < cell_state->open_set->items[best_i]->f) {
        best_i = i;
      }
    }

    cell_t *current = cell_state->open_set->items[best_i];
    cell_state->open_set->items[best_i] =
        cell_state->open_set->items[cell_state->open_set->num_entries - 1];
    cell_state->open_set->num_entries--;
    current->in_open = 0;
    current->in_closed = 1;

    // current is player
    if (current->y == player->current_row && current->x == player->current_col) {
      cell_t *next_step = return_next_pos(current);
      return next_step;
    }

    // try all neighbours
    try_neighbour(cell_state, current, map, win, current->x + 1, current->y);
    try_neighbour(cell_state, current, map, win, current->x - 1, current->y);
    try_neighbour(cell_state, current, map, win, current->x, current->y + 1);
    try_neighbour(cell_state, current, map, win, current->x, current->y - 1);
  }

  return NULL;
}

int space_occupied(char **map, int next_row, int next_col) {
  if (map[next_row][next_col] == '#' || map[next_row][next_col] == '@') {
    return 1;
  }
  return 0;
}

void increment_enemy_pos(enemy_t *enemy, cell_state_t *cell_state, char **map, camera_t *cam,
                         player_t *player, window_t *win) {
  // 1. calculate shortest path
  cell_t *next_step = calculate_path(win, cell_state, player, enemy, map);

  if (next_step == NULL) {
    // draw in place
    mvaddch(enemy->current_row - cam->cam_row, enemy->current_col - cam->cam_col, enemy->symbol);
    return;
  }

  // 2. find direction from current cell to "first step" of path and increment in this direction
  int next_row = next_step->y;
  int next_col = next_step->x;

  if (next_row <= 1 || next_row >= win->total_rows) {
    next_row = enemy->current_row;
  }

  if (next_col < 0 || next_col >= win->total_cols) {
    next_col = enemy->current_col;
  }

  mvaddch(next_row - cam->cam_row, next_col - cam->cam_col, enemy->symbol);
  enemy->current_row = next_row;
  enemy->current_col = next_col;
}

int update_enemies(enemy_arr_t *enemies, char **map, cell_state_t *cell_state, camera_t *cam,
                   window_t *win, int cycles, player_t *player) {
  if (enemies->length == 0) {
    return 0;
  }

  int remaining_enemies = 0;
  for (size_t i = 0; i < enemies->length; i++) {
    enemy_t *enemy = &enemies->enemies[i];

    if (enemy->active == 0) {
      continue;
    }

    // + 1 because cycles resets to 1 not 0
    if (i % 10 + 1 == cycles) {
      increment_enemy_pos(enemy, cell_state, map, cam, player, win);
    } else {
      mvaddch(enemy->current_row - cam->cam_row, enemy->current_col - cam->cam_col, enemy->symbol);
    }

    remaining_enemies++;
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
