#include <stdint.h>

typedef uint8_t btn_t;
typedef enum {
    MENU_SELF,
    MENU_MAIN,
    MENU_SPLASH
} menu_state_t;

#define COLOR_RED 0xE0
#define COLOR_ORANGE 0xF4
#define COLOR_YELLOW 0xFC
#define COLOR_PINK 0xF3
#define COLOR_GREEN 0x1C
#define COLOR_BLUE 0x03
#define COLOR_CYAN 0x1F
#define COLOR_WHITE 0xFF
#define COLOR_BLACK 0x00

// The main function to render the current menu 
void render_menu();

// The main settings menu
menu_state_t menu_main(btn_t);
// The splash screen
menu_state_t menu_splash(btn_t);
