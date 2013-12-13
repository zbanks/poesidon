#include "stm32f10x.h"
#include "hal.h"
#include "project.h"
#include "images.h"
#include "menus.h"

int main(void)
{
  hardware_init();
  busy_wait(100000);
  init_lcd();
  
  init_menu();
  for(;;)
  {
      render_menu();
  }
}

  /*
  set_laser(0);

  //busy_wait(1000000);
  init_lcd();

  lcd_blit_mem(0,0,SPLASH_WIDTH,SPLASH_HEIGHT,(uint8_t*)SPLASH_DATA);

  //lcd_blit_mem(100,20,X_WIDTH,X_HEIGHT,(uint8_t*)X_DATA);
  
  int start_t=time;
 
  int i;
  
  while (1)
  {
    i++;
    int y=ramp(100,5000,time-start_t);

    int xa=5000+square(300,50,time-start_t+25);
    int ya=get_mirror_angle(y,32);
    ya+=square(300,50,time-start_t);
    set_laser_pos(xa,ya);

    lcd_blit_sprite(7,23,SPLASH_TXT_WIDTH,SPLASH_TXT_HEIGHT,(uint8_t*)SPLASH_TXT_DATA,(uint8_t*)SPLASH_DATA,RAINBOW[i%sizeof(RAINBOW)]);
    busy_wait(100000);
  }
  */

/*
    int xa=5000+square(1000,30,time-start_t+15);
    int ya=get_mirror_angle(y,32);
    ya+=square(500,2,time-start_t);
    set_laser_pos(xa,ya);
  

*/

#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{ 
  while (1)
  {
  }
}
#endif
