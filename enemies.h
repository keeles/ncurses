#include "types.h"

cell_t build_cell(cell_t *parent, player_t *player, int x, int y);
enemy_arr_t *init_enemies(char **map, window_t *win);
int update_enemies(enemy_arr_t *enemies, char **map, cell_state_t *cell_state,
                   camera_t *cam, window_t *win, int cycles, player_t *player);
void spawn_enemies(enemy_arr_t *enemies, char **map, window_t *win);
void increment_enemy_pos(enemy_t *enemy, cell_state_t *cell_state, char **map,
                         camera_t *cam, player_t *player, window_t *win);
