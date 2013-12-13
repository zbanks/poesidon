#include <menus.h>
#include <hal.h>
#include <images.h>

uint8_t RAINBOW[7]={0xE0,0xF4,0xFC,0x1C,0x1F,0x4B,0xE3};

#define MENU_SPLASH_TIMEOUT 300

#define RAINBOW_PERIOD 100

//FIXME:
#define LASER_BG_IMAGE SPLASH_IMAGE
#define LASER_BG_DATA SPLASH_DATA
#define WOW_BG_IMAGE SPLASH_IMAGE
#define WOW_BG_DATA SPLASH_DATA
#define LENGTH_BG_IMAGE SPLASH_IMAGE
#define LENGTH_BG_DATA SPLASH_DATA

// The main settings menu
void menu_main_init();
void menu_main_run();
void menu_main_redraw(uint8_t);

// The laser settings menu
void menu_laser_init();
void menu_laser_run();

// The length/depth settings menu
void menu_length_init();
void menu_length_run();

// The speed settings menu
void menu_speed_init();
void menu_speed_run();

// The splash screen
void menu_splash_init();
void menu_splash_run();

// The konami code
void menu_konami_init();
void menu_konami_run();

// Run
void menu_wow_init();
void menu_wow_run();

int splash_timeout;

typedef struct {
  void (* const init)();
  void (* const run)();
} state_t;

const state_t menu_main = {&menu_main_init,&menu_main_run};
const state_t menu_laser = {&menu_laser_init,&menu_laser_run};
const state_t menu_length = {&menu_length_init,&menu_length_run};
const state_t menu_speed = {&menu_speed_init,&menu_speed_run};
const state_t menu_splash = {&menu_splash_init,&menu_splash_run};
const state_t menu_konami = {&menu_konami_init,&menu_konami_run};
const state_t menu_wow = {&menu_wow_init,&menu_wow_run};

state_t const* state;

btn_t buttons,buttons_edge;

typedef struct {
  uint8_t x,y,dx,dy;
  uint8_t* img;
  uint8_t color;
} sprite_t;

enum sprite_names {
    TXT_BRIGHT_LASER_SPRITE,
    TXT_MUCH_LENGTH_SPRITE,
    TXT_SUCH_SPEED_SPRITE,
    TXT_WOW_IMAGE_SPRITE,
    CIRCLE_SPRITE,
    LINE_SPRITE,
    DOT_SPRITE,
    TXT_SO_SHALLOW_SPRITE,
    TXT_MUCH_DEEP_SPRITE,
    TXT_HOW_MANY_FEAT_LONG_SPRITE
};

const sprite_t sprites[] = {
  {110, 10, TXT_BRIGHT_LASER_IMAGE, COLOR_RED},
  {90, 48, TXT_MUCH_LENGTH_IMAGE, COLOR_GREEN},
  {25, 20, TXT_SUCH_SPEED_IMAGE, COLOR_BLUE},
  {10, 90, TXT_WOW_IMAGE, COLOR_ORANGE},
  {80, 20, CIRCLE_IMAGE, COLOR_GREEN},
  {50, 70, DOT_IMAGE, COLOR_GREEN},
  {10, 25, LINE_IMAGE, COLOR_GREEN},
  {120, 10, TXT_SO_SHALLOW_IMAGE, COLOR_ORANGE},
  {95, 20, TXT_MUCH_DEEP_IMAGE, COLOR_ORANGE},
  {70, 17, TXT_HOW_MANY_FEAT_LONG_IMAGE, COLOR_ORANGE},
};

inline void draw_sprite(enum sprite_names s, const uint8_t* bg);
inline void draw_sprite_color(enum sprite_names s, const uint8_t* bg, uint8_t color);

const uint8_t konami_code[] = {BUTTON_UP, BUTTON_UP, BUTTON_DOWN, BUTTON_DOWN, BUTTON_LEFT, BUTTON_RIGHT, BUTTON_LEFT, BUTTON_RIGHT, BUTTON_B, BUTTON_A};

shape_t setting_laser_shape = CIRCLE;
int setting_depth = 0;
int setting_length = 100;
int setting_speed = 60;

