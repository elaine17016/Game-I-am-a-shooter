#include "object.h"
#include "scene.h"

ALLEGRO_FONT *font = NULL;
ALLEGRO_BITMAP *game_background = NULL;
ALLEGRO_BITMAP *game_background_2 = NULL;
ALLEGRO_BITMAP *menu_background = NULL;
ALLEGRO_BITMAP *choose_chara_background=NULL;
ALLEGRO_BITMAP *chara_to_choose[6];
ALLEGRO_BITMAP *win = NULL;
ALLEGRO_BITMAP *lose= NULL;
ALLEGRO_BITMAP * scoreboard=NULL;
//win sound
ALLEGRO_SAMPLE_INSTANCE *win_Sound;
ALLEGRO_SAMPLE *win_sound_sample = NULL;
//lose sound
ALLEGRO_SAMPLE_INSTANCE *lose_Sound;
ALLEGRO_SAMPLE *lose_sound_sample = NULL;
int no_sound=1;//record there is no win/lose sound



// function of menu
void menu_init(){
    font = al_load_ttf_font("./font/normalfont.otf",30,0);
    menu_background=al_load_bitmap("./image/menu_background.png");

}

void menu_process(ALLEGRO_EVENT event){
    if( event.type == ALLEGRO_EVENT_KEY_UP )
        if( event.keyboard.keycode == ALLEGRO_KEY_ENTER )
            judge_next_window = true;
}

void menu_draw(){
    // al_clear_to_color(al_map_rgb(100,100,100));
    al_draw_scaled_bitmap(menu_background,0, 0,al_get_bitmap_width(menu_background),al_get_bitmap_height(menu_background),0, 0,WIDTH,HEIGHT,0);
    //al_draw_text(font, al_map_rgb(255,255,255), WIDTH/2, HEIGHT/2+100 , ALLEGRO_ALIGN_CENTRE, "Press 'Enter' to start");
    //al_draw_rectangle(WIDTH/2-150, 510, WIDTH/2+150, HEIGHT/2, al_map_rgb(255, 255, 255), 0);
    if(no_sound==0)
        no_sound=1;

}

void menu_destroy(){

    al_destroy_bitmap(menu_background);
    al_destroy_font(font);
}

// function of game_scene
void game_scene_init(){
    all_object_init();
    game_background = al_load_bitmap("./image/stage.jpg");
    game_background_2 = al_load_bitmap("./image/stage2.png");
    lose = al_load_bitmap("./image/lose.png");
    win= al_load_bitmap("./image/win.png");
    //win sound init
    win_sound_sample = al_load_sample("./sound/win.wav");
    win_Sound  = al_create_sample_instance(win_sound_sample);
    al_set_sample_instance_playmode( win_Sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(win_Sound, al_get_default_mixer());
    //lose sound init
    lose_sound_sample = al_load_sample("./sound/lose.wav");
    lose_Sound  = al_create_sample_instance(lose_sound_sample);
    al_set_sample_instance_playmode( lose_Sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(lose_Sound, al_get_default_mixer());
    //no win/lose sound now
    no_sound=1;
}
void re_init_game(){
    sc=0;
    hp=5;
    next=0;
    ti_me=0;
    mt=10;
    num_of_background=0;
    num_of_enemy=4;
    count_time=0;
    choose_num=1;
    memset(key_state,false,sizeof(key_state));
    return ;
}
void scoreboard_init(){
    font = al_load_ttf_font("./font/normalfont.otf",30,0);
    scoreboard=al_load_bitmap("./image/images.jpeg");
    return ;
}
void scoreboard_destroy(){
    al_destroy_font(font);
    al_destroy_bitmap(scoreboard);
}

void re_game_process(int bg){

    FILE * fp,*fpw;
    int max_score;
    char date[200];
    char* tmp;
    time_t curtime;
    time(&curtime);
    fp=fopen("./score.txt","r");
    fscanf(fp,"score:%d",&max_score);
    fclose(fp);
    if(max_score<sc){
        max_score=sc;
        tmp=ctime(&curtime);
        fp=fopen("./score.txt","w+");
        fclose(fp);
        fp=fopen("./score.txt","r+");
        fprintf(fp,"score:%d\nname:none\ndate:%s.\n",sc,tmp);
        fclose(fp);
    }
    //fp=fopen("./score.txt","r");
    //fscanf(fp,"date:%s.",date);
    //fclose(fp);
    //printf("%s",date);

    while(1){
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);
        if(bg==1) al_draw_scaled_bitmap(lose,0, 0,al_get_bitmap_width(lose),al_get_bitmap_height(lose),0, 0,WIDTH,HEIGHT,0);
        else if(bg==2) al_draw_scaled_bitmap(win,0, 0,al_get_bitmap_width(win),al_get_bitmap_height(win),0, 0,WIDTH,HEIGHT,0);
        //al_draw_text(font, al_map_rgb(255,0,0), WIDTH/2, HEIGHT/2+100 , ALLEGRO_ALIGN_CENTRE, "Press 'Enter");
        al_flip_display();
        if( event.type == ALLEGRO_EVENT_KEY_UP){
            if( event.keyboard.keycode == ALLEGRO_KEY_ENTER)break ;
        }
    }
    scoreboard_init();
    while(1){ // show your score
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);
        // draw your score
        al_draw_scaled_bitmap(scoreboard,0,0,al_get_bitmap_width(scoreboard),al_get_bitmap_height(scoreboard),0, 0,WIDTH,HEIGHT,0);
        al_draw_text(font, al_map_rgb(0,128,0),140, HEIGHT/2+100 , 0, "Press 'Enter' back to menu");
        al_draw_textf(font,al_map_rgb_f(255,0,0),140, 140,0,"your score: %d",sc);
        al_draw_textf(font,al_map_rgb_f(255,0,0),140, 180,0,"recent highest score: %d",max_score);
        al_draw_textf(font,al_map_rgb_f(255,0,0),140, 220,0,"game time: %d",ti_me);
        al_draw_textf(font,al_map_rgb_f(255,0,0),140, 260,0,"enemy killed: %d",4-num_of_enemy);



        al_flip_display();
        if( event.type == ALLEGRO_EVENT_KEY_UP){
            if( event.keyboard.keycode == ALLEGRO_KEY_ENTER)break;
        }
    }
    scoreboard_destroy();
    re_init_game();
}

