static void gtkui_connection_data_attach ( void ) {
 if ( curr_conn ) {
 conntrack_hook_conn_del ( curr_conn , split_print_po ) ;
 conntrack_hook_conn_del ( curr_conn , join_print_po ) ;
 }
 gtk_widget_destroy ( data_window ) ;
 textview1 = NULL ;
 textview2 = NULL ;
 textview3 = NULL ;
 data_window = NULL ;
 gtkui_connection_data_split ( ) ;
 }