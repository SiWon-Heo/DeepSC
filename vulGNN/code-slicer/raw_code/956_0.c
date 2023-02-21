static gboolean refresh_connections ( gpointer data ) {
 struct row_pairs * lastconn = NULL , * cache = NULL ;
 GtkTreeModel * model = GTK_TREE_MODEL ( ls_conns ) ;
 void * list , * next , * listend ;
 struct conn_object * conn ;
 GtkTreeIter iter ;
 char flags [ 2 ] , status [ 8 ] ;
 unsigned int tx = 0 , rx = 0 ;
 struct row_pairs * row = NULL , * nextrow = NULL , top , bottom ;
 ( void ) data ;
 memset ( & flags , 0 , sizeof ( flags ) ) ;
 memset ( & status , 0 , sizeof ( status ) ) ;
 if ( ls_conns ) {
 if ( ! gtk_widget_get_visible ( conns_window ) ) return ( FALSE ) ;
 }
 else {
 ls_conns = gtk_list_store_new ( 12 , G_TYPE_STRING , G_TYPE_STRING , G_TYPE_UINT , G_TYPE_STRING , G_TYPE_STRING , G_TYPE_UINT , G_TYPE_STRING , G_TYPE_STRING , G_TYPE_UINT , G_TYPE_UINT , G_TYPE_STRING , G_TYPE_POINTER ) ;
 connections = NULL ;
 }
 for ( row = connections ;
 row ;
 row = nextrow ) {
 nextrow = row -> next ;
 if ( conntrack_get ( 0 , row -> conn , NULL ) == NULL ) {
 gtk_list_store_remove ( GTK_LIST_STORE ( ls_conns ) , & row -> iter ) ;
 if ( row -> next ) row -> next -> prev = row -> prev ;
 if ( row -> prev ) row -> prev -> next = row -> next ;
 else connections = row -> next ;
 SAFE_FREE ( row ) ;
 }
 if ( row ) lastconn = row ;
 }
 if ( ! lastconn ) {
 listend = conntrack_get ( 0 , NULL , NULL ) ;
 if ( listend == NULL ) return ( TRUE ) ;
 }
 else {
 listend = lastconn -> conn ;
 }
 for ( list = conntrack_get ( + 1 , listend , NULL ) ;
 list ;
 list = next ) {
 next = conntrack_get ( + 1 , list , & conn ) ;
 cache = gtkui_connections_add ( conn , list , & connections ) ;
 if ( cache ) lastconn = cache ;
 }
 gtkui_connection_list_row ( 1 , & top ) ;
 gtkui_connection_list_row ( 0 , & bottom ) ;
 if ( top . conn == NULL ) return ( TRUE ) ;
 iter = top . iter ;
 do {
 gtk_tree_model_get ( model , & iter , 11 , & list , - 1 ) ;
 conntrack_get ( 0 , list , & conn ) ;
 conntrack_flagstr ( conn , flags , sizeof ( flags ) ) ;
 conntrack_statusstr ( conn , status , sizeof ( status ) ) ;
 tx = conn -> tx ;
 rx = conn -> rx ;
 gtk_list_store_set ( ls_conns , & iter , 0 , flags , 7 , status , 8 , tx , 9 , rx , - 1 ) ;
 if ( bottom . conn == list ) break ;
 }
 while ( gtk_tree_model_iter_next ( model , & iter ) ) ;
 gtk_tree_model_filter_refilter ( GTK_TREE_MODEL_FILTER ( filter . model ) ) ;
 return ( TRUE ) ;
 }