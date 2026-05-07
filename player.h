#include "types.h"
#include <ncurses.h>
#pragma once

void update_camera(camera_t *cam, player_t *player, window_t *win);
void move_player(window_t *window, char **map, char input, player_t *player);
