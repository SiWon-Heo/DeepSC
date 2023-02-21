static void gtkui_connection_purge ( void * conn ) {
 struct row_pairs * row , * nextrow , * list = connections ;
 ( void ) conn ;
 DEBUG_MSG ( "gtkui_connection_purge" ) ;
 connections = NULL ;
 for ( row = list ;
 row ;
 row = nextrow ) {
 nextrow = row -> next ;
 SAFE_FREE ( row ) ;
 }
 conntrack_purge ( ) ;
 gtk_list_store_clear ( GTK_LIST_STORE ( ls_conns ) ) ;
 }