int wow_start_time;

// Begin Functions

void init_menu()
{
  state=&menu_splash;
  state->init();
}

void render_menu()
{
    // The main function to render the current menu 
    state_t const* old_state;

    static uint8_t konami_counter = 0;
    static btn_t last_buttons = 0x00;
    buttons = read_buttons_debounced();
    buttons_edge = buttons & ~last_buttons;
    last_buttons = buttons;

    if(buttons_edge){
        if(buttons_edge == konami_code[konami_counter]){
            konami_counter++;
        }else{
            konami_counter = 0;
        }
        if(konami_counter == 10){
            //TODO
            konami_counter = 0;
        }
    }

  old_state=state;
  state->run();
  if(state != old_state)
  {
    state->init();
  }
}


void menu_main_init()
{
  lcd_blit_mem(0, 0, DOGE_WATER_IMAGE);
  int i;
  for(i=0;i<4;i++)
  {
    menu_main_redraw(i);
  }
}

void menu_main_redraw(uint8_t i){
    draw_sprite(i, DOGE_WATER_DATA);
}

inline void draw_sprite(enum sprite_names s, const uint8_t* bg)
{
  lcd_blit_sprite(sprites[s].x,sprites[s].y,sprites[s].dx,sprites[s].dy,sprites[s].img,(uint8_t *)bg,sprites[s].color);
}

inline void draw_sprite_color(enum sprite_names s, const uint8_t* bg, uint8_t color)
{
  lcd_blit_sprite(sprites[s].x,sprites[s].y,sprites[s].dx,sprites[s].dy,sprites[s].img, (uint8_t *)bg,color);
}

void menu_main_run()
{
    // The main settings menu
    static uint8_t selection = 0;
   
    // Much divison
    // Very sad
    int rainbow_ctr = (time/RAINBOW_PERIOD) % sizeof(RAINBOW);

    if(buttons_edge & BUTTON_DOWN){
        if(selection < 3) {
            menu_main_redraw(selection);
            selection++;
        }
    }

    if(buttons_edge & BUTTON_UP){
        if(selection > 0){
            menu_main_redraw(selection);
            selection--;
        }
    }

    // Draw selection in rainbow
    draw_sprite_color(selection, DOGE_WATER_DATA, RAINBOW[rainbow_ctr]);
    if(buttons_edge & BUTTON_A){
        switch(selection){
            case 0:
                state=&menu_laser;
            break;
            case 1:
                state = &menu_length;
            break;
            case 2:
                state = &menu_speed;
            break;
            case 3:
                state = &menu_wow;
            break;
        }
    }
}

void menu_laser_init(){
    lcd_blit_mem(0, 0, LASER_BG_IMAGE);
    draw_sprite(DOT_SPRITE, LASER_BG_DATA);
    draw_sprite(CIRCLE_SPRITE, LASER_BG_DATA);
    draw_sprite(LINE_SPRITE, LASER_BG_DATA);
}

void menu_laser_run(){
    uint8_t rainbow_color = RAINBOW[(time/RAINBOW_PERIOD) % sizeof(RAINBOW)];

    draw_sprite_color(CIRCLE_SPRITE + setting_laser_shape, LASER_BG_DATA, rainbow_color);

    if(buttons_edge & (BUTTON_DOWN | BUTTON_RIGHT)){
        if(setting_laser_shape < LAST_SHAPE){
            draw_sprite(CIRCLE_SPRITE + setting_laser_shape, LASER_BG_DATA);
            setting_laser_shape++;
        }
    }
    if(buttons_edge & (BUTTON_UP | BUTTON_LEFT)){
        if(setting_laser_shape > 0){
            draw_sprite(CIRCLE_SPRITE + setting_laser_shape, LASER_BG_DATA);
            setting_laser_shape--;
        }
    }

    if(buttons_edge & (BUTTON_A | BUTTON_B)){
        state = &menu_main;
        return;
    }
}


