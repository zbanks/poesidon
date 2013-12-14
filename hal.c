#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_spi.h"
#include "hal.h"

GPIO_InitTypeDef GPIO_InitStructure;
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_OCInitTypeDef TIM_OCInitStructure;
USART_ClockInitTypeDef USART_ClockInitStructure;
USART_InitTypeDef USART_InitStructure;

uint32_t time=0;

void hardware_init()
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | 
                         RCC_APB2Periph_GPIOB | 
                         RCC_APB2Periph_GPIOC | 
                         RCC_APB2Periph_TIM1 | 
                         RCC_APB2Periph_AFIO | 
                         RCC_APB2Periph_SPI1, ENABLE);

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  
  // PA8, PA9 for laser
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  // PB13, PB14 for laser
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  // PB10 for laser on/off
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_ResetBits(GPIOB,GPIO_Pin_10); // Laser off

  // PA2, PA4 for talking to screen
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  // PWM for galvos
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = PWM_PERIOD-1;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Set;

  TIM_OCInitStructure.TIM_Pulse = PWM_PERIOD/2;
  TIM_OC1Init(TIM1, &TIM_OCInitStructure);

  TIM_OCInitStructure.TIM_Pulse = PWM_PERIOD/2;
  TIM_OC2Init(TIM1, &TIM_OCInitStructure);

  TIM_Cmd(TIM1, ENABLE);
  TIM_CtrlPWMOutputs(TIM1, ENABLE);

  // USART for screen
  USART_ClockInitStructure.USART_Clock = USART_Clock_Enable;
  USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
  USART_ClockInitStructure.USART_CPHA = USART_CPHA_1Edge;
  USART_ClockInitStructure.USART_LastBit = USART_LastBit_Enable;
  USART_ClockInit(USART2, &USART_ClockInitStructure);

  USART_OverSampling8Cmd(USART2, ENABLE);

  USART_InitStructure.USART_BaudRate = 3000000;
  USART_InitStructure.USART_WordLength = USART_WordLength_9b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Tx;
  USART_Init(USART2, &USART_InitStructure);

  USART_Cmd(USART2, ENABLE);
  
  // screen ON, CS, RESET
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_SetBits(GPIOA,GPIO_Pin_3);
  GPIO_SetBits(GPIOA,GPIO_Pin_5);

  // Buttons
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 |
                                GPIO_Pin_1 |
                                GPIO_Pin_2 |
                                GPIO_Pin_3 |
                                GPIO_Pin_4 |
                                GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  // SD Card
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 |
                                GPIO_Pin_4 |
                                GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_SetBits(GPIOA, GPIO_Pin_15);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SPI1,ENABLE);
  
  SPI_InitTypeDef SPI_InitStructure;
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);

  /* Enable SPIy */
  SPI_Cmd(SPI1, ENABLE);
  
  // Systick on
  SysTick_Config(SystemCoreClock / 1000);
}

void set_laser_pos(int x,int y)
{
  if(x>10000)
  {
    x=10000;
  }
  if(x<0)
  {
    x=0;
  }
  if(y>10000)
  {
    y=10000;
  }
  if(y<0)
  {
    y=0;
  }
  TIM_OCInitStructure.TIM_Pulse = x;
  TIM_OC1Init(TIM1, &TIM_OCInitStructure);

  TIM_OCInitStructure.TIM_Pulse = y;
  TIM_OC2Init(TIM1, &TIM_OCInitStructure);
}

void set_laser(uint8_t on)
{
  if(on)
  {
    GPIO_SetBits(GPIOB,GPIO_Pin_10);
  }
  else
  {
    GPIO_ResetBits(GPIOB,GPIO_Pin_10);
  }
}

void send_screen_array(uint16_t* datum,int n)
{
  int i;

  uint32_t reversed;

  GPIO_ResetBits(GPIOA,GPIO_Pin_3);

  for(i=0;i<n;i++)
  {
    asm("rbit %0, %1\n" : "=r"(reversed) : "r"(datum[i]));
    USART_SendData(USART2, (uint16_t)(reversed>>23));
    while(!USART_GetFlagStatus(USART2, USART_FLAG_TXE));
  }

  while(!USART_GetFlagStatus(USART2, USART_FLAG_TC));
  GPIO_SetBits(GPIOA,GPIO_Pin_3);  
}

void reset_screen()
{
  GPIO_ResetBits(GPIOA,GPIO_Pin_5);
  busy_wait(1000000);
  GPIO_SetBits(GPIOA,GPIO_Pin_5);
  busy_wait(100000);
}

void busy_wait(uint32_t nCount)
{
  __IO uint32_t n=nCount;
  for(; n != 0; n--);
}

void send_screen_byte(uint16_t datum)
{
  GPIO_ResetBits(GPIOA,GPIO_Pin_3);

  uint32_t reversed;

  asm("rbit %0, %1\n" : "=r"(reversed) : "r"(datum));
  USART_SendData(USART2, (uint16_t)(reversed>>23));
  while(!USART_GetFlagStatus(USART2, USART_FLAG_TC));

  GPIO_SetBits(GPIOA,GPIO_Pin_3);
}

#define DISON       0xaf
#define DISOFF      0xae
#define DISNOR      0xa6
#define DISINV      0xa7
#define COMSCN      0xbb
#define DISCTL      0xca
#define SLPIN       0x95
#define SLPOUT      0x94
#define PASET       0x75
#define CASET       0x15
#define DATCTL      0xbc
#define RGBSET8     0xce
#define RAMWR       0x5c
#define RAMRD       0x5d
#define PTLIN       0xa8
#define PTLOUT      0xa9
#define RMWIN       0xe0
#define RMWOUT      0xee
#define ASCSET      0xaa
#define SCSTART     0xab
#define OSCON       0xd1
#define OSCOFF      0xd2
#define PWRCTR      0x20
#define VOLCTR      0x81
#define VOLUP       0xd6
#define VOLDOWN     0xd7
#define TMPGRD      0x82
#define EPCTIN      0xcd
#define EPCOUT      0xcc
#define EPMWR       0xfc
#define EPMRD       0xfd
#define EPSRRD1     0x7c
#define EPSRRD2     0x7d
#define NOP         0x25
#define xlMin 0
#define ylMin 2

static const uint16_t lcd_init[]={

  LCD_PACK(lctCmd, DISCTL),	// Display Control
  LCD_PACK(lctData, 0x00),	// default
  LCD_PACK(lctData, 0x20),	// (32 + 1) * 4 = 132 lines (of which 130 are visible)
  LCD_PACK(lctData, 0x0a),	// default

  LCD_PACK(lctCmd, COMSCN),	// Common Scan
  LCD_PACK(lctData, 0x01),	// COM1-80: 1->80, COM81-160: 160<-81 (otherwise bottom of screen is upside down)

  LCD_PACK(lctCmd, OSCON),	// Oscillation On
  LCD_PACK(lctCmd, SLPOUT),	// Sleep Out (exit sleep mode)

  LCD_PACK(lctCmd, VOLCTR),	// Electronic Volume Control (brightness/contrast)
  LCD_PACK(lctData, 55),
  LCD_PACK(lctData, 3),

  LCD_PACK(lctCmd, TMPGRD),
  LCD_PACK(lctData, 0),		// default

  LCD_PACK(lctCmd, PWRCTR),	// Power Control Set
  LCD_PACK(lctData, 0x0f),

  LCD_PACK(lctCmd, DISINV),	// Inverse Display
  LCD_PACK(lctCmd, PTLOUT),	// Partial Out (no partial display)

  LCD_PACK(lctCmd, DATCTL),	// Data Control
  LCD_PACK(lctData, 0x00),	// normal orientation, scan across cols, then rows
  LCD_PACK(lctData, 0x00),	// RGB arrangement (RGB all rows/cols)
  //	LCD_PACK(lctData, 0x04),	// RGB arrangement (RGB row 1, BGR row 2)
  LCD_PACK(lctData, 0x01),	// 8 bit-color display
  //	LCD_PACK(lctData, 0x02),	// 16 bit-color display

  /* if 256-color mode, bytes represent RRRGGGBB, the following
  maps to 4-bit color for each value in range (0-7 R/G, 0-3 B) */

  LCD_PACK(lctCmd, RGBSET8),	// 256-color position set
  LCD_PACK(lctData, 0x00),	// 000 RED
  LCD_PACK(lctData, 0x02),	// 001  
  LCD_PACK(lctData, 0x04),	// 010
  LCD_PACK(lctData, 0x06),	// 011
  LCD_PACK(lctData, 0x08),	// 100
  LCD_PACK(lctData, 0x0a),	// 101
  LCD_PACK(lctData, 0x0c),	// 110
  LCD_PACK(lctData, 0x0f),	// 111
  LCD_PACK(lctData, 0x00),	// 000 GREEN
  LCD_PACK(lctData, 0x02),	// 001  
  LCD_PACK(lctData, 0x04),	// 010
  LCD_PACK(lctData, 0x06),	// 011
  LCD_PACK(lctData, 0x08),	// 100
  LCD_PACK(lctData, 0x0a),	// 101
  LCD_PACK(lctData, 0x0c),	// 110
  LCD_PACK(lctData, 0x0f),	// 111
  LCD_PACK(lctData, 0x00),	//  00 BLUE
  LCD_PACK(lctData, 0x06),	//  01
  LCD_PACK(lctData, 0x09),	//  10
  LCD_PACK(lctData, 0x0f),	//  11
};

