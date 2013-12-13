#include <menus.h>
#include <hal.h>
#include <images.h>

uint8_t RAINBOW[7]={0xE0,0xF4,0xFC,0x1C,0x1F,0x4B,0xE3};

#define MENU_SPLASH_TIMEOUT 300

#define RAINBOW_PERIOD 100


//FIXME:
#define LASER_BG_IMAGE SPLASH_IMAGE
#define LASER_BG_DATA SPLASH_DATA


// The main settings menu
void menu_main_init();
void menu_main_run();

// The laser settings menu
void menu_laser_init();
void menu_laser_run();

// The length/depth settings menu
void menu_length_init();
void menu_length_run();

// The speed settings menu
void menu_speed_init();
void menu_speed_run();

// The operating state + laser
void menu_run_init();
void menu_run_run();

// The splash screen
void menu_splash_init();
void menu_splash_run();

// The konami code
void menu_konami_init();
void menu_konami_run();

int splash_timeout;

typedef struct {
  void (* const init)();
  void (* const run)();
} state_t;

const state_t menu_main = {&menu_main_init,&menu_main_run};
const state_t menu_laser = {&menu_laser_init,&menu_laser_run};
const state_t menu_length = {&menu_length_init,&menu_length_run};
const state_t menu_speed = {&menu_speed_init,&menu_speed_run};
const state_t menu_run = {&menu_run_init,&menu_run_run};
const state_t menu_splash = {&menu_splash_init,&menu_splash_run};
const state_t menu_konami = {&menu_konami_init,&menu_konami_run};

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
    DOT_SPRITE
};

const sprite_t sprites[] = {
  {110, 10, TXT_BRIGHT_LASER_IMAGE, COLOR_RED},
  {90, 48, TXT_MUCH_LENGTH_IMAGE, COLOR_GREEN},
  {25, 20, TXT_SUCH_SPEED_IMAGE, COLOR_BLUE},
  {10, 90, TXT_WOW_IMAGE, COLOR_ORANGE},
  {80, 20, CIRCLE_IMAGE, COLOR_GREEN},
  {50, 70, DOT_IMAGE, COLOR_GREEN},
  {10, 25, LINE_IMAGE, COLOR_GREEN},
};

const uint8_t konami_code = {BUTTON_UP, BUTTON_UP, BUTTON_DOWN, BUTTON_DOWN, BUTTON_LEFT, BUTTON_LEFT, BUTTON_B, BUTTON_A};

enum laser_shape setting_laser_shape;
uint8_t setting_depth;
uint8_t setting_length;
uint8_t setting_speed;

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
        }
        if(konami_counter == 10){
            //TODO
        }
    }

  old_state=state;
  state->run();
  if(state != old_state)
  {
    state->init();
  }
}

void menu_main_redraw(uint8_t i);
inline void draw_sprite(enum sprite_names s, const uint8_t* bg);

void menu_main_init()
{
  lcd_blit_mem(0, 0, DOGE_WATER_IMAGE);
  int i;
  for(i=0;i<4;i++)
  {
    menu_main_redraw(i); &
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

    if(buttons_edge & BUTTON_A){
        //TODO
        if(selection == 0){
              state=&menu_laser;
        }
        return;
    }

    // Draw selection in rainbow
    //lcd_blit_sprite(sprites[selection].x,sprites[selection].y,sprites[selection].dx,sprites[selection].dy,sprites[selection].img,(uint8_t*) DOGE_WATER_DATA,RAINBOW[rainbow_ctr]);
    draw_sprite_color(selection, DOGE_WATER_DATA, RAINBOW[rainbow_ctr]);

    return;
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
