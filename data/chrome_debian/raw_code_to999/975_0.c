static void gtkui_connection_data_join ( void ) {
 GtkWidget * hbox , * vbox , * label , * scrolled , * button , * child ;
 GtkTextIter iter ;
 # define TITLE_LEN ( MAX_ASCII_ADDR_LEN * 2 ) + 6 char src [ MAX_ASCII_ADDR_LEN ] ;
 char dst [ MAX_ASCII_ADDR_LEN ] ;
 char title [ TITLE_LEN ] ;
 static gint scroll_join = 2 ;
 DEBUG_MSG ( "gtk_connection_data_join" ) ;
 conntrack_hook_conn_del ( curr_conn , split_print_po ) ;
 if ( data_window ) {
 child = gtk_bin_get_child ( GTK_BIN ( data_window ) ) ;
 gtk_container_remove ( GTK_CONTAINER ( data_window ) , child ) ;
 textview1 = NULL ;
 textview2 = NULL ;
 splitbuf1 = NULL ;
 splitbuf2 = NULL ;
 endmark1 = NULL ;
 endmark2 = NULL ;
 }
 else {
 data_window = gtkui_page_new ( "Connection data" , & gtkui_destroy_conndata , & gtkui_connection_data_detach ) ;
 }
 curr_conn -> flags |= CONN_VIEWING ;
 vbox = gtkui_box_new ( GTK_ORIENTATION_VERTICAL , 0 , FALSE ) ;
 gtk_container_add ( GTK_CONTAINER ( data_window ) , vbox ) ;
 gtk_widget_show ( vbox ) ;
 snprintf ( title , TITLE_LEN , "%s:%d - %s:%d" , ip_addr_ntoa ( & curr_conn -> L3_addr1 , src ) , ntohs ( curr_conn -> L4_addr1 ) , ip_addr_ntoa ( & curr_conn -> L3_addr2 , dst ) , ntohs ( curr_conn -> L4_addr2 ) ) ;
 label = gtk_label_new ( title ) ;
 gtk_misc_set_alignment ( GTK_MISC ( label ) , 0 , 0.5 ) ;
 gtk_box_pack_start ( GTK_BOX ( vbox ) , label , FALSE , FALSE , 0 ) ;
 gtk_widget_show ( label ) ;
 scrolled = gtk_scrolled_window_new ( NULL , NULL ) ;
 gtk_scrolled_window_set_policy ( GTK_SCROLLED_WINDOW ( scrolled ) , GTK_POLICY_AUTOMATIC , GTK_POLICY_AUTOMATIC ) ;
 gtk_scrolled_window_set_shadow_type ( GTK_SCROLLED_WINDOW ( scrolled ) , GTK_SHADOW_IN ) ;
 gtk_box_pack_start ( GTK_BOX ( vbox ) , scrolled , TRUE , TRUE , 0 ) ;
 gtk_widget_show ( scrolled ) ;
 textview3 = gtk_text_view_new ( ) ;
 gtk_text_view_set_wrap_mode ( GTK_TEXT_VIEW ( textview3 ) , GTK_WRAP_CHAR ) ;
 gtk_text_view_set_editable ( GTK_TEXT_VIEW ( textview3 ) , FALSE ) ;
 gtk_text_view_set_cursor_visible ( GTK_TEXT_VIEW ( textview3 ) , FALSE ) ;
 gtk_text_view_set_right_margin ( GTK_TEXT_VIEW ( textview3 ) , 5 ) ;
 gtk_text_view_set_right_margin ( GTK_TEXT_VIEW ( textview3 ) , 5 ) ;
 gtk_container_add ( GTK_CONTAINER ( scrolled ) , textview3 ) ;
 gtk_widget_show ( textview3 ) ;
 joinedbuf = gtk_text_view_get_buffer ( GTK_TEXT_VIEW ( textview3 ) ) ;
 gtk_text_buffer_create_tag ( joinedbuf , "blue_fg" , "foreground" , "blue" , NULL ) ;
 gtk_text_buffer_create_tag ( joinedbuf , "monospace" , "family" , "monospace" , NULL ) ;
 gtk_text_buffer_get_end_iter ( joinedbuf , & iter ) ;
 endmark3 = gtk_text_buffer_create_mark ( joinedbuf , "end" , & iter , FALSE ) ;
 hbox = gtkui_box_new ( GTK_ORIENTATION_HORIZONTAL , 5 , TRUE ) ;
 gtk_box_pack_start ( GTK_BOX ( vbox ) , hbox , FALSE , FALSE , 0 ) ;
 gtk_widget_show ( hbox ) ;
 button = gtk_button_new_with_mnemonic ( "_Split View" ) ;
 g_signal_connect ( G_OBJECT ( button ) , "clicked" , G_CALLBACK ( gtkui_connection_data_split ) , NULL ) ;
 gtk_box_pack_start ( GTK_BOX ( hbox ) , button , TRUE , TRUE , 0 ) ;
 gtk_widget_show ( button ) ;
 button = gtk_button_new_with_mnemonic ( "_Kill Connection" ) ;
 g_signal_connect ( G_OBJECT ( button ) , "clicked" , G_CALLBACK ( gtkui_connection_kill_curr_conn ) , NULL ) ;
 gtk_box_pack_start ( GTK_BOX ( hbox ) , button , TRUE , TRUE , 0 ) ;
 gtk_widget_show ( button ) ;
 gtk_widget_show ( data_window ) ;
 if ( GTK_IS_WINDOW ( data_window ) ) gtk_window_present ( GTK_WINDOW ( data_window ) ) ;
 else gtkui_page_present ( data_window ) ;
 g_timeout_add ( 500 , gtkui_connections_scroll , & scroll_join ) ;
 connbuf_print ( & curr_conn -> data , join_print ) ;
 conntrack_hook_conn_add ( curr_conn , join_print_po ) ;
 }