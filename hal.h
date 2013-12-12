#include "stm32f10x.h"

#define dxLCDScreen 130
#define dyLCDScreen 130

#define lctCmd 0
#define lctData 1

#define LCD_PACK(x,y) ((uint16_t)((x)<<8) | (y))

#define BUTTON_UP (1 << 0)
#define BUTTON_RIGHT (1 << 1)
#define BUTTON_LEFT (1 << 2)
#define BUTTON_DOWN (1 << 3)
#define BUTTON_A (1 << 4)
#define BUTTON_B (1 << 5)

extern uint32_t time;

void hardware_init();
void set_laser_pos(int x,int y);
void set_laser(uint8_t on);
void send_screen_array(uint16_t* datum,int n);
void send_screen_byte(uint16_t datum);
void fill_screen(uint32_t datum,int n);
void reset_screen();
void init_lcd();
void lcd_blit_mem(uint8_t x,uint8_t y,uint8_t dx,uint8_t dy,uint8_t* img);
void lcd_blit_fun(uint8_t x,uint8_t y,uint8_t dx,uint8_t dy,uint8_t (*fun)(uint8_t,uint8_t));
void lcd_blit_sprite(uint8_t x,uint8_t y,uint8_t dx,uint8_t dy,uint8_t* img,uint8_t* bg,uint8_t color);
uint8_t read_buttons();
uint8_t read_buttons_debounced();
void busy_wait(__IO uint32_t nCount);
