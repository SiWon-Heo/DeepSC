static void curses_connection_kill ( void * conn ) {
 struct conn_tail * c = ( struct conn_tail * ) conn ;
 DEBUG_MSG ( "curses_connection_kill" ) ;
 switch ( user_kill ( c -> co ) ) {
 case E_SUCCESS : c -> co -> status = CONN_KILLED ;
 curses_message ( "The connection was killed !!" ) ;
 break ;
 case - E_FATAL : curses_message ( "Cannot kill UDP connections !!" ) ;
 break ;
 }
 }