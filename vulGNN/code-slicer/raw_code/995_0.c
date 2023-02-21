static void curses_connection_data_split ( void ) {
 char tmp [ MAX_ASCII_ADDR_LEN ] ;
 char title [ MAX_ASCII_ADDR_LEN + 6 ] ;
 DEBUG_MSG ( "curses_connection_data_split" ) ;
 if ( wdg_conndata ) {
 struct conn_object * tmp_conn = curr_conn ;
 wdg_destroy_object ( & wdg_conndata ) ;
 curses_destroy_conndata ( ) ;
 curr_conn = tmp_conn ;
 }
 curr_conn -> flags |= CONN_VIEWING ;
 wdg_create_object ( & wdg_conndata , WDG_COMPOUND , WDG_OBJ_WANT_FOCUS ) ;
 wdg_set_color ( wdg_conndata , WDG_COLOR_SCREEN , EC_COLOR ) ;
 wdg_set_color ( wdg_conndata , WDG_COLOR_WINDOW , EC_COLOR ) ;
 wdg_set_color ( wdg_conndata , WDG_COLOR_FOCUS , EC_COLOR_FOCUS ) ;
 wdg_set_color ( wdg_conndata , WDG_COLOR_TITLE , EC_COLOR_TITLE ) ;
 wdg_set_title ( wdg_conndata , "Connection data" , WDG_ALIGN_LEFT ) ;
 wdg_set_size ( wdg_conndata , 1 , 2 , - 1 , SYSMSG_WIN_SIZE - 1 ) ;
 wdg_create_object ( & wdg_c1 , WDG_SCROLL , 0 ) ;
 snprintf ( title , MAX_ASCII_ADDR_LEN + 6 , "%s:%d" , ip_addr_ntoa ( & curr_conn -> L3_addr1 , tmp ) , ntohs ( curr_conn -> L4_addr1 ) ) ;
 wdg_set_title ( wdg_c1 , title , WDG_ALIGN_LEFT ) ;
 wdg_set_color ( wdg_c1 , WDG_COLOR_TITLE , EC_COLOR_TITLE ) ;
 wdg_set_color ( wdg_c1 , WDG_COLOR_FOCUS , EC_COLOR_FOCUS ) ;
 wdg_set_size ( wdg_c1 , 2 , 3 , current_screen . cols / 2 , SYSMSG_WIN_SIZE - 2 ) ;
 wdg_create_object ( & wdg_c2 , WDG_SCROLL , 0 ) ;
 snprintf ( title , MAX_ASCII_ADDR_LEN + 6 , "%s:%d" , ip_addr_ntoa ( & curr_conn -> L3_addr2 , tmp ) , ntohs ( curr_conn -> L4_addr2 ) ) ;
 wdg_set_title ( wdg_c2 , title , WDG_ALIGN_LEFT ) ;
 wdg_set_color ( wdg_c2 , WDG_COLOR_TITLE , EC_COLOR_TITLE ) ;
 wdg_set_color ( wdg_c2 , WDG_COLOR_FOCUS , EC_COLOR_FOCUS ) ;
 wdg_set_size ( wdg_c2 , current_screen . cols / 2 + 1 , 3 , - 2 , SYSMSG_WIN_SIZE - 2 ) ;
 wdg_scroll_set_lines ( wdg_c1 , GBL_CONF -> connection_buffer / ( current_screen . cols / 2 ) ) ;
 wdg_scroll_set_lines ( wdg_c2 , GBL_CONF -> connection_buffer / ( current_screen . cols / 2 ) ) ;
 wdg_compound_add ( wdg_conndata , wdg_c1 ) ;
 wdg_compound_add ( wdg_conndata , wdg_c2 ) ;
 wdg_add_destroy_key ( wdg_conndata , CTRL ( 'Q' ) , curses_destroy_conndata ) ;
 wdg_compound_add_callback ( wdg_conndata , 'j' , curses_connection_data_join ) ;
 wdg_compound_add_callback ( wdg_conndata , 'y' , curses_connection_inject ) ;
 wdg_compound_add_callback ( wdg_conndata , 'Y' , curses_connection_inject_file ) ;
 wdg_compound_add_callback ( wdg_conndata , 'k' , curses_connection_kill_wrapper ) ;
 wdg_compound_add_callback ( wdg_conndata , ' ' , curses_connection_data_help ) ;
 wdg_draw_object ( wdg_conndata ) ;
 wdg_set_focus ( wdg_conndata ) ;
 connbuf_print ( & curr_conn -> data , split_print ) ;
 conntrack_hook_conn_add ( curr_conn , split_print_po ) ;
 }