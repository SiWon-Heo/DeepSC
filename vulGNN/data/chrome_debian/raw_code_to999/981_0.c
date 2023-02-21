static void gtkui_kill_connections ( void ) {
 DEBUG_MSG ( "gtk_kill_connections" ) ;
 g_source_remove ( connections_idle ) ;
 gtk_widget_destroy ( conns_window ) ;
 conns_window = NULL ;
 }