static void gtkui_data_print ( int buffer , char * data , int color ) {
 GtkTextIter iter ;
 GtkTextBuffer * textbuf = NULL ;
 GtkWidget * textview = NULL ;
 GtkTextMark * endmark = NULL ;
 char * unicode = NULL ;
 switch ( buffer ) {
 case 1 : textbuf = splitbuf1 ;
 textview = textview1 ;
 endmark = endmark1 ;
 break ;
 case 2 : textbuf = splitbuf2 ;
 textview = textview2 ;
 endmark = endmark2 ;
 break ;
 case 3 : textbuf = joinedbuf ;
 textview = textview3 ;
 endmark = endmark3 ;
 break ;
 default : return ;
 }
 unicode = gtkui_utf8_validate ( data ) ;
 if ( ! data_window || ! textbuf || ! textview || ! endmark || ! unicode ) return ;
 gtk_text_buffer_get_end_iter ( textbuf , & iter ) ;
 if ( color == 2 ) gtk_text_buffer_insert_with_tags_by_name ( textbuf , & iter , unicode , - 1 , "blue_fg" , "monospace" , NULL ) ;
 else gtk_text_buffer_insert_with_tags_by_name ( textbuf , & iter , unicode , - 1 , "monospace" , NULL ) ;
 gtk_text_view_scroll_to_mark ( GTK_TEXT_VIEW ( textview ) , endmark , 0 , FALSE , 0 , 0 ) ;
 }