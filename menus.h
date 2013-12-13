#include <stdint.h>

typedef uint8_t btn_t;

#define COLOR_RED 0xE0
#define COLOR_ORANGE 0xF4
#define COLOR_YELLOW 0xFC
#define COLOR_PINK 0xF3
#define COLOR_GREEN 0x1C
#define COLOR_BLUE 0x03
#define COLOR_CYAN 0x1F
#define COLOR_WHITE 0xFF
#define COLOR_BLACK 0x00

// Global settings
enum laser_shape {
    CIRCLE,
    DOT,
    LINE,
    LAST_SHAPE = LINE
};

extern enum laser_shape setting_laser_shape;
extern int setting_depth;
extern int setting_length;
extern int setting_speed;

// The main function to render the current menu 
void init_menu();
void render_menu();

