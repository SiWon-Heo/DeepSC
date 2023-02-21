static void gtkui_connection_kill_curr_conn ( void ) {
 DEBUG_MSG ( "gtkui_connection_kill_curr_conn" ) ;
 switch ( user_kill ( curr_conn ) ) {
 case E_SUCCESS : curr_conn -> status = CONN_KILLED ;
 gtkui_message ( "The connection was killed !!" ) ;
 break ;
 case - E_FATAL : gtkui_message ( "Cannot kill UDP connections !!" ) ;
 break ;
 }
 }