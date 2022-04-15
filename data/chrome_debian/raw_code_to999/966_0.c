static void gtkui_connection_inject ( void ) {
 GtkWidget * dialog , * text , * label , * vbox , * frame , * content_area ;
 GtkWidget * button1 , * button2 , * hbox ;
 GtkTextBuffer * buf ;
 GtkTextIter start , end ;
 char tmp [ MAX_ASCII_ADDR_LEN ] ;
 gint response = 0 ;
 DEBUG_MSG ( "gtk_connection_inject" ) ;
 if ( curr_conn == NULL ) return ;
 dialog = gtk_dialog_new_with_buttons ( "Character Injection" , GTK_WINDOW ( window ) , GTK_DIALOG_MODAL , GTK_STOCK_CANCEL , GTK_RESPONSE_CANCEL , GTK_STOCK_OK , GTK_RESPONSE_OK , NULL ) ;
 # if ! GTK_CHECK_VERSION ( 2 , 22 , 0 ) gtk_dialog_set_has_separator ( GTK_DIALOG ( dialog ) , FALSE ) ;
 # endif gtk_container_set_border_width ( GTK_CONTAINER ( dialog ) , 5 ) ;
 content_area = gtk_dialog_get_content_area ( GTK_DIALOG ( dialog ) ) ;
 vbox = gtkui_box_new ( GTK_ORIENTATION_VERTICAL , 0 , FALSE ) ;
 gtk_box_pack_start ( GTK_BOX ( content_area ) , vbox , FALSE , FALSE , 0 ) ;
 label = gtk_label_new ( "Packet destination:" ) ;
 gtk_misc_set_alignment ( GTK_MISC ( label ) , 0 , 0.5 ) ;
 gtk_box_pack_start ( GTK_BOX ( vbox ) , label , FALSE , FALSE , 0 ) ;
 hbox = gtkui_box_new ( GTK_ORIENTATION_HORIZONTAL , 5 , FALSE ) ;
 gtk_box_pack_start ( GTK_BOX ( vbox ) , hbox , FALSE , FALSE , 0 ) ;
 button1 = gtk_radio_button_new_with_label ( NULL , ip_addr_ntoa ( & curr_conn -> L3_addr2 , tmp ) ) ;
 gtk_box_pack_start ( GTK_BOX ( hbox ) , button1 , FALSE , FALSE , 0 ) ;
 button2 = gtk_radio_button_new_with_label_from_widget ( GTK_RADIO_BUTTON ( button1 ) , ip_addr_ntoa ( & curr_conn -> L3_addr1 , tmp ) ) ;
 gtk_box_pack_start ( GTK_BOX ( hbox ) , button2 , FALSE , FALSE , 0 ) ;
 label = gtk_label_new ( "Characters to be injected:" ) ;
 gtk_misc_set_alignment ( GTK_MISC ( label ) , 0 , 0.5 ) ;
 gtk_box_pack_start ( GTK_BOX ( vbox ) , label , FALSE , FALSE , 0 ) ;
 frame = gtk_frame_new ( NULL ) ;
 gtk_frame_set_shadow_type ( GTK_FRAME ( frame ) , GTK_SHADOW_IN ) ;
 gtk_box_pack_start ( GTK_BOX ( vbox ) , frame , TRUE , TRUE , 5 ) ;
 text = gtk_text_view_new ( ) ;
 gtk_text_view_set_wrap_mode ( GTK_TEXT_VIEW ( text ) , GTK_WRAP_CHAR ) ;
 gtk_container_add ( GTK_CONTAINER ( frame ) , text ) ;
 gtk_widget_show_all ( dialog ) ;
 response = gtk_dialog_run ( GTK_DIALOG ( dialog ) ) ;
 if ( response == GTK_RESPONSE_OK ) {
 gtk_widget_hide ( dialog ) ;
 SAFE_REALLOC ( injectbuf , 501 * sizeof ( char ) ) ;
 memset ( injectbuf , 0 , 501 ) ;
 buf = gtk_text_view_get_buffer ( GTK_TEXT_VIEW ( text ) ) ;
 gtk_text_buffer_get_start_iter ( buf , & start ) ;
 gtk_text_buffer_get_start_iter ( buf , & end ) ;
 gtk_text_iter_forward_chars ( & end , 500 ) ;
 strncpy ( injectbuf , gtk_text_buffer_get_text ( buf , & start , & end , FALSE ) , 501 ) ;
 if ( gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON ( button1 ) ) ) gtkui_inject_user ( 1 ) ;
 else if ( gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON ( button2 ) ) ) gtkui_inject_user ( 2 ) ;
 }
 gtk_widget_destroy ( dialog ) ;
 }