static void gtkui_connection_list_row ( int top , struct row_pairs * pair ) {
 GtkTreeIter iter ;
 GtkTreePath * path = NULL ;
 GtkTreeModel * model = NULL ;
 GdkRectangle rect ;
 int wx = 0 , wy = 0 ;
 void * row = NULL ;
 if ( ! ls_conns || ! pair ) return ;
 pair -> conn = NULL ;
 model = GTK_TREE_MODEL ( ls_conns ) ;
 if ( gtk_tree_model_get_iter_first ( model , & iter ) ) {
 gtk_tree_view_get_visible_rect ( GTK_TREE_VIEW ( treeview ) , & rect ) ;
 gtk_tree_view_convert_bin_window_to_widget_coords ( GTK_TREE_VIEW ( treeview ) , rect . x , ( top ) ? rect . y : rect . height , & wx , & wy ) ;
 path = gtk_tree_path_new ( ) ;
 if ( gtk_tree_view_get_path_at_pos ( GTK_TREE_VIEW ( treeview ) , wx + 2 , ( top ) ? wy + 2 : wy - 2 , & path , NULL , NULL , NULL ) ) {
 gtk_tree_model_get_iter ( model , & iter , path ) ;
 gtk_tree_model_get ( model , & iter , 11 , & row , - 1 ) ;
 pair -> iter = iter ;
 pair -> conn = row ;
 }
 if ( path ) gtk_tree_path_free ( path ) ;
 }
 return ;
 }