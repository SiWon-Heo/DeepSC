static void set_connfilter ( GtkWidget * widget , gpointer * data ) {
 gboolean * value ;
 DEBUG_MSG ( "set_connfilter" ) ;
 value = ( gboolean * ) data ;
 * value = gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON ( widget ) ) ;
 gtk_tree_model_filter_refilter ( GTK_TREE_MODEL_FILTER ( filter . model ) ) ;
 }