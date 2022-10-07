#include "object.h"
#include "scene.h"
ALLEGRO_SAMPLE_INSTANCE *explode_Sound;
ALLEGRO_SAMPLE *explode = NULL;




typedef struct character
{
    int x, y; // the position of image
    int width, height; // the width and height of image
    bool dir; // left: false, right: true
    int state; // the state of character
    ALLEGRO_BITMAP *img_move[2];
    ALLEGRO_BITMAP *img_atk[2];
     ALLEGRO_BITMAP *img_stop[2];
    ALLEGRO_SAMPLE_INSTANCE *atk_Sound;
    int anime; // counting the time of animation
    int anime_time; // indicate how long the animation
    int active; // when active=1 show in the screen,if active =0 then hide
    int hp;
    int hp_full;
    int mode;
}Character;

typedef struct Bullet
{
    int x, y; // the position of image
    int width, height; // the width and height of image
    bool dir; // left: false, right: true
    int state; // the state of character
    ALLEGRO_BITMAP *img_b[2];
    int active; // when active=1 show in the screen,if active =0 then hide
}bullet;

typedef struct Tool{
    int x,y;
    int width,height;
    bool dir;
    int state;
    ALLEGRO_BITMAP *img_t[2];
    int active;
}tool;

ALLEGRO_FONT *font1 = NULL;



//srand(time(NULL)); // set random seed

// the state of character
enum {STOP = 0, MOVE, ATK};

// define main character and enemies
Character chara={0},ene[5]={0};

// define main character bullets and enemies
bullet bu_m[28]={0},bu_e[11]={0};

// define tools
tool tl[11]={0};

int count2=-1,count1=0;
ALLEGRO_SAMPLE *sample = NULL;


// to determine whether two thing is touched( ps: not yet sure if it is correct!!!!!!!!!!,need to check)
int touched(int a_x,int a_y,int a_width,int a_height,int b_x,int b_y,int b_width,int b_height){
    if ( ( (a_x+a_width) - (b_x-b_width) >=0 && ( (a_x-a_width) - (b_x+b_width) ) <= 0) &&
       ( ( (a_y+a_height) - (b_y-b_height) >=0 && ( (a_y-a_height) - (b_y+b_height) ) <= 0))   )return 1;
    else return 0;
}

// (x,y)=health bar left up corner position,percent=health percentage
void draw_health_bar(float x,float y,int width,int height,float percent){

    al_draw_rectangle(x,y,x+width,y+height,al_map_rgb(0,0,0),2);
    if(percent!=0) al_draw_filled_rectangle(x+1,y+1,x+(width*percent)-1,y+height-1,al_map_rgb(255,0,0));
    return ;

}

void draw_bitmap(ALLEGRO_BITMAP* var,float x,float y,int width,int height,int flags){

     al_draw_scaled_bitmap(var,0, 0,al_get_bitmap_width(var),al_get_bitmap_height(var),x,y,width,height,flags);
     return ;
}

void font_init(){
     // load font
    font1 = al_load_ttf_font("./font/normalfont.otf",40,0);
    return ;
}

