#include "types.h"
#pragma once

int position_occupied(char **map, int next_row, int next_col);
void update_camera(camera_t *cam, player_t *player, window_t *win);
void move_player(window_t *window, char **map, char input, player_t *player,
                 camera_t *cam);
