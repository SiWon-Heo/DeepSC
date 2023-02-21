static void curses_connection_help ( void * dummy ) {
 ( void ) dummy ;
 char help [ ] = "HELP: shortcut list:\n\n" " ENTER - open the data panel in real time\n" " d - show details of the current connection\n" " k - kill the connection\n" " x - purge the connection list" ;
 curses_message ( help ) ;
 }