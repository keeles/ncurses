#include "types.h"
#include <time.h>
#pragma once

int count_neighbour_walls(char **map, window_t *win, int row, int col);
void smooth_map(char **map, window_t *win);
void draw_map(char **map, camera_t *cam);
char **init_map(window_t *win);
void game_win(camera_t *cam);
void game_lose(camera_t *cam);
int print_status_line(int viewport_cols, int remaining_enemies,
                      time_t end_time);
