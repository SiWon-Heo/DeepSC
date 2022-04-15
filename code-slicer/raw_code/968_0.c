static void gtkui_connection_kill ( void * conn ) {
 GtkTreeIter iter ;
 GtkTreeModel * model ;
 struct conn_tail * c = NULL ;
 ( void ) conn ;
 DEBUG_MSG ( "gtkui_connection_kill" ) ;
 model = GTK_TREE_MODEL ( ls_conns ) ;
 if ( gtk_tree_selection_get_selected ( GTK_TREE_SELECTION ( selection ) , & model , & iter ) ) {
 gtk_tree_model_get ( model , & iter , 11 , & c , - 1 ) ;
 }
 else return ;
 if ( ! c || ! c -> co ) return ;
 switch ( user_kill ( c -> co ) ) {
 case E_SUCCESS : c -> co -> status = CONN_KILLED ;
 gtkui_message ( "The connection was killed !!" ) ;
 break ;
 case - E_FATAL : gtkui_message ( "Cannot kill UDP connections !!" ) ;
 break ;
 }
 }