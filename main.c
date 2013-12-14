#include "stm32f10x.h"
#include "hal.h"
#include "images.h"
#include "menus.h"

int main(void)
{
  hardware_init();
  busy_wait(100000);
  init_lcd();
  init_sd();

  init_menu();
  for(;;)
  {
    render_menu();
  }
}

#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t* file, uint32_t line)
{ 
  while (1)
  {
  }
}
#endif
