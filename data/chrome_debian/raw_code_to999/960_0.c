static void set_connfilter_host ( GtkWidget * widget , gpointer * data ) {
 ( void ) data ;
 DEBUG_MSG ( "set_connfilter_host" ) ;
 filter . host = gtk_entry_get_text ( GTK_ENTRY ( widget ) ) ;
 gtk_tree_model_filter_refilter ( GTK_TREE_MODEL_FILTER ( filter . model ) ) ;
 }