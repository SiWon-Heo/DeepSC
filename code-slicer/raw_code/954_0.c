static void gtkui_destroy_conndata ( void ) {
 DEBUG_MSG ( "gtkui_destroy_conndata" ) ;
 if ( curr_conn ) {
 conntrack_hook_conn_del ( curr_conn , split_print_po ) ;
 conntrack_hook_conn_del ( curr_conn , join_print_po ) ;
 curr_conn -> flags &= ~ CONN_VIEWING ;
 curr_conn = NULL ;
 }
 gtk_widget_destroy ( data_window ) ;
 textview1 = NULL ;
 textview2 = NULL ;
 textview3 = NULL ;
 data_window = NULL ;
 }