static void curses_connection_data ( void * conn ) {
 struct conn_tail * c = ( struct conn_tail * ) conn ;
 DEBUG_MSG ( "curses_connection_data" ) ;
 if ( curr_conn ) {
 conntrack_hook_conn_del ( curr_conn , split_print_po ) ;
 conntrack_hook_conn_del ( curr_conn , join_print_po ) ;
 curr_conn -> flags &= ~ CONN_VIEWING ;
 }
 curr_conn = c -> co ;
 curr_conn -> flags |= CONN_VIEWING ;
 curses_connection_data_split ( ) ;
 }