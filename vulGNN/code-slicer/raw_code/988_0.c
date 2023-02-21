static void curses_connection_inject ( void ) {
 wdg_t * in ;
 DEBUG_MSG ( "curses_connection_inject" ) ;
 SAFE_REALLOC ( injectbuf , 501 * sizeof ( char ) ) ;
 memset ( injectbuf , 0 , 501 ) ;
 wdg_create_object ( & in , WDG_INPUT , WDG_OBJ_WANT_FOCUS | WDG_OBJ_FOCUS_MODAL ) ;
 wdg_set_color ( in , WDG_COLOR_SCREEN , EC_COLOR ) ;
 wdg_set_color ( in , WDG_COLOR_WINDOW , EC_COLOR ) ;
 wdg_set_color ( in , WDG_COLOR_FOCUS , EC_COLOR_FOCUS ) ;
 wdg_set_color ( in , WDG_COLOR_TITLE , EC_COLOR_MENU ) ;
 wdg_input_size ( in , 75 , 12 ) ;
 wdg_input_add ( in , 1 , 1 , "Chars to be injected :" , ( char * ) injectbuf , 50 , 10 ) ;
 wdg_input_set_callback ( in , inject_user ) ;
 wdg_draw_object ( in ) ;
 wdg_set_focus ( in ) ;
 }