static void gtkui_connection_data_detach ( GtkWidget * child ) {
 data_window = gtk_window_new ( GTK_WINDOW_TOPLEVEL ) ;
 gtk_window_set_title ( GTK_WINDOW ( data_window ) , "Connection data" ) ;
 gtk_window_set_default_size ( GTK_WINDOW ( data_window ) , 600 , 400 ) ;
 gtk_container_set_border_width ( GTK_CONTAINER ( data_window ) , 5 ) ;
 g_signal_connect ( G_OBJECT ( data_window ) , "delete_event" , G_CALLBACK ( gtkui_destroy_conndata ) , NULL ) ;
 gtkui_page_attach_shortcut ( data_window , gtkui_connection_data_attach ) ;
 gtk_container_add ( GTK_CONTAINER ( data_window ) , child ) ;
 gtk_window_present ( GTK_WINDOW ( data_window ) ) ;
 }