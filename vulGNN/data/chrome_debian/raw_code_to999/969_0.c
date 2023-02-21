static void gtkui_connection_inject_file ( void ) {
 GtkWidget * dialog , * label , * vbox , * hbox , * content_area ;
 GtkWidget * button1 , * button2 , * button , * entry ;
 char tmp [ MAX_ASCII_ADDR_LEN ] ;
 const char * filename = NULL ;
 gint response = 0 ;
 DEBUG_MSG ( "gtk_connection_inject_file" ) ;
 if ( curr_conn == NULL ) return ;
 dialog = gtk_dialog_new_with_buttons ( "Character Injection" , GTK_WINDOW ( window ) , GTK_DIALOG_MODAL , GTK_STOCK_CANCEL , GTK_RESPONSE_CANCEL , GTK_STOCK_OK , GTK_RESPONSE_OK , NULL ) ;
 gtk_window_set_default_size ( GTK_WINDOW ( dialog ) , 400 , 150 ) ;
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
 gtk_widget_show ( button1 ) ;
 button2 = gtk_radio_button_new_with_label_from_widget ( GTK_RADIO_BUTTON ( button1 ) , ip_addr_ntoa ( & curr_conn -> L3_addr1 , tmp ) ) ;
 gtk_box_pack_start ( GTK_BOX ( hbox ) , button2 , FALSE , FALSE , 0 ) ;
 label = gtk_label_new ( "File to inject:" ) ;
 gtk_misc_set_alignment ( GTK_MISC ( label ) , 0 , 0.5 ) ;
 gtk_box_pack_start ( GTK_BOX ( vbox ) , label , FALSE , FALSE , 0 ) ;
 hbox = gtkui_box_new ( GTK_ORIENTATION_HORIZONTAL , 5 , FALSE ) ;
 gtk_box_pack_start ( GTK_BOX ( vbox ) , hbox , FALSE , FALSE , 0 ) ;
 entry = gtk_entry_new ( ) ;
 gtk_box_pack_start ( GTK_BOX ( hbox ) , entry , TRUE , TRUE , 0 ) ;
 button = gtk_button_new_with_label ( "..." ) ;
 gtk_box_pack_start ( GTK_BOX ( hbox ) , button , FALSE , FALSE , 0 ) ;
 g_signal_connect ( G_OBJECT ( button ) , "clicked" , G_CALLBACK ( gtkui_filename_browse ) , entry ) ;
 gtk_widget_show_all ( dialog ) ;
 response = gtk_dialog_run ( GTK_DIALOG ( dialog ) ) ;
 if ( response == GTK_RESPONSE_OK ) {
 gtk_widget_hide ( dialog ) ;
 filename = gtk_entry_get_text ( GTK_ENTRY ( entry ) ) ;
 if ( filename && strlen ( filename ) > 0 ) {
 if ( gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON ( button1 ) ) ) gtkui_inject_file ( filename , 1 ) ;
 else if ( gtk_toggle_button_get_active ( GTK_TOGGLE_BUTTON ( button2 ) ) ) gtkui_inject_file ( filename , 2 ) ;
 }
 }
 gtk_widget_destroy ( dialog ) ;
 }