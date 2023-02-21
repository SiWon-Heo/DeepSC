void gtkui_connections_detach ( GtkWidget * child ) {
 conns_window = gtk_window_new ( GTK_WINDOW_TOPLEVEL ) ;
 gtk_window_set_title ( GTK_WINDOW ( conns_window ) , "Live connections" ) ;
 gtk_window_set_default_size ( GTK_WINDOW ( conns_window ) , 500 , 250 ) ;
 g_signal_connect ( G_OBJECT ( conns_window ) , "delete_event" , G_CALLBACK ( gtkui_kill_connections ) , NULL ) ;
 gtkui_page_attach_shortcut ( conns_window , gtkui_connections_attach ) ;
 gtk_container_add ( GTK_CONTAINER ( conns_window ) , child ) ;
 gtk_window_present ( GTK_WINDOW ( conns_window ) ) ;
 }