#include <stdint.h>



extern uint16_t lcd_data[dxLCDScreen*dyLCDScreen];

void init_lcd();
void lcd_flip();
void lcd_fill(uint8_t color);
void lcd_setup_write(uint8_t x,uint8_t y,uint8_t dx,uint8_t dy)