#include "numbers.h"
#include "images.h"
#include "hal.h"
#include "project.h"

typedef struct
{
  uint8_t dx,dy;
  uint8_t* img;
} number_t;

const number_t digit_img[10]={
  {TXT_0_IMAGE},
  {TXT_1_IMAGE},
  {TXT_2_IMAGE},
  {TXT_3_IMAGE},
  {TXT_4_IMAGE},
  {TXT_5_IMAGE},
  {TXT_6_IMAGE},
  {TXT_7_IMAGE},
  {TXT_8_IMAGE},
  {TXT_9_IMAGE},
};

void blit_number(uint8_t x,uint8_t y,int num,uint8_t* bg,uint8_t color)
{
  uint8_t digits[10];
  int n=0;
  int next;
  number_t const * img;

  do
  {
    next=num/10;
    digits[n++]=num-next*10;
    num=next;
  } while(num>0);
  for(n=n-1;n>=0;n--)
  {
    img=&digit_img[digits[n]];
    lcd_blit_sprite(x,y,img->dx,img->dy,img->img,bg,color);
    y+=img->dy;
  }
}
