#define MENU_SPLASH_TIMEOUT 30

void render_menu(btn_t buttons){
    // The main function to render the current menu 
    static menu_state_t state = MENU_SPLASH;
    menu_state_t new_state;
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
    static uint8_t bg_drawn = 0;
    static uint8_t selection = 0;

    if(!bg_drawn){
        //TODO: Draw the background image
        bg_drawn = 1;
    }

    if(buttons & BUTTON_UP){

    }

    if(buttons & BUTTON_DOWN){

    }

    if(buttons & BUTTON_A){

    }

    if(0){ //TODO - fill with an actual transition
        bg_drawn = 0;
        return MENU_MAIN;
    }
    return MENU_SELF;
}

menu_state_t menu_splash(btn_t buttons){
    // The splash screen
    static uint16_t timeout = MENU_SPLASH_TIMEOUT;

    if(timeout == MENU_SPLASH_TIMEOUT){
        //TODO draw splash screen
    }else{
        // cycle colors or some shit
    }

    if(--timeout){
        timeout = MENU_SPLASH_TIMEOUT;
        return MENU_MAIN;
    }
    return MENU_SELF;
}