void menu_length_init(){
    lcd_blit_mem(0, 0, LENGTH_BG_IMAGE);
    draw_sprite(TXT_SO_SHALLOW_SPRITE, LENGTH_BG_DATA);
    draw_sprite(TXT_MUCH_DEEP_SPRITE, LENGTH_BG_DATA);
    // TODO something for length
    draw_sprite(TXT_HOW_MANY_FEAT_LONG_SPRITE, LENGTH_BG_DATA);

}

void menu_length_run(){
    static uint8_t length_state = 0;
    uint8_t rainbow_color = RAINBOW[(time/RAINBOW_PERIOD) % sizeof(RAINBOW)];

    if(length_state == 0){
        if(setting_depth == 0){
            if(buttons_edge & (BUTTON_DOWN)){
                draw_sprite(TXT_SO_SHALLOW_SPRITE, LENGTH_BG_DATA);
                setting_depth = 1;
            }else{
                draw_sprite_color(TXT_SO_SHALLOW_SPRITE, LENGTH_BG_DATA, rainbow_color);
            }
        }else{
            if(buttons_edge & (BUTTON_UP)){
                draw_sprite(TXT_MUCH_DEEP_SPRITE, LENGTH_BG_DATA);
                setting_depth = 0;
            }else{
                draw_sprite_color(TXT_MUCH_DEEP_SPRITE, LENGTH_BG_DATA, rainbow_color);
            }
        }
    }else if(length_state == 1){
        // Hundreds Digit
        if(buttons_edge & BUTTON_DOWN){
            if(setting_length >= 100){
                setting_length -= 100;
            }
        }
        if(buttons_edge & BUTTON_UP){
            if(setting_length < 900){
                setting_length += 100;
            }
        }
        // TODO: Rainbow digit
    }else if(length_state == 2){
        // Tens Digit
        if(buttons_edge & BUTTON_DOWN){
            if(setting_length % 100 >= 10){
                setting_length -= 10;
            }
        }
        if(buttons_edge & BUTTON_UP){
            if(setting_length % 100 < 90){
                setting_length += 10;
            }
        }
        // TODO: Rainbow digit
    }else if(length_state == 3){
        // Tens Digit
        if(buttons_edge & BUTTON_DOWN){
            if(setting_length % 10 >= 1){
                setting_length -= 1;
            }
        }
        if(buttons_edge & BUTTON_UP){
            if(setting_length % 10 < 900){
                setting_length += 1;
            }
        }
        // TODO: Rainbow digit
    }

    if(buttons_edge & (BUTTON_A | BUTTON_LEFT)){
        length_state = (length_state + 1) % 4;
    }
    if((buttons_edge & BUTTON_RIGHT) && length_state){
        length_state--;
    }
    if(buttons_edge & BUTTON_B){
        length_state = 0;
        state = &menu_main;
    }
}

void menu_speed_init(){

}

void menu_speed_run(){
    //TODO
    state = &menu_main;
}

void menu_splash_init()
{
  splash_timeout=2000;
  lcd_blit_mem(0, 0, SPLASH_IMAGE);
}

void menu_splash_run()
{
  int rainbow_ctr = (time/RAINBOW_PERIOD) % sizeof(RAINBOW);
  lcd_blit_sprite(7, 23, SPLASH_TXT_IMAGE, (uint8_t*) SPLASH_DATA, RAINBOW[rainbow_ctr]);

  if((!splash_timeout--) || (buttons & BUTTON_A))
  {
      state=&menu_main;
  }
}

void menu_konami_init(){

}

void menu_konami_run(){
    //TODO
    state = &menu_main;
}

void menu_wow_init()
{
  lcd_blit_mem(0, 0, WOW_BG_IMAGE);
  wow_start_time=time;
  set_laser(1);
}

void menu_wow_deinit()
{
  set_laser(0);
}

void menu_wow_run()
{
  static int wow_last_time=-1;
  int wow_time;
  
  wow_time=(time-wow_start_time)/1000;
  if(wow_time != wow_last_time)
  {
    blit_number(10,10,wow_time,WOW_BG_DATA,0);
  }

  project(setting_laser_shape,setting_depth,setting_length,setting_speed,time-wow_start_time);
  
  if(buttons & BUTTON_B)
  {
    state=&menu_main;
  }
}
