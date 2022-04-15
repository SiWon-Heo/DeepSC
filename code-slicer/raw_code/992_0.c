static void curses_connection_purge ( void * conn ) {
 ( void ) conn ;
 DEBUG_MSG ( "curses_connection_purge" ) ;
 conntrack_purge ( ) ;
 refresh_connections ( ) ;
 }