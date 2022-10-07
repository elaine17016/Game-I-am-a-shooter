#include "global.h"
#include "object.h"

void menu_init();
void menu_process(ALLEGRO_EVENT event);
void menu_draw();
void menu_destroy();
void re_game_process(int temp);
void game_scene_init();
int game_scene_draw();
void game_scene_destroy();
void scoreboard_init();
int choose_chara_process(ALLEGRO_EVENT event);
void choose_chara_draw();
void choose_chara_init();
void choose_chara_process_destroy();
void scoreboard_destroy();