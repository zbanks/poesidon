#include <menus.h>
#include <hal.h>
#include <images.h>

uint8_t RAINBOW[7]={0xE0,0xF4,0xFC,0x1C,0x1F,0x4B,0xE3};

#define MENU_SPLASH_TIMEOUT 300
#define SPLASH_IMAGE SPLASH_WIDTH,SPLASH_HEIGHT,(uint8_t*)SPLASH_DATA
//#define SPLASH_TXT_IMAGE SPLASH_TXT_WIDTH,SPLASH_TXT_HEIGHT,(uint8_t*)SPLASH_TXT_DATA

#define SPLASH_TXT_IMAGE TXT_WOW_IMAGE

#define DOGE_WATER_DATA SPLASH_DATA
#define DOGE_WATER_IMAGE SPLASH_IMAGE

#define RAINBOW_PERIOD 100

// The main settings menu
void menu_main_init();
void menu_main_run();
void menu_main_redraw(uint8_t s);

// The splash screen
void menu_splash_init();
void menu_splash_run();

int splash_timeout;

typedef struct {
  void (* const init)();
  void (* const run)();
} state_t;

const state_t menu_main = {&menu_main_init,&menu_main_run};
const state_t menu_splash = {&menu_splash_init,&menu_splash_run};

state_t const* state;

btn_t buttons,buttons_edge;

typedef struct {
  uint8_t x,y,dx,dy;
  uint8_t* img;
  uint8_t color;
} sprite_t;

const sprite_t sprites[] = {
  {100, 20, TXT_BRIGHT_LASER_IMAGE, COLOR_RED},
  {70, 80, TXT_MUCH_LENGTH_IMAGE, COLOR_CYAN},
  {40, 30, TXT_SUCH_SPEED_IMAGE, COLOR_YELLOW},
  {10, 90, TXT_WOW_IMAGE, COLOR_GREEN},
};

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
        switch(konami_counter){
            //TODO
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

void menu_main_init()
{
  lcd_blit_mem(0, 0, DOGE_WATER_IMAGE);
  int i;
  for(i=0;i<4;i++)
  {
    menu_main_redraw(i);
  }
}

void menu_main_redraw(uint8_t s)
{
  lcd_blit_sprite(sprites[s].x,sprites[s].y,sprites[s].dx,sprites[s].dy,sprites[s].img,(uint8_t*) DOGE_WATER_DATA,sprites[s].color);
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
        return;
    }

    // Draw selection in rainbow
    lcd_blit_sprite(sprites[selection].x,sprites[selection].y,sprites[selection].dx,sprites[selection].dy,sprites[selection].img,(uint8_t*) DOGE_WATER_DATA,RAINBOW[rainbow_ctr]);

    return;
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
