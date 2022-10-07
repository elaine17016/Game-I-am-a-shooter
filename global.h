#ifndef GLOBAL_H_INCLUDED
#define GLOBAL_H_INCLUDED
#define GAME_TERMINATE -1
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

// note that you can't assign initial value here!
extern const float FPS;
extern const int WIDTH;
extern  int sc;
extern  int hp;
extern const int HEIGHT;
extern bool key_state[ALLEGRO_KEY_MAX];
extern bool judge_next_window;
extern ALLEGRO_EVENT_QUEUE *event_queue;
extern ALLEGRO_TIMER *fps;
extern int num_of_enemy;
extern int next;
extern int ti_me;
extern int hp_full;
extern int mt;
extern int count_time;
extern int num_of_background; //(num_of_enemy didn't use)
extern int choose_num;
extern bool re_init ;
#endif
