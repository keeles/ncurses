#include "types.h"
#pragma once

bullet_arr_t *init_bullets();
void update_bullets(bullet_arr_t *bullets, char **map, camera_t *cam,
                    window_t *win);
void shoot_bullet(player_t *player, char **map, bullet_arr_t *bullets);
