static gboolean connfilter ( GtkTreeModel * model , GtkTreeIter * iter , gpointer * data ) {
 gchar * src_host , * dst_host ;
 gboolean ret = TRUE ;
 struct conn_tail * conn = NULL ;
 ( void ) data ;
 gtk_tree_model_get ( model , iter , 1 , & src_host , 4 , & dst_host , 11 , & conn , - 1 ) ;
 if ( filter . host && strlen ( filter . host ) ) {
 if ( src_host && ! strcasestr ( src_host , filter . host ) && dst_host && ! strcasestr ( dst_host , filter . host ) ) {
 ret = FALSE ;
 g_free ( src_host ) ;
 g_free ( dst_host ) ;
 }
 }
 if ( conn && conn -> co ) {
 switch ( conn -> co -> L4_proto ) {
 case NL_TYPE_UDP : if ( ! filter . udp ) ret = FALSE ;
 break ;
 case NL_TYPE_TCP : if ( ! filter . tcp ) ret = FALSE ;
 break ;
 default : if ( ! filter . other ) ret = FALSE ;
 }
 switch ( conn -> co -> status ) {
 case CONN_IDLE : if ( ! filter . idle ) ret = FALSE ;
 break ;
 case CONN_ACTIVE : if ( ! filter . active ) ret = FALSE ;
 break ;
 case CONN_CLOSING : if ( ! filter . closing ) ret = FALSE ;
 break ;
 case CONN_CLOSED : if ( ! filter . closed ) ret = FALSE ;
 break ;
 case CONN_KILLED : if ( ! filter . killed ) ret = FALSE ;
 break ;
 default : break ;
 }
 }
 else {
 ret = FALSE ;
 }
 return ret ;
 }