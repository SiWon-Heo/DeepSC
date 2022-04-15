static void curses_connection_inject_file ( void ) {
 wdg_t * fop ;
 DEBUG_MSG ( "curses_connection_inject_file" ) ;
 wdg_create_object ( & fop , WDG_FILE , WDG_OBJ_WANT_FOCUS | WDG_OBJ_FOCUS_MODAL ) ;
 wdg_set_title ( fop , "Select a file to inject..." , WDG_ALIGN_LEFT ) ;
 wdg_set_color ( fop , WDG_COLOR_SCREEN , EC_COLOR ) ;
 wdg_set_color ( fop , WDG_COLOR_WINDOW , EC_COLOR_MENU ) ;
 wdg_set_color ( fop , WDG_COLOR_FOCUS , EC_COLOR_FOCUS ) ;
 wdg_set_color ( fop , WDG_COLOR_TITLE , EC_COLOR_TITLE ) ;
 wdg_file_set_callback ( fop , inject_file ) ;
 wdg_draw_object ( fop ) ;
 wdg_set_focus ( fop ) ;
 }