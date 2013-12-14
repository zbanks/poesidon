
#include <menus.h>
#include <hal.h>
#include <images.h>
#include "project.h"
#include "numbers.h"

uint8_t RAINBOW[7]={0xE0,0xF4,0xFC,0x1C,0x1F,0x4B,0xE3};

#define MENU_SPLASH_TIMEOUT 300

#define RAINBOW_PERIOD 100

//FIXME:
#define LASER_BG_IMAGE TPAYNE_IMAGE
#define LASER_BG_DATA TPAYNE_DATA
#define LENGTH_BG_IMAGE POESIDON_DOGE_1_IMAGE
#define LENGTH_BG_DATA POESIDON_DOGE_1_DATA

#define KONAMI_SEQUENCE_STEP 512*34
#define KONAMI_SEQUENCE_LENGTH 3

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
void menu_konami_redraw();

// Run
void menu_wow_init();
void menu_wow_run();
void menu_wow_deinit();

uint8_t rand();

int splash_timeout;
int konami_index;

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
    SQUARE_SPRITE,
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
  {80, 8, CIRCLE_IMAGE, COLOR_GREEN},
  {62, 86, SQUARE_IMAGE, COLOR_GREEN},
  {35, 21, DOT_IMAGE, COLOR_GREEN},
  {5, 25, LINE_IMAGE, COLOR_GREEN},
  {108, 10, TXT_SO_SHALLOW_IMAGE, COLOR_BLACK},
  {83, 27, TXT_MUCH_DEEP_IMAGE, COLOR_BLACK},
  {65, 6, TXT_HOW_MANY_FEAT_LONG_IMAGE, COLOR_BLUE},
};

inline void draw_sprite(enum sprite_names s, const uint8_t* bg);
inline void draw_sprite_color(enum sprite_names s, const uint8_t* bg, uint8_t color);

const uint8_t konami_code[] = {BUTTON_UP, BUTTON_UP, BUTTON_DOWN, BUTTON_DOWN, BUTTON_LEFT, BUTTON_RIGHT, BUTTON_LEFT, BUTTON_RIGHT, BUTTON_B, BUTTON_A};

shape_t setting_laser_shape = LINE;
int setting_depth = 100;
int setting_length = 450;
int setting_speed = 200;

int wow_start_time;
int wow_last_time;

// Begin Functions

void init_menu(){
  state = &menu_splash;
  state->init();
}


