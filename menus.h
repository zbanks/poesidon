typedef uint8_t btn_t;
typedef enum {
    MENU_SELF,
    MENU_MAIN,
    MENU_SPLASH
} menu_state_t;

// The main function to render the current menu 
void render_menu(btn_t);

// The main settings menu
void menu_main(btn_t);
// The splash screen
void menu_splash(btn_t);