void been_shot_sound_init()
{


    explode = al_load_sample("./sound/beenshot.wav");

    explode_Sound  = al_create_sample_instance(explode);
    al_set_sample_instance_playmode(explode_Sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(explode_Sound, al_get_default_mixer());
}

void chara_init(){

    char temp[50];


    for(int i = 1 ; i <=2 ; i++){

         // load character images
        sprintf( temp, "./image/%dchar_move%d.png", choose_num,i );
        chara.img_move[i-1] = al_load_bitmap(temp);

        sprintf( temp, "./image/%dchar_atk%d.png",choose_num, i );
        chara.img_atk[i-1] =  al_load_bitmap(temp);
    }
    sprintf(temp,"./image/%dchar_stop.png",choose_num);
    chara.img_stop[0]=al_load_bitmap(temp);

    // initial the geometric information of character
    chara.width = WIDTH/11;
    chara.height = HEIGHT/11+20;
    // main character central point : (chara.width,chara.height+30)
    chara.x = WIDTH/2-50;
    chara.y = HEIGHT/2-30;

    chara.dir = false;

    // initial the animation component
    chara.state = STOP;
    chara.anime = 0;
    chara.anime_time = 30;

    chara.mode=0;




    return ;
}

void ene_init(){

    char temp[50];


    for(int i=1;i<=4;i++){

    // load enemy images
        sprintf( temp, "./image/ene%d_move.png",i);
        ene[i].img_move[0] = al_load_bitmap(temp);
    // init enemy picture size

        ene[i].width =WIDTH/11;
        ene[i].height = HEIGHT/11+20;
    }

    // init enemy position
    ene[1].x = 0;
    ene[1].y = HEIGHT/2;
    ene[1].active=1;
    ene[1].dir=true; //down
    ene[1].hp=3;
    ene[1].hp_full=3;

    ene[2].x = WIDTH-140;
    ene[2].y = HEIGHT;
    ene[2].active=1;
    ene[2].dir=false; // up
    ene[2].hp=3;
    ene[2].hp_full=3;


    ene[3].x = 60;
    ene[3].y = 0-20;
    ene[3].active=0;
    ene[3].hp=6;
    ene[3].hp_full=6;

    ene[4].x = WIDTH-200;
    ene[4].y = HEIGHT+20;
    ene[4].active=0;
    ene[4].hp=6;
    ene[4].hp_full=6;
    return ;

}

void sound_init(){

    // load effective sound
    sample = al_load_sample("./sound/atk_sound.wav");
    chara.atk_Sound  = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(chara.atk_Sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(chara.atk_Sound, al_get_default_mixer());
    been_shot_sound_init();
    return ;
}
// initialize main character's bullets and enemy bullets
void bullet_init(){

    char temp[50];


    // load chara bullet's picture
    for(int i=1;i<=27;i++){
        sprintf( temp, "./image/%dbu%d.png",choose_num,1);
        bu_m[i].img_b[0] = al_load_bitmap(temp);
        sprintf( temp, "./image/%drotated_bu.png",choose_num);
        bu_m[i].img_b[1]=al_load_bitmap(temp);
        bu_m[i].width = WIDTH/20;
        bu_m[i].height = HEIGHT/20;
        bu_m[i].x = chara.x;
        bu_m[i].y=chara.y+70;
        bu_m[i].dir = chara.dir;
        bu_m[i].active=0;
        bu_m[i].state=0;
    }

    // load enemy bullet's(bomb) picture,there are only one recently
    for(int i=1;i<=3;i++){
        sprintf( temp, "./image/bu%d.png",2);
        bu_e[i].img_b[0] = al_load_bitmap(temp);
        bu_e[i].width = WIDTH/20;
        bu_e[i].height = HEIGHT/20;
        if(i%2==1) bu_e[i].x =WIDTH-1 ;
        else bu_e[i].x = 0+1;
        bu_e[i].y=100*i;
        if(i%2==1) bu_e[i].dir =false ;
        else bu_e[i].dir =true;
        if(i!=1 && i!=5) bu_e[i].active=1;
        else bu_e[i].active=0;
    }

    for(int i=4;i<=5;i++){
        sprintf(temp,"./image/bu3.jpg");
        bu_e[i].img_b[0] = al_load_bitmap(temp);
        bu_e[i].width = WIDTH/40;
        bu_e[i].height = HEIGHT/5;
        if(i%2==1) bu_e[i].x =WIDTH-1 ;
        else bu_e[i].x = 0+1;
        if(i%2==1)bu_e[i].y=rand()%(HEIGHT/2)+30;
        else bu_e[i].y=HEIGHT-30-rand()%(HEIGHT/2);
        if(i%2==1) bu_e[i].dir =false ;
        else bu_e[i].dir =true;
        bu_e[i].active=0;
    }

    return ;

}

void tool_init(){
    char temp[50];

    // load tool's picture

        sprintf( temp, "./image/tool%d.png",1);
        tl[1].img_t[0] = al_load_bitmap(temp);
        sprintf( temp,"./image/tool%d.jpg",2);
        tl[2].img_t[0]=al_load_bitmap(temp);
        sprintf( temp, "./image/tool%d.png",3);
        tl[3].img_t[0] = al_load_bitmap(temp);
        sprintf( temp,"./image/tool%d.png",4);
        tl[4].img_t[0]=al_load_bitmap(temp);

    tl[1].width =  WIDTH/20;
    tl[1].height = HEIGHT/20+20;
    if(chara.dir) tl[1].x =1 ;
    else tl[1].x = WIDTH-1;
    tl[1].y=chara.y+30;
    tl[1].dir = !(chara.dir);
    tl[1].active=1;

    tl[2].width =  WIDTH/40;
    tl[2].height = HEIGHT/15+20;
    if(chara.dir) tl[2].x =chara.x+70;
    else tl[2].x = chara.x+70;
    tl[2].y=chara.y+20;
    tl[2].dir = !(chara.dir);
    tl[2].active=0;

    tl[3].width =  WIDTH/20;
    tl[3].height = HEIGHT/20+20;
    if(chara.dir) tl[3].x =1 ;
    else tl[3].x = WIDTH-1;
    tl[3].y=chara.y+100;
    tl[3].dir = !(chara.dir);
    tl[3].active=1;

    tl[4].width=WIDTH/20;
    tl[4].height=HEIGHT/20+20;
    if(chara.dir) tl[4].x =1 ;
    else tl[4].x = WIDTH-1;
    tl[4].y=rand()%(HEIGHT-60)+30;
    tl[4].dir = !(chara.dir);
    tl[4].active=1;


    return;
}


void all_object_init(){
    tool_init();
    font_init();
    chara_init();
    ene_init();
    sound_init();
    bullet_init();
    return ;
}

void object_process(ALLEGRO_EVENT event){
    // process the animation
    if( event.type == ALLEGRO_EVENT_TIMER ){
        if( event.timer.source == fps ){
            chara.anime++;
            chara.anime %= chara.anime_time;
        }
    // process the keyboard event
    }else if( event.type == ALLEGRO_EVENT_KEY_DOWN ){
        key_state[event.keyboard.keycode] = true;
        chara.anime = 0;
    }else if( event.type == ALLEGRO_EVENT_KEY_UP ){
        key_state[event.keyboard.keycode] = false;
    }
    return ;
}

void shot_ene(){//determine whether the enemy has been shot
    for(int i=1;i<=5;i++){
        for(int j=1;j<=27;j++){
            if(abs( bu_m[j].x-bu_e[i].x)<20 && abs(bu_m[j].y-bu_e[i].y)<20 && bu_m[j].active==1 && bu_e[i].active==1){
                    bu_m[j].active=0;
                    if(chara.dir) bu_e[i].x =WIDTH-1;
                    else bu_e[i].x = 0+1;
                    bu_e[i].y=chara.y;
                    bu_e[i].dir = chara.dir;
            }
        }
    }

    for(int i=1;i<=4;i++){ // four enemies
        for(int j=1;j<=27;j++){ // nine main character's bullet

            if(abs( bu_m[j].x-ene[i].x)<20 && abs(bu_m[j].y-ene[i].y)<25 && bu_m[j].active==1 && ene[i].active==1)
            {
                if(count1-ti_me>0){
                    if(ene[i].hp>1)ene[i].hp-=2;
                    else ene[i].hp--;
                    sc+=2;
                }
                else{
                    ene[i].hp--;
                    sc++;
                }

                if(ene[i].hp==0){
                    ene[i].active=0;
                    num_of_enemy--; // enemy hide
                }
                bu_m[j].active=0; // bullet hide

                // bullet back to chara
                bu_m[j].x=chara.x;
                bu_m[j].y=chara.y+30;
                bu_m[j].dir = chara.dir;

                //return;
            }
        }

    }
    return;
}

void been_shot() //determine whether the main character has been shot
{
    for(int i=1;i<=3;i++){ // 1 enemy bullet

        if(abs(bu_e[i].x-chara.x)<30&&abs(bu_e[i].y-15-chara.y)<30&& bu_e[i].active==1)
        {
            bu_e[i].active=1; // bullet hide
            if(chara.dir) bu_e[i].x =WIDTH-1;
            else bu_e[i].x = 0+1;
            bu_e[i].y=chara.y;
            bu_e[i].dir = chara.dir;
            hp--; //health point minus 1
            //play sound
            al_play_sample_instance(explode_Sound);
            return;
        }
    }
    for(int i=4;i<=5;i++){
         if(abs(bu_e[i].x-chara.x)<50&&abs(bu_e[i].y+15-chara.y)<80&& bu_e[i].active==1)
         {
            bu_e[i].active=1; // bullet hide
            if(chara.dir) bu_e[i].x =WIDTH-1;
            else bu_e[i].x = 0+1;
            if(i%2==1)bu_e[i].y=HEIGHT/2+40-rand()%(HEIGHT/2);
            else bu_e[i].y=HEIGHT-30-rand()%(HEIGHT/2);
            bu_e[i].dir = chara.dir;
            hp--; //health point minus 1
            //play sound
            al_play_sample_instance(explode_Sound);
            return;
         }

    }
    return ;
}

void been_cure(){  //determine whether the main character has been cure
    for(int i=1;i<=4;i++){ // 1 enemy bullet
        if(i==2) continue ;
        if(abs(tl[i].x-chara.x)<50&&abs(tl[i].y-chara.y)<70&& tl[i].active==1)
        {
            tl[i].active=1; // bullet hide
            if(chara.dir) tl[i].x =1;
            else tl[i].x = WIDTH-1;
            tl[i].y=(rand()%(HEIGHT)-30);
            tl[i].dir = chara.dir;
            if(i==1){
                if(hp<hp_full) hp++; //health point minus 1
                else tl[2].active=1;
            }
            else if(i==4){
                chara.mode=1;
                count2=ti_me+20;
            }
            else if(i==3){
                count1=ti_me+20;
            }
            return;
        }
    }
    return ;

}

void bounce(){
    for(int i=1;i<=5;i++){
        if(abs(tl[2].x-bu_e[i].x)<50&&abs(tl[2].y-bu_e[i].y)<70&& tl[2].active==1)
        {
            tl[2].active=0;
            bu_e[i].dir=!(bu_e[i].dir);
        }
    }

}

void bullet_active(){ // show new bullet

    int i=1;
    if(count2<ti_me){
            count2=-1;
            chara.mode=0;
    }
    if(chara.mode==0){
        while(bu_m[i].active==1 && i<9){ // iterate to the available bullet (bullet that is hidden and not flying)
             i++;
        }
        bu_m[i].active=1; // show this bullet,let it start to fly
    }
    else if(chara.mode ==1){
            for(int j=1;j<=3;j++){
                i=10;
                while(bu_m[i].active==1 && i<27){ // iterate to the available bullet (bullet that is hidden and not flying)
                    i++;
                }
                bu_m[i].active=1;
                bu_m[i].state=j-1;
            }
    }

    return ;
}

void ene_bullet_active(int next){
    if(next==1){
        for(int i=1;i<=4;i++){
            ene[i].active=1; // when scene upgrade let the hidden enemy be shown,and start to move
            ene[i].hp=ene[i].hp_full;
        }
        for(int i=1;i<=5;i++){
            bu_e[i].active=1;
        }
    }
    return ;
}

void object_moving(){
    // char built that is not acitve goes with chara
    for(int i=1;i<=27;i++){
        if(bu_m[i].active==0){
            bu_m[i].x=chara.x;
            bu_m[i].y=chara.y+20;
            bu_m[i].dir=chara.dir;
        }
    }                                                                                           //remain y moving problem

     // chara bullets that is active will keep flying
    for(int i=1;i<=27;i++){

        if(bu_m[i].x>=30&&bu_m[i].x<=WIDTH && bu_m[i].active==1){
            if(bu_m[i].dir){
                bu_m[i].x+=20;
                if(bu_m[i].state==1) bu_m[i].y+=10;
                else if(bu_m[i].state==2) bu_m[i].y-=10;
            }
            else {
                bu_m[i].x-=20;
                if(bu_m[i].state==1) bu_m[i].y+=10;
                 else if(bu_m[i].state==2) bu_m[i].y-=10;
            }


        }
        else if( (bu_m[i].x<30 || bu_m[i].x>WIDTH) && bu_m[i].active==1){
            bu_m[i].state=0;
            bu_m[i].active=0; // bullet out of screen ,so hide
            bu_m[i].x=chara.x; // bullet back to chara
            bu_m[i].y=chara.y;
            bu_m[i].dir = chara.dir;
        }
    }

    // enemy bullets keep flying
    for(int i=1;i<=3;i++){
        if(bu_e[i].x>0&&bu_e[i].x<(WIDTH)&& bu_e[i].active==1){

            if(bu_e[i].dir) bu_e[i].x+=3;
            else bu_e[i].x-=3;
        }
        else if((bu_e[i].x<=0 || bu_e[i].x>=(WIDTH)) && bu_e[i].active==1){
            if(bu_e[i].dir){
                bu_e[i].dir=false;bu_e[i].x=WIDTH-1;
                if(i>=2 && i<=4)bu_e[i].y=rand()%(200)+((i-1)*100);
                else bu_e[i].y=i*100;
            }
            else {
                bu_e[i].dir=true;bu_e[i].x=1;
                if(i>=2 && i<=4) bu_e[i].y=rand()%(200)+((i-1)*100);
                else bu_e[i].y=i*100;
            }
        }

    }
    for(int i=4;i<=5;i++){
        if(bu_e[i].x>0&&bu_e[i].x<(WIDTH)&& bu_e[i].active==1){

            if(bu_e[i].dir) bu_e[i].x+=3;
            else bu_e[i].x-=3;
        }
        else if((bu_e[i].x<=0 || bu_e[i].x>=(WIDTH)) && bu_e[i].active==1){
            if(bu_e[i].dir){
                bu_e[i].dir=false;bu_e[i].x=WIDTH-1;
            }
            else{
                 bu_e[i].dir=true;bu_e[i].x=1;
            }
            if(i%2==1)bu_e[i].y=rand()%(HEIGHT/2)+30;
            else bu_e[i].y=HEIGHT-30-rand()%(HEIGHT/2);
        }

    }

    // tool keep flying
    for(int i=1;i<=3;i+=2){
        if(tl[i].x>0&&tl[i].x<(WIDTH)&& tl[i].active==1){

            if(tl[i].dir) tl[i].x+=3;
            else tl[i].x-=3;
        }
        else if((tl[i].x<=0 || tl[i].x>=(WIDTH)) && tl[i].active==1){
            if(tl[i].dir){tl[i].dir=false;tl[i].x=WIDTH-1;}
            else {tl[i].dir=true;tl[i].x=1;}
        }

    }

    for(int i=2;i<=2;i++){
        if(tl[i].y>=0 &&tl[i].y<=(HEIGHT)&& tl[i].active==1){
            if(chara.dir){
                tl[i].y=chara.y+10;
                tl[i].x=chara.x+70;

            }
            else{
                tl[i].y=chara.y+10;
                tl[i].x=chara.x-30;


            }
        }
    }
    for(int i=4;i<=4;i++){
        if(tl[i].x>0&&tl[i].x<(WIDTH)&& tl[i].active==1){

            if(tl[i].dir) tl[i].x+=3;
            else tl[i].x-=3;
        }
        else if((tl[i].x<=0 || tl[i].x>=(WIDTH)) && tl[i].active==1){
            if(tl[i].dir){tl[i].dir=false;tl[i].x=WIDTH-1;}
            else {tl[i].dir=true;tl[i].x=1;}
        }

    }


    // active enemy go back and forth
    for(int i=1;i<=4;i++){
        if(ene[i].y>0 &&ene[i].y<(HEIGHT)&& ene[i].active==1){

            if(ene[i].dir) ene[i].y+=20/4;
            else ene[i].y-=20/4;
        }
         else if((ene[i].y<=0 || ene[i].y>=(HEIGHT)) && ene[i].active==1){
            if(ene[i].dir){ene[i].dir=false;ene[i].y=HEIGHT-1;}
            else {ene[i].dir=true;ene[i].y=1;}
        }
    }
    return ;

}

void interpreting_keys(){

    if( key_state[ALLEGRO_KEY_W] ){
        if(chara.y>-30+chara.height/2+10)
        chara.y -= 5;

         chara.state = MOVE;
    }
    else if( key_state[ALLEGRO_KEY_S] ){
        if(chara.y<(HEIGHT-30-chara.height/2))
        chara.y += 5;

         chara.state = MOVE;
    }
    if( key_state[ALLEGRO_KEY_A] ){
        chara.dir = false;
        //chara.x -= 5;
        chara.state = STOP;
    }
    else if( key_state[ALLEGRO_KEY_D] ){
        chara.dir = true;
        //chara.x += 5;
        chara.state = STOP;
    }

    if( key_state[ALLEGRO_KEY_SPACE] ){
        chara.state = ATK;
        bullet_active(); // if space is pressed ,then active a new bullet

        /* if space is pressed ,play a sound (dont put this in character_draw
        or else the sound cannot be match to shooting bullets)*/
        al_play_sample_instance(chara.atk_Sound);

        key_state[ALLEGRO_KEY_SPACE]=false; // if you dont add this,then when you long-press key space ,it will be continous shooting
    }




    // bullet that is hidden ,will goes with character
    return ;
}

void object_update()
{
    // use the idea of finite state machine to deal with different state
    if(num_of_background==1)
    {
        num_of_background=0;
    }

    // see if charater been shot
    been_shot();

    //see if enemy been shot
    shot_ene();

    //see if charater been cure
    been_cure();

    bounce();

     // upgrade scene and difficulty
    if((sc%5)==0&&sc>0){
            if(hp<3)
            hp=3;
    }
    //when scene==5 will change background and let hp back to full

    if(sc>5&&next==0){next++;ene_bullet_active(next);}//sc is the score，sc>5 will have two more enemies

    // update the movement of things (except the main character)
    object_moving();

    // interpreting what we should do if a keys is press
    interpreting_keys();

    // about main charater's anime settings
    if( chara.anime == chara.anime_time-1 ){
        chara.anime = 0;
        chara.state = STOP;
    }
    if ( chara.anime == 0 ){
        chara.state = STOP;
    }
    return;

}

void object_draw(){                                                           //add tool

    // count time for scoring
    count_time++;
    ti_me=count_time/10;

    if(mt>0) mt-=1;

    al_draw_textf(font1,al_map_rgb_f(1,1,1),1, 1,0,"score: %d",sc);
    al_draw_textf(font1,al_map_rgb_f(1,1,1),750, 1,0,"hp: %d",hp);
    al_draw_textf(font1,al_map_rgb_f(1,1,1),1, 475,0,"time: %d",ti_me);
    if(count1-ti_me>=0) al_draw_textf(font1,al_map_rgb_f(1,1,1),650, 475,0,"double hurt:%d",count1-ti_me);//計算道具剩餘時間
    if(count2-ti_me>=0) al_draw_textf(font1,al_map_rgb_f(1,1,1),650, 425,0,"triple bullet:%d",count2-ti_me);//計算道具剩餘時間
    //void al_draw_textf(const ALLEGRO_FONT *font, ALLEGRO_COLOR color,float x, float y, int flags,const char *format, ...)

    // draw enemy if active
    for(int i=1;i<=4;i++){
        //printf("%d",ene[2].active);
        if(ene[i].active==1){
            draw_health_bar(ene[i].x+20,ene[i].y-30,40,10,float(ene[i].hp)/ene[i].hp_full);
            draw_bitmap(ene[i].img_move[0], ene[i].x, ene[i].y,ene[i].width,ene[i].height, 0);
        }
    }

    // draw main character and enmy bullets if active
    for(int i=1;i<=27;i++){
        if(bu_m[i].active==1){
            if(bu_m[i].dir) {
                if(bu_m[i].state==0) draw_bitmap(bu_m[i].img_b[0], bu_m[i].x, bu_m[i].y,bu_m[i].width,bu_m[i].height,0);
                else if(bu_m[i].state==1) draw_bitmap(bu_m[i].img_b[1], bu_m[i].x, bu_m[i].y,bu_m[i].width,bu_m[i].height,ALLEGRO_FLIP_VERTICAL);
                else if(bu_m[i].state==2) draw_bitmap(bu_m[i].img_b[1], bu_m[i].x, bu_m[i].y,bu_m[i].width,bu_m[i].height,0);
            }


            else {
                if(bu_m[i].state==0) draw_bitmap(bu_m[i].img_b[0], bu_m[i].x, bu_m[i].y,bu_m[i].width,bu_m[i].height, ALLEGRO_FLIP_HORIZONTAL);
                else if(bu_m[i].state==1)  draw_bitmap(bu_m[i].img_b[1], bu_m[i].x, bu_m[i].y,bu_m[i].width,bu_m[i].height,ALLEGRO_FLIP_HORIZONTAL+ALLEGRO_FLIP_VERTICAL);
                else if(bu_m[i].state==2)  draw_bitmap(bu_m[i].img_b[1], bu_m[i].x, bu_m[i].y,bu_m[i].width,bu_m[i].height,ALLEGRO_FLIP_HORIZONTAL);
            }




        }
    }
    for(int i=1;i<=5;i++){
        if(bu_e[i].active==1){
            if(bu_e[i].dir) draw_bitmap(bu_e[i].img_b[0], bu_e[i].x, bu_e[i].y,bu_e[i].width,bu_e[i].height, 0);

            else draw_bitmap(bu_e[i].img_b[0], bu_e[i].x, bu_e[i].y,bu_e[i].width,bu_e[i].height, ALLEGRO_FLIP_HORIZONTAL);
        }
    }
    for(int i=1;i<=4;i++){
        if(tl[i].active==1){
            if(tl[i].dir) draw_bitmap(tl[i].img_t[0], tl[i].x, tl[i].y,tl[i].width,tl[i].height, 0);

            else draw_bitmap(tl[i].img_t[0], tl[i].x, tl[i].y,tl[i].width,tl[i].height, ALLEGRO_FLIP_HORIZONTAL);
        }
    }


    draw_health_bar(chara.x+20,chara.y-30,40,10,float(hp)/hp_full);
    // with the state, draw corresponding image
    if( chara.state == STOP ){
        if( chara.dir )
            draw_bitmap(chara.img_stop[0],chara.x, chara.y,chara.width,chara.height,0);
        else
            draw_bitmap(chara.img_stop[0],chara.x, chara.y,chara.width,chara.height,ALLEGRO_FLIP_HORIZONTAL);
    }else if( chara.state == MOVE ){
        if( chara.dir ){
            if( chara.anime < chara.anime_time/2 ){
                draw_bitmap(chara.img_move[0],chara.x, chara.y,chara.width,chara.height,0);
            }else{
               draw_bitmap(chara.img_move[1],chara.x, chara.y,chara.width,chara.height,0);
            }
        }else{
            if( chara.anime < chara.anime_time/2 ){
                draw_bitmap(chara.img_move[0],chara.x, chara.y,chara.width,chara.height,ALLEGRO_FLIP_HORIZONTAL);
            }else{
                 draw_bitmap(chara.img_move[1],chara.x, chara.y,chara.width,chara.height,ALLEGRO_FLIP_HORIZONTAL);
            }
        }
    }else if( chara.state == ATK ){
        if( chara.dir ){
            if( chara.anime < chara.anime_time/2 ){
                draw_bitmap(chara.img_atk[0],chara.x, chara.y,chara.width,chara.height,0);
            }else{
               draw_bitmap(chara.img_atk[0],chara.x, chara.y,chara.width,chara.height,0);
            }
        }else{
            if( chara.anime < chara.anime_time/2 ){
                draw_bitmap(chara.img_atk[0],chara.x, chara.y,chara.width,chara.height,ALLEGRO_FLIP_HORIZONTAL);
            }else{
                 draw_bitmap(chara.img_atk[0],chara.x, chara.y,chara.width,chara.height,ALLEGRO_FLIP_HORIZONTAL);

            }
        }

    }
}

void object_destroy(){ // destroy created objects

    for(int i=0;i<=1;i++){
        al_destroy_bitmap(chara.img_atk[i]);
        al_destroy_bitmap(chara.img_move[i]);
    }
    for(int i=1;i<=4;i++) al_destroy_bitmap(ene[i].img_move[0]);

    for(int i=1;i<=27;i++) al_destroy_bitmap(bu_m[i].img_b[0]);

    for(int i=1;i<=5;i++) al_destroy_bitmap(bu_e[i].img_b[0]);

    for(int i=1;i<=4;i++) al_destroy_bitmap(tl[i].img_t[0]);

    al_destroy_sample_instance(chara.atk_Sound);


    al_destroy_sample_instance(explode_Sound);

    return ;

}