static uint16_t lcd_write[]={
  LCD_PACK(lctCmd, PASET),	// Page Address Set
  LCD_PACK(lctData, ylMin),	// start page (line)
  LCD_PACK(lctData, dyLCDScreen-1),	// end page
  LCD_PACK(lctCmd, CASET),	// Column Address Set
  LCD_PACK(lctData, xlMin),	// start address
  LCD_PACK(lctData, dxLCDScreen-1),	// end address
  LCD_PACK(lctCmd, RAMWR),	// Memory Write
};

void init_lcd()
{
  reset_screen();

  send_screen_array((uint16_t*)lcd_init,sizeof(lcd_init)/sizeof(lcd_init[0]));

  busy_wait(100000);
  
  send_screen_byte(LCD_PACK(lctCmd, DISON));
}

static void lcd_setup_write(uint8_t x,uint8_t y,uint8_t dx,uint8_t dy)
{
  lcd_write[1]=LCD_PACK(lctData,ylMin+y);
  lcd_write[2]=LCD_PACK(lctData,ylMin+y+dy-1);
  lcd_write[4]=LCD_PACK(lctData,xlMin+x);
  lcd_write[5]=LCD_PACK(lctData,xlMin+x+dx-1);
  send_screen_array((uint16_t*)lcd_write,sizeof(lcd_write)/sizeof(lcd_write[0]));
}

void lcd_blit_mem(uint8_t x,uint8_t y,uint8_t dx,uint8_t dy,uint8_t* img)
{
  int i;
  uint32_t reversed;
  int n=dx*dy;

  lcd_setup_write(x,y,dx,dy);


  GPIO_ResetBits(GPIOA,GPIO_Pin_3);

  for(i=0;i<n;i++)
  {
    asm("rbit %0, %1\n" : "=r"(reversed) : "r"(LCD_PACK(lctData,img[i])));
    USART_SendData(USART2, (uint16_t)(reversed>>23));
    while(!USART_GetFlagStatus(USART2, USART_FLAG_TXE));
  }

  while(!USART_GetFlagStatus(USART2, USART_FLAG_TC));
  GPIO_SetBits(GPIOA,GPIO_Pin_3);  
}

void lcd_blit_sprite(uint8_t x,uint8_t y,uint8_t dx,uint8_t dy,uint8_t* img,uint8_t* bg,uint8_t color)
{
  int i,j;
  uint32_t reversed;
  uint8_t fgc,out;
  uint8_t c = 0;

  lcd_setup_write(x,y,dx,dy);
  
  GPIO_ResetBits(GPIOA,GPIO_Pin_3);

  for(j=0;j<dy;j++)
  {
    for(i=0;i<dx;i++)
    {
      if(!c){
          fgc = (*img & 0xE0)>>5;
          c = 1;
      }else{
          fgc=(*img & 0x0E)>>1;
          img++;
          c = 0;
      }
      
      if(fgc==7)
      {
        out=bg[dxLCDScreen*(j+y)+(i+x)];
      }
      else if(fgc==0)
      {
        out=color;
      }
      else
      {
        uint8_t bgc=bg[dxLCDScreen*(j+y)+(i+x)];
        
        out=(((((bgc & 0xE0)>>5)*fgc+((color & 0xE0)>>5)*(7-fgc))/7)<<5)+
        (((((bgc & 0x1C)>>2)*fgc+((color & 0x1C)>>2)*(7-fgc))/7)<<2)+
        (((((bgc & 0x03))*fgc+((color & 0x03))*(7-fgc))/7));
      }

      asm("rbit %0, %1\n" : "=r"(reversed) : "r"(LCD_PACK(lctData,out)));
      USART_SendData(USART2, (uint16_t)(reversed>>23));
      while(!USART_GetFlagStatus(USART2, USART_FLAG_TXE));
    }
  }

  while(!USART_GetFlagStatus(USART2, USART_FLAG_TC));
  GPIO_SetBits(GPIOA,GPIO_Pin_3);  
}

uint8_t read_buttons()
{
  return ~GPIO_ReadInputData(GPIOC) & 0x3F;
}

int debounce[6];
uint8_t debounced=0;

uint8_t read_buttons_debounced()
{
  int i;
  uint8_t state;
  uint8_t switches=read_buttons();
  for(i=0;i<6;i++)
  {
    state = 1 & (switches >> i);
    if(!state && debounce[i]>0)
    {
      debounce[i]--;
      if(debounce[i] < 1)
      {
        debounced &= ~(1<<i);
      }
    }
    else if(state && debounce[i]<3)
    {
      debounce[i]++;
      if(debounce[i] > 2)
      {
        debounced |= 1<<i;
      }
    }
  }
  return debounced;
}

/*
    int xa=5000+square(1000,30,time-start_t+15);
    int ya=get_mirror_angle(y,32);
    ya+=square(500,2,time-start_t);
    set_laser_pos(xa,ya);

*/
