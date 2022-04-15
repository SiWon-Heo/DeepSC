static void gtkui_connection_data ( void ) {
 GtkTreeIter iter ;
 GtkTreeModel * model ;
 struct conn_tail * c = NULL ;
 DEBUG_MSG ( "gtk_connection_data" ) ;
 model = GTK_TREE_MODEL ( ls_conns ) ;
 if ( gtk_tree_selection_get_selected ( GTK_TREE_SELECTION ( selection ) , & model , & iter ) ) {
 gtk_tree_model_get ( model , & iter , 11 , & c , - 1 ) ;
 }
 else return ;
 if ( c == NULL || c -> co == NULL ) return ;
 if ( curr_conn ) {
 conntrack_hook_conn_del ( curr_conn , split_print_po ) ;
 conntrack_hook_conn_del ( curr_conn , join_print_po ) ;
 curr_conn -> flags &= ~ CONN_VIEWING ;
 }
 curr_conn = c -> co ;
 curr_conn -> flags |= CONN_VIEWING ;
 gtkui_connection_data_split ( ) ;
 }