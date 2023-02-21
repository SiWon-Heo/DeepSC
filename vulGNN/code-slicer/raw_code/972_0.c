static gboolean gtkui_connections_scroll ( gpointer data ) {
 gint * type = data ;
 if ( type == NULL ) return FALSE ;
 if ( * type == 1 && textview1 && endmark1 && textview2 && endmark2 ) {
 gtk_text_view_scroll_to_mark ( GTK_TEXT_VIEW ( textview1 ) , endmark1 , 0 , FALSE , 0 , 0 ) ;
 gtk_text_view_scroll_to_mark ( GTK_TEXT_VIEW ( textview2 ) , endmark2 , 0 , FALSE , 0 , 0 ) ;
 }
 else if ( textview3 && endmark3 ) {
 gtk_text_view_scroll_to_mark ( GTK_TEXT_VIEW ( textview3 ) , endmark3 , 0 , FALSE , 0 , 0 ) ;
 }
 return ( FALSE ) ;
 }