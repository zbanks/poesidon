#include <stdint.h>

typedef struct
{
  uint8_t dx,dy;
  uint8_t* img;
} number_t;

extern const number_t digit_img[10]; 

uint8_t blit_digit(uint8_t x,uint8_t y,int n,uint8_t* bg,uint8_t color){
void blit_number(uint8_t x,uint8_t y,int num,uint8_t* bg,uint8_t color);
