static void curses_connection_kill_wrapper ( void ) {
 struct conn_tail c ;
 DEBUG_MSG ( "curses_connection_kill_wrapper" ) ;
 c . co = curr_conn ;
 curses_connection_kill ( & c ) ;
 }