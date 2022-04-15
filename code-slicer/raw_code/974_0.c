static void gtkui_connection_detail_destroy ( GtkWidget * widget , gpointer * data ) {
 ( void ) data ;
 if ( detail_timer1 ) g_source_remove ( detail_timer1 ) ;
 if ( detail_timer2 ) g_source_remove ( detail_timer2 ) ;
 gtk_widget_destroy ( widget ) ;
 }