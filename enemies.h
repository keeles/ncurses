#include "types.h"

enemy_arr_t *init_enemies(char **map, window_t *win);
int update_enemies(enemy_arr_t *enemies, char **map, camera_t *cam,
                   window_t *win, int cycles);
void spawn_enemies(enemy_arr_t *enemies, char **map, window_t *win);
void increment_enemy_pos(enemy_t *enemy, char **map, camera_t *cam,
                         window_t *win, int cycles);
