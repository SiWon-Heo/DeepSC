void curses_show_connections ( void ) {
 DEBUG_MSG ( "curses_show_connections" ) ;
 if ( wdg_connections ) {
 wdg_set_focus ( wdg_connections ) ;
 return ;
 }
 wdg_create_object ( & wdg_connections , WDG_DYNLIST , WDG_OBJ_WANT_FOCUS ) ;
 wdg_set_title ( wdg_connections , "Live connections:" , WDG_ALIGN_LEFT ) ;
 wdg_set_size ( wdg_connections , 1 , 2 , - 1 , SYSMSG_WIN_SIZE - 1 ) ;
 wdg_set_color ( wdg_connections , WDG_COLOR_SCREEN , EC_COLOR ) ;
 wdg_set_color ( wdg_connections , WDG_COLOR_WINDOW , EC_COLOR ) ;
 wdg_set_color ( wdg_connections , WDG_COLOR_BORDER , EC_COLOR_BORDER ) ;
 wdg_set_color ( wdg_connections , WDG_COLOR_FOCUS , EC_COLOR_FOCUS ) ;
 wdg_set_color ( wdg_connections , WDG_COLOR_TITLE , EC_COLOR_TITLE ) ;
 wdg_draw_object ( wdg_connections ) ;
 wdg_set_focus ( wdg_connections ) ;
 wdg_dynlist_print_callback ( wdg_connections , conntrack_print ) ;
 wdg_dynlist_select_callback ( wdg_connections , curses_connection_data ) ;
 wdg_add_idle_callback ( refresh_connections ) ;
 wdg_add_destroy_key ( wdg_connections , CTRL ( 'Q' ) , curses_kill_connections ) ;
 wdg_dynlist_add_callback ( wdg_connections , 'd' , curses_connection_detail ) ;
 wdg_dynlist_add_callback ( wdg_connections , 'k' , curses_connection_kill ) ;
 wdg_dynlist_add_callback ( wdg_connections , 'x' , curses_connection_purge ) ;
 wdg_dynlist_add_callback ( wdg_connections , ' ' , curses_connection_help ) ;
 }