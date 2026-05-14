#include "types.h"
#pragma once

void increment_bullet_pos(bullet_t *bullet, char **map, camera_t *cam,
                          window_t *win, enemy_arr_t *enemies);
bullet_arr_t *init_bullets();
void update_bullets(bullet_arr_t *bullets, char **map, camera_t *cam,
                    window_t *win, enemy_arr_t *enemies);
void shoot_bullet(player_t *player, char **map, bullet_arr_t *bullets,
                  window_t *win);