void choose_chara_init(){
    char temp[30];
    font = al_load_ttf_font("./font/normalfont.otf",30,0);
    choose_chara_background=al_load_bitmap("./image/choose_chara_background.png");
    for(int i=1;i<=4;i++){
        sprintf(temp,"./image/%dchar_show.png",i);
        chara_to_choose[i]=al_load_bitmap(temp);
    }
    return ;

}
int choose_chara_process(ALLEGRO_EVENT event){
     if( event.type == ALLEGRO_EVENT_KEY_UP ){
        if( event.keyboard.keycode == ALLEGRO_KEY_LEFT && choose_num>1){
            choose_num--;
        }
        else if( event.keyboard.keycode == ALLEGRO_KEY_RIGHT && choose_num<4){

            choose_num++;
        }
        else if(event.keyboard.keycode == ALLEGRO_KEY_C){
            return 1;
        }
     }

    return 0;


}

void choose_chara_draw(){
    int x1[5]={0,51,283,515,739},x2[5]={0,236,468,704,924};
    // draw background
     al_draw_scaled_bitmap(choose_chara_background,0, 0,al_get_bitmap_width(choose_chara_background),al_get_bitmap_height(choose_chara_background),0, 0,WIDTH,HEIGHT,0);
     //draw_bitmap(chara_to_choose[choose_num],WIDTH/2-200,HEIGHT/2-150,300,300,0);
     //al_draw_textf(font,al_map_rgb_f(255,1,1),WIDTH/2, HEIGHT/2+100,0,"choose your character");
        al_draw_rectangle(x1[choose_num], 175, x2[choose_num],  368, al_map_rgb(0, 191, 255), 10);
}

void choose_chara_process_destroy(){
    al_destroy_bitmap(choose_chara_background);
    al_destroy_font(font);
    for(int i=1;i<=4;i++){
        al_destroy_bitmap(chara_to_choose[i]);
    }
    return ;
}


int game_scene_draw(){
    int t=0;
    if(hp<=0)
    {
          t=1;
          al_draw_scaled_bitmap(lose,0, 0,al_get_bitmap_width(lose),al_get_bitmap_height(lose),0, 0,WIDTH,HEIGHT,0);
          if(no_sound)
          {
             al_play_sample_instance(lose_Sound);
             no_sound=0;
          }

    }
    else if(sc>=10)
    {
        t=2;
         al_draw_scaled_bitmap(win,0, 0,al_get_bitmap_width(win),al_get_bitmap_height(win),0, 0,WIDTH,HEIGHT,0);
         if(no_sound)
         {
             al_play_sample_instance(win_Sound);
             no_sound=0;
         }

    }
    else if(sc<5)
    {
        al_draw_scaled_bitmap(game_background,0, 0,al_get_bitmap_width(game_background),al_get_bitmap_height(game_background),0, 0,WIDTH,HEIGHT,0);
        object_draw();

    }
    else
    {
        al_draw_scaled_bitmap(game_background_2,0, 0,al_get_bitmap_width(game_background_2),al_get_bitmap_height(game_background_2),0, 0,WIDTH,HEIGHT,0);
        object_draw();

    }
    return t;
}



void game_scene_destroy(){
    al_destroy_bitmap(game_background);
    al_destroy_bitmap(game_background_2);
    al_destroy_bitmap(win);
    al_destroy_bitmap(lose);
    al_destroy_sample_instance(win_Sound);
    al_destroy_sample_instance(lose_Sound);
    object_destroy();


}