void render_menu()
{
  // The main function to render the current menu 
  static state_t const* old_state;
  static uint8_t konami_counter = 0;
  static btn_t last_buttons = 0x00;
  buttons = read_buttons_debounced();
  buttons_edge = buttons & ~last_buttons;
  last_buttons = buttons;

  if(state != old_state)
  {
    state->init();
  }

  old_state=state;
  state->run();
  
  if(buttons_edge){
    if(buttons_edge == konami_code[konami_counter]){
        konami_counter++;
    }else{
        konami_counter = 0;
        if(buttons_edge == konami_code[konami_counter]){
          konami_counter++;
        }
    }
    if(konami_counter == 10){
        //TODO
        konami_counter = 0;
        state = &menu_konami;
    }
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
    draw_sprite(SQUARE_SPRITE, LASER_BG_DATA);
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

int length_digits[3];

void menu_length_render(){
    uint8_t shallow_color = (setting_depth > 80) ? COLOR_BLACK : COLOR_GREEN;
    uint8_t deep_color = (setting_depth < 80) ? COLOR_BLACK : COLOR_GREEN;
    
    draw_sprite_color(TXT_SO_SHALLOW_SPRITE, LENGTH_BG_DATA, shallow_color);
    draw_sprite_color(TXT_MUCH_DEEP_SPRITE, LENGTH_BG_DATA, deep_color);

    blit_number(3, 33, setting_length, (uint8_t*) LENGTH_BG_DATA, COLOR_RED);
}

void menu_length_recalc(){
    length_digits[2] = setting_length % 10;
    length_digits[1] = (setting_length / 10) % 10;
    length_digits[0] = (setting_length / 100) % 10; // Shouldn't be more than 9
}

void menu_length_init(){
    lcd_blit_mem(0, 0, LENGTH_BG_IMAGE);
    draw_sprite(TXT_HOW_MANY_FEAT_LONG_SPRITE, LENGTH_BG_DATA);
    menu_length_recalc();
    menu_length_render();
    

}

void menu_length_run(){
    static uint8_t length_state = 0;
    int digit_dy;
    uint8_t rainbow_color = RAINBOW[(time/RAINBOW_PERIOD) % sizeof(RAINBOW)];

    // Assume fixed-width font because lazy
    digit_dy = digit_img[0].dy * (length_state - 1);
    
    if(length_state == 1){
        // Hundreds Digit
        if(buttons_edge & BUTTON_DOWN){
            if(setting_length > 200){
                setting_length -= 100;
            }
        }
        if(buttons_edge & BUTTON_UP){
            if(setting_length < 900){
                setting_length += 100;
            }
        }
    }else if(length_state == 2){
        // Tens Digit
        if(buttons_edge & BUTTON_DOWN){
            if(setting_length > 110){
                setting_length -= 10;
            }
        }
        if(buttons_edge & BUTTON_UP){
            if(setting_length < 990){
                setting_length += 10;
            }
        }
    }else if(length_state == 3){
        // Tens Digit
        if(buttons_edge & BUTTON_DOWN){
            if(setting_length > 101){
                setting_length -= 1;
            }
        }
        if(buttons_edge & BUTTON_UP){
            if(setting_length < 999){
                setting_length += 1;
            }
        }
    }

    if(length_state == 0){
        if(setting_depth < 80){
            if(buttons_edge & (BUTTON_DOWN)){
                draw_sprite(TXT_SO_SHALLOW_SPRITE, LENGTH_BG_DATA);
                setting_depth = 100;
            }else{
                draw_sprite_color(TXT_SO_SHALLOW_SPRITE, LENGTH_BG_DATA, rainbow_color);
            }
        }else{
            if(buttons_edge & (BUTTON_UP)){
                draw_sprite(TXT_MUCH_DEEP_SPRITE, LENGTH_BG_DATA);
                setting_depth = 50;
            }else{
                draw_sprite_color(TXT_MUCH_DEEP_SPRITE, LENGTH_BG_DATA, rainbow_color);
            }
        }
    }else{
        if(buttons_edge & (BUTTON_DOWN | BUTTON_UP)){
            menu_length_recalc();
        }
        blit_digit(3,33 + digit_dy, length_digits[length_state -1], (uint8_t*) LENGTH_BG_DATA, rainbow_color);
    } 

    if(buttons_edge & (BUTTON_RIGHT)){
        if(length_state == 1 || length_state == 2){
            length_state++;
            menu_length_render();
        }
    }
    if((buttons_edge & BUTTON_LEFT)){
        if(length_state == 2 || length_state == 3){
            length_state--;
            menu_length_render();
        }
    }
    if(buttons_edge & (BUTTON_A)){
        if(length_state == 0){
            length_state = 1;
        }else{
            length_state = 0;
        }
        menu_length_render();
    }
    if(buttons_edge & BUTTON_B){
        length_state = 0;
        digit_dy = 0;
        state = &menu_main;
    }
}

int speed_digits[3];


void menu_speed_render(){


    blit_number(30, 30, setting_speed, (uint8_t*) LENGTH_BG_DATA, COLOR_BLUE);
}

void menu_speed_recalc(){
    speed_digits[2] = setting_speed % 10;
    speed_digits[1] = (setting_speed / 10) % 10;
    speed_digits[0] = (setting_speed / 100) % 10; // Shouldn't be more than 9
}

void menu_speed_init(){
    lcd_blit_mem(0, 0, LENGTH_BG_IMAGE);
    draw_sprite(TXT_HOW_MANY_FEAT_LONG_SPRITE, LENGTH_BG_DATA);
    menu_speed_recalc();
    menu_speed_render();
}


void menu_speed_run(){
    //TODO
  static uint8_t speed_state = 0;
    int digit_dy;
    uint8_t rainbow_color = RAINBOW[(time/RAINBOW_PERIOD) % sizeof(RAINBOW)];

    // Assume fixed-width font because lazy
    digit_dy = digit_img[0].dy * (speed_state );
    
    if(speed_state == 1){
        // Hundreds Digit
        if(buttons_edge & BUTTON_DOWN){
            if(setting_speed > 200){
                setting_speed -= 100;
            }
        }
        if(buttons_edge & BUTTON_UP){
            if(setting_speed < 900){
                setting_speed += 100;
            }
        }
    }else if(speed_state == 2){
        // Tens Digit
        if(buttons_edge & BUTTON_DOWN){
            if(setting_speed > 110){
                setting_speed -= 10;
            }
        }
        if(buttons_edge & BUTTON_UP){
            if(setting_speed < 990){
                setting_speed += 10;
            }
        }
    }else if(speed_state == 3){
        // Tens Digit
        if(buttons_edge & BUTTON_DOWN){
            if(setting_speed > 101){
                setting_speed -= 1;
            }
        }
        if(buttons_edge & BUTTON_UP){
            if(setting_speed < 999){
                setting_speed += 1;
            }
        }
    }


      if(buttons_edge & (BUTTON_DOWN | BUTTON_UP)){
          menu_speed_recalc();
      }
      blit_digit(30,30 + digit_dy, speed_digits[speed_state], (uint8_t*) LENGTH_BG_DATA, rainbow_color);
   

    if(buttons_edge & (BUTTON_RIGHT)){
        if(speed_state == 1 || speed_state == 0){
            speed_state++;
            menu_speed_render();
        }
    }
    if((buttons_edge & BUTTON_LEFT)){
        if(speed_state == 2 || speed_state == 1){
            speed_state--;
            menu_speed_render();
        }
    }

    if(buttons_edge & (BUTTON_A | BUTTON_B)){
        speed_state = 0;
        digit_dy = 0;
        state = &menu_main;
    }

}

void menu_splash_init()
{
  splash_timeout=time+2000;
  lcd_blit_mem(0, 0, SPLASH_IMAGE);
}

void menu_splash_run()
{
  int rainbow_ctr = (time/RAINBOW_PERIOD) % sizeof(RAINBOW);
  lcd_blit_sprite(7, 23, SPLASH_TXT_IMAGE, (uint8_t*) SPLASH_DATA, RAINBOW[rainbow_ctr]);

  if((time>splash_timeout) || (buttons & BUTTON_A))
  {
      state=&menu_main;
  }
}

void menu_konami_init(){
  konami_index=0;
  menu_konami_redraw();
}

void menu_konami_redraw()
{
  lcd_blit_sd(0,0,dxLCDScreen,dyLCDScreen,konami_index*KONAMI_SEQUENCE_STEP);
}

void menu_konami_run(){
  if(buttons_edge & (BUTTON_RIGHT | BUTTON_A))
  {
    if(konami_index < KONAMI_SEQUENCE_LENGTH)
    {
      konami_index++;
      menu_konami_redraw();
    }
  }
  else if(buttons_edge & BUTTON_LEFT)
  {
    if(konami_index > 0)
    {
      konami_index--;
      menu_konami_redraw();
    }
  }
  else if(buttons_edge & BUTTON_B)
  {
    state=&menu_main;
  }
}

void menu_wow_init()
{
  lcd_blit_mem(0, 0, SWIMMING_IMAGE);
  wow_start_time=time;
  wow_last_time=-1;
  set_laser(1);
}

void menu_wow_deinit()
{
  set_laser(0);
  set_laser_pos(PWM_PERIOD/2,PWM_PERIOD/2);
}

void menu_wow_run()
{
  static uint8_t rainbow_counter=0;
  int wow_time;

  wow_time=(time-wow_start_time)/(setting_speed*1000);
  if(wow_time != wow_last_time)
  {
    blit_number(10,50,wow_time,(uint8_t*)SWIMMING_DATA,RAINBOW[rainbow_counter++]);
    if(rainbow_counter>sizeof(RAINBOW))
    {
      rainbow_counter=0;
    }
    wow_last_time=wow_time;
    if(wow_time != 0){
      lcd_blit_sprite(rand() & 0x7F, rand() & 0x7F, TXT_WOW_IMAGE, (uint8_t*) SWIMMING_DATA, RAINBOW[rainbow_counter]);
    }
  }

  project(setting_laser_shape,setting_depth,setting_length,setting_speed,time-wow_start_time);
  
  if(buttons & BUTTON_B)
  {
    menu_wow_deinit();
    state=&menu_main;
  }
}

uint8_t rand_base = 0;

uint8_t rand(){
    return (rand_base + ((time & 0xff00) >> 8)) ^ ((time & 0x00ff) * rand_base++);
}
