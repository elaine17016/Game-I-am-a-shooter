#include "global.h"

// variables for global usage
const float FPS = 60;
const int WIDTH = 960;
const int HEIGHT = 540;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *fps = NULL;
bool key_state[ALLEGRO_KEY_MAX] = {false};
bool judge_next_window = false;
bool re_init=false;
int sc=0;
int hp=5;
int hp_full=5;
int num_of_enemy=4;
int next=0;
int ti_me=0;
int mt=10;
int count_time=0;
int num_of_background=0; //(num_of_enemy didn't use)
int choose_num=1;





