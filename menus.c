#include <menus.h>
#include <hal.h>

#define MENU_SPLASH_TIMEOUT 30

void render_menu(){
    // The main function to render the current menu 
    static menu_state_t state = MENU_SPLASH;
    menu_state_t new_state;
    static uint8_t konami_counter = 0;

    buttons = get_buttons();

    if(buttons){
        switch(komani_counter){
            //TODO
        }
        if(komani_counter == 10){
            //TODO
        }
    }

    switch(state){
        case MENU_MAIN:
            new_state = menu_main(buttons);
        break;
        case MENU_SPLASH:
            new_state = menu_splash(buttons);
        break;
        default:
            new_state = MENU_SPLASH;
        break;
    }
    if(new_state != MENU_SELF){
        state = new_state;
    }
}

menu_state_t menu_main(btn_t buttons){
    // The main settings menu
    static uint8_t force_redraw = 1;
    static uint8_t selection = 0;
    static uint8_t rainbow_ctr = 0;
    /* 
        Speed -- Such Speed
        Depth -- Much Length
        Shape --  Bright Laser
        Go -- Wow
    */

    if(buttons & BUTTON_UP){
        if(selection < 3){
            selection++;
            force_redraw = 1;
        }
    }

    if(buttons & BUTTON_DOWN){
        if(selection > 0){
            selection--;
            force_redraw = 1;
        }
    }

    if(buttons & BUTTON_A){
        //TODO
        force_redraw = 1;
        return MENU_MAIN;
    }

    if(force_redraw){
        //TODO: Draw the background image
        lcd_blt_background(0, 0, DOGE_WATER_IMAGE);
        lcd_blt_sprite(100, 20, TXT_BRIGHT_LASER_IMAGE, COLOR_RED);
        lcd_blt_sprite(70, 80, TXT_MUCH_LENGTH_IMAGE, COLOR_CYAN);
        lcd_blt_sprite(40, 30, TXT_SUCH SPEED_IMAGE, COLOR_YELLOW);
        lcd_blt_sprite(10, 90, TXT_WOW_IMAGE, COLOR_GREEN);

        force_redraw = 0;
    }

    // Draw selection in rainbow
    switch(selection){
        case 0:
            lcd_blt_sprite(100, 20, TXT_BRIGHT_LASER_IMAGE, (uint8_t) RAINBOW[rainbow_ctr]);
        break;
        case 1:
            lcd_blt_sprite(70, 80, TXT_MUCH_LENGTH_IMAGE, (uint8_t) RAINBOW[rainbow_ctr]);
        break;
        case 2:
            lcd_blt_sprite(40, 30, TXT_SUCH SPEED_IMAGE, (uint8_t) RAINBOW[rainbow_ctr]);
        break;
        case 3:
            lcd_blt_sprite(10, 90, TXT_WOW_IMAGE, (uint8_t) RAINBOW[rainbow_ctr]);
        break;
        default:
            selection = 0;
        break;
    }

    return MENU_SELF;
}

menu_state_t menu_splash(btn_t buttons){
    // The splash screen
    static uint16_t timeout = MENU_SPLASH_TIMEOUT;

    if(timeout == MENU_SPLASH_TIMEOUT){
        lcd_blit_mem(0, 0, SPLASH_IMAGE);
    }else{
        lcd_blit_sprite(7, 23, SPLASH_TXT_SPRITE);
        // cycle colors or some shit
    }

    if(--timeout || (buttons & BUTTON_A)){
        timeout = MENU_SPLASH_TIMEOUT;
        return MENU_MAIN;
    }
    return MENU_SELF;
}
