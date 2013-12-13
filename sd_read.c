#include "sd_read.h"
#include "sd.h"

void sd_read()
{
  SD_Error Status;
  if(SD_Init() != SD_RESPONSE_NO_ERROR)
  {
    for(;;);
  }
}
