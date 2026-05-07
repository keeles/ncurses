#include "types.h"
#pragma once

void smooth_map(char **map, window_t *win);
void draw_map(char **map, camera_t *cam);
char **init_map(window_t *win);
