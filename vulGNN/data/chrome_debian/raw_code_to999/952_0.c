static void gtkui_connection_detail ( void ) {
 GtkWidget * dwindow , * vbox , * hbox , * table , * label , * button ;
 GtkTreeIter iter ;
 GtkTreeModel * model ;
 struct conn_tail * c = NULL ;
 char tmp [ MAX_ASCII_ADDR_LEN ] ;
 char name [ MAX_HOSTNAME_LEN ] ;
 gchar * str , * markup ;
 guint nrows = 14 , ncols = 3 , row = 0 , col = 0 ;
 DEBUG_MSG ( "gtk_connection_detail" ) ;
 model = GTK_TREE_MODEL ( ls_conns ) ;
 if ( gtk_tree_selection_get_selected ( GTK_TREE_SELECTION ( selection ) , & model , & iter ) ) {
 gtk_tree_model_get ( model , & iter , 11 , & c , - 1 ) ;
 }
 else return ;
 if ( ! c || ! c -> co ) return ;
 dwindow = gtk_window_new ( GTK_WINDOW_TOPLEVEL ) ;
 gtk_window_set_title ( GTK_WINDOW ( dwindow ) , "Connection Details" ) ;
 gtk_window_set_modal ( GTK_WINDOW ( dwindow ) , TRUE ) ;
 gtk_window_set_transient_for ( GTK_WINDOW ( dwindow ) , GTK_WINDOW ( window ) ) ;
 gtk_window_set_position ( GTK_WINDOW ( dwindow ) , GTK_WIN_POS_CENTER_ON_PARENT ) ;
 gtk_container_set_border_width ( GTK_CONTAINER ( dwindow ) , 5 ) ;
 g_signal_connect ( G_OBJECT ( dwindow ) , "delete-event" , G_CALLBACK ( gtkui_connection_detail_destroy ) , NULL ) ;
 vbox = gtkui_box_new ( GTK_ORIENTATION_VERTICAL , 5 , FALSE ) ;
 gtk_container_add ( GTK_CONTAINER ( dwindow ) , vbox ) ;
 table = gtk_table_new ( nrows , ncols , FALSE ) ;
 gtk_table_set_row_spacings ( GTK_TABLE ( table ) , 5 ) ;
 gtk_table_set_col_spacings ( GTK_TABLE ( table ) , 5 ) ;
 gtk_container_set_border_width ( GTK_CONTAINER ( table ) , 8 ) ;
 gtk_box_pack_start ( GTK_BOX ( vbox ) , table , FALSE , FALSE , 0 ) ;
 label = gtk_label_new ( "Layer 2 Information:" ) ;
 markup = g_markup_printf_escaped ( "<span weight=\"bold\">%s</span>" , gtk_label_get_text ( GTK_LABEL ( label ) ) ) ;
 gtk_label_set_markup ( GTK_LABEL ( label ) , markup ) ;
 gtk_misc_set_alignment ( GTK_MISC ( label ) , 0 , 0.5 ) ;
 gtk_table_attach ( GTK_TABLE ( table ) , label , col , col + 3 , row , row + 1 , GTK_FILL , GTK_FILL , 0 , 0 ) ;
 g_free ( markup ) ;
 row ++ ;
 label = gtk_label_new ( "Source MAC address:" ) ;
 gtk_misc_set_alignment ( GTK_MISC ( label ) , 0 , 0.5 ) ;
 gtk_table_attach ( GTK_TABLE ( table ) , label , col , col + 1 , row , row + 1 , GTK_FILL , GTK_FILL , 0 , 0 ) ;
 label = gtk_label_new ( mac_addr_ntoa ( c -> co -> L2_addr1 , tmp ) ) ;
 gtk_label_set_selectable ( GTK_LABEL ( label ) , TRUE ) ;
 gtk_misc_set_alignment ( GTK_MISC ( label ) , 0 , 0.5 ) ;
 gtk_table_attach_defaults ( GTK_TABLE ( table ) , label , col + 1 , col + 3 , row , row + 1 ) ;
 row ++ ;
 label = gtk_label_new ( "Destination MAC address:" ) ;
 gtk_misc_set_alignment ( GTK_MISC ( label ) , 0 , 0.5 ) ;
 gtk_table_attach ( GTK_TABLE ( table ) , label , col , col + 1 , row , row + 1 , GTK_FILL , GTK_FILL , 0 , 0 ) ;
 label = gtk_label_new ( mac_addr_ntoa ( c -> co -> L2_addr2 , tmp ) ) ;
 gtk_label_set_selectable ( GTK_LABEL ( label ) , TRUE ) ;
 gtk_misc_set_alignment ( GTK_MISC ( label ) , 0 , 0.5 ) ;
 gtk_table_attach_defaults ( GTK_TABLE ( table ) , label , col + 1 , col + 3 , row , row + 1 ) ;
 row ++ ;
 label = gtk_label_new ( "Layer 3 Information:" ) ;
 markup = g_markup_printf_escaped ( "<span weight=\"bold\">%s</span>" , gtk_label_get_text ( GTK_LABEL ( label ) ) ) ;
 gtk_label_set_markup ( GTK_LABEL ( label ) , markup ) ;
 gtk_misc_set_alignment ( GTK_MISC ( label ) , 0 , 0.5 ) ;
 gtk_table_attach ( GTK_TABLE ( table ) , label , col , col + 3 , row , row + 1 , GTK_FILL , GTK_FILL , 0 , 0 ) ;
 gtk_table_set_row_spacing ( GTK_TABLE ( table ) , row - 1 , 10 ) ;
 g_free ( markup ) ;
 row ++ ;
 label = gtk_label_new ( "Source IP address:" ) ;
 gtk_misc_set_alignment ( GTK_MISC ( label ) , 0 , 0.5 ) ;
 gtk_table_attach ( GTK_TABLE ( table ) , label , col , col + 1 , row , row + 1 , GTK_FILL , GTK_FILL , 0 , 0 ) ;
 label = gtk_label_new ( ip_addr_ntoa ( & c -> co -> L3_addr1 , tmp ) ) ;
 gtk_label_set_selectable ( GTK_LABEL ( label ) , TRUE ) ;
 gtk_misc_set_alignment ( GTK_MISC ( label ) , 0 , 0.5 ) ;
 gtk_table_attach_defaults ( GTK_TABLE ( table ) , label , col + 1 , col + 3 , row , row + 1 ) ;
 if ( GBL_OPTIONS -> resolve ) {
 row ++ ;
 label = gtk_label_new ( "Source hostname:" ) ;
 gtk_misc_set_alignment ( GTK_MISC ( label ) , 0 , 0.5 ) ;
 gtk_table_attach ( GTK_TABLE ( table ) , label , col , col + 1 , row , row + 1 , GTK_FILL , GTK_FILL , 0 , 0 ) ;
 label = gtk_label_new ( "resolving..." ) ;
 if ( host_iptoa ( & c -> co -> L3_addr1 , name ) == - E_NOMATCH ) {
 struct resolv_object * ro ;
 SAFE_CALLOC ( ro , 1 , sizeof ( struct resolv_object ) ) ;
 ro -> type = GTK_TYPE_LABEL ;
 ro -> widget = label ;
 ro -> ip = & c -> co -> L3_addr1 ;
 detail_timer1 = g_timeout_add ( 1000 , gtkui_iptoa_deferred , ro ) ;
 }
 else {
 gtk_label_set_text ( GTK_LABEL ( label ) , name ) ;
 }
 gtk_label_set_selectable ( GTK_LABEL ( label ) , TRUE ) ;
 gtk_misc_set_alignment ( GTK_MISC ( label ) , 0 , 0.5 ) ;
 gtk_table_attach_defaults ( GTK_TABLE ( table ) , label , col + 1 , col + 3 , row , row + 1 ) ;
 }
 # ifdef WITH_GEOIP if ( GBL_CONF -> geoip_support_enable ) {
 row ++ ;
 label = gtk_label_new ( "Source location:" ) ;
 gtk_misc_set_alignment ( GTK_MISC ( label ) , 0 , 0.5 ) ;
 gtk_table_attach ( GTK_TABLE ( table ) , label , col , col + 1 , row , row + 1 , GTK_FILL , GTK_FILL , 0 , 0 ) ;
 label = gtk_label_new ( geoip_country_by_ip ( & c -> co -> L3_addr1 ) ) ;
 gtk_label_set_selectable ( GTK_LABEL ( label ) , TRUE ) ;
 gtk_misc_set_alignment ( GTK_MISC ( label ) , 0 , 0.5 ) ;
 gtk_table_attach_defaults ( GTK_TABLE ( table ) , label , col + 1 , col + 3 , row , row + 1 ) ;
 }
 # endif row ++ ;
 label = gtk_label_new ( "Destination IP address:" ) ;
 gtk_misc_set_alignment ( GTK_MISC ( label ) , 0 , 0.5 ) ;
 gtk_table_attach ( GTK_TABLE ( table ) , label , col , col + 1 , row , row + 1 , GTK_FILL , GTK_FILL , 0 , 0 ) ;
 label = gtk_label_new ( ip_addr_ntoa ( & c -> co -> L3_addr2 , tmp ) ) ;
 gtk_label_set_selectable ( GTK_LABEL ( label ) , TRUE ) ;
 gtk_misc_set_alignment ( GTK_MISC ( label ) , 0 , 0.5 ) ;
 gtk_table_attach_defaults ( GTK_TABLE ( table ) , label , col + 1 , col + 3 , row , row + 1 ) ;
 if ( GBL_OPTIONS -> resolve ) {
 row ++ ;
 label = gtk_label_new ( "Destination hostname:" ) ;
 gtk_misc_set_alignment ( GTK_MISC ( label ) , 0 , 0.5 ) ;
 gtk_table_attach ( GTK_TABLE ( table ) , label , col , col + 1 , row , row + 1 , GTK_FILL , GTK_FILL , 0 , 0 ) ;
 label = gtk_label_new ( "resolving..." ) ;
 if ( host_iptoa ( & c -> co -> L3_addr2 , name ) == - E_NOMATCH ) {
 struct resolv_object * ro ;
 SAFE_CALLOC ( ro , 1 , sizeof ( struct resolv_object ) ) ;
 ro -> type = GTK_TYPE_LABEL ;
 ro -> widget = label ;
 ro -> ip = & c -> co -> L3_addr2 ;
 detail_timer2 = g_timeout_add ( 1000 , gtkui_iptoa_deferred , ro ) ;
 }
 else {
 gtk_label_set_text ( GTK_LABEL ( label ) , name ) ;
 }
 gtk_label_set_selectable ( GTK_LABEL ( label ) , TRUE ) ;
 gtk_misc_set_alignment ( GTK_MISC ( label ) , 0 , 0.5 ) ;
 gtk_table_attach_defaults ( GTK_TABLE ( table ) , label , col + 1 , col + 3 , row , row + 1 ) ;
 }
 # ifdef WITH_GEOIP if ( GBL_CONF -> geoip_support_enable ) {
 row ++ ;
 label = gtk_label_new ( "Destination location:" ) ;
 gtk_misc_set_alignment ( GTK_MISC ( label ) , 0 , 0.5 ) ;
 gtk_table_attach ( GTK_TABLE ( table ) , label , col , col + 1 , row , row + 1 , GTK_FILL , GTK_FILL , 0 , 0 ) ;
 label = gtk_label_new ( geoip_country_by_ip ( & c -> co -> L3_addr2 ) ) ;
 gtk_label_set_selectable ( GTK_LABEL ( label ) , TRUE ) ;
 gtk_misc_set_alignment ( GTK_MISC ( label ) , 0 , 0.5 ) ;
 gtk_table_attach_defaults ( GTK_TABLE ( table ) , label , col + 1 , col + 3 , row , row + 1 ) ;
 }
 # endif row ++ ;
 label = gtk_label_new ( "Layer 4 Information:" ) ;
 markup = g_markup_printf_escaped ( "<span weight=\"bold\">%s</span>" , gtk_label_get_text ( GTK_LABEL ( label ) ) ) ;
 gtk_label_set_markup ( GTK_LABEL ( label ) , markup ) ;
 gtk_misc_set_alignment ( GTK_MISC ( label ) , 0 , 0.5 ) ;
 gtk_table_attach ( GTK_TABLE ( table ) , label , col , col + 3 , row , row + 1 , GTK_FILL , GTK_FILL , 0 , 0 ) ;
 gtk_table_set_row_spacing ( GTK_TABLE ( table ) , row - 1 , 10 ) ;
 g_free ( markup ) ;
 row ++ ;
 label = gtk_label_new ( "Protocol:" ) ;
 gtk_misc_set_alignment ( GTK_MISC ( label ) , 0 , 0.5 ) ;
 gtk_table_attach ( GTK_TABLE ( table ) , label , col , col + 1 , row , row + 1 , GTK_FILL , GTK_FILL , 0 , 0 ) ;
 switch ( c -> co -> L4_proto ) {
 case NL_TYPE_UDP : label = gtk_label_new ( "UDP" ) ;
 break ;
 case NL_TYPE_TCP : label = gtk_label_new ( "TCP" ) ;
 break ;
 default : label = gtk_label_new ( "" ) ;
 break ;
 }
 gtk_label_set_selectable ( GTK_LABEL ( label ) , TRUE ) ;
 gtk_misc_set_alignment ( GTK_MISC ( label ) , 0 , 0.5 ) ;
 gtk_table_attach_defaults ( GTK_TABLE ( table ) , label , col + 1 , col + 3 , row , row + 1 ) ;
 row ++ ;
 label = gtk_label_new ( "Source port:" ) ;
 gtk_misc_set_alignment ( GTK_MISC ( label ) , 0 , 0.5 ) ;
 gtk_table_attach ( GTK_TABLE ( table ) , label , col , col + 1 , row , row + 1 , GTK_FILL , GTK_FILL , 0 , 0 ) ;
 label = gtk_label_new ( ( str = g_strdup_printf ( "%d" , ntohs ( c -> co -> L4_addr1 ) ) ) ) ;
 gtk_label_set_selectable ( GTK_LABEL ( label ) , TRUE ) ;
 gtk_misc_set_alignment ( GTK_MISC ( label ) , 0 , 0.5 ) ;
 gtk_table_attach_defaults ( GTK_TABLE ( table ) , label , col + 1 , col + 2 , row , row + 1 ) ;
 g_free ( str ) ;
 label = gtk_label_new ( service_search ( c -> co -> L4_addr1 , c -> co -> L4_proto ) ) ;
 gtk_label_set_selectable ( GTK_LABEL ( label ) , TRUE ) ;
 gtk_misc_set_alignment ( GTK_MISC ( label ) , 0 , 0.5 ) ;
 gtk_table_attach_defaults ( GTK_TABLE ( table ) , label , col + 2 , col + 3 , row , row + 1 ) ;
 row ++ ;
 label = gtk_label_new ( "Destination port:" ) ;
 gtk_misc_set_alignment ( GTK_MISC ( label ) , 0 , 0.5 ) ;
 gtk_table_attach ( GTK_TABLE ( table ) , label , col , col + 1 , row , row + 1 , GTK_FILL , GTK_FILL , 0 , 0 ) ;
 label = gtk_label_new ( ( str = g_strdup_printf ( "%d" , ntohs ( c -> co -> L4_addr2 ) ) ) ) ;
 gtk_label_set_selectable ( GTK_LABEL ( label ) , TRUE ) ;
 gtk_misc_set_alignment ( GTK_MISC ( label ) , 0 , 0.5 ) ;
 gtk_table_attach_defaults ( GTK_TABLE ( table ) , label , col + 1 , col + 2 , row , row + 1 ) ;
 g_free ( str ) ;
 label = gtk_label_new ( service_search ( c -> co -> L4_addr2 , c -> co -> L4_proto ) ) ;
 gtk_label_set_selectable ( GTK_LABEL ( label ) , TRUE ) ;
 gtk_misc_set_alignment ( GTK_MISC ( label ) , 0 , 0.5 ) ;
 gtk_table_attach_defaults ( GTK_TABLE ( table ) , label , col + 2 , col + 3 , row , row + 1 ) ;
 row ++ ;
 label = gtk_label_new ( "Transferred bytes:" ) ;
 gtk_misc_set_alignment ( GTK_MISC ( label ) , 0 , 0.5 ) ;
 gtk_table_attach ( GTK_TABLE ( table ) , label , col , col + 1 , row , row + 1 , GTK_FILL , GTK_FILL , 0 , 0 ) ;
 label = gtk_label_new ( ( str = g_strdup_printf ( "%d" , c -> co -> xferred ) ) ) ;
 gtk_label_set_selectable ( GTK_LABEL ( label ) , TRUE ) ;
 gtk_misc_set_alignment ( GTK_MISC ( label ) , 0 , 0.5 ) ;
 gtk_table_attach_defaults ( GTK_TABLE ( table ) , label , col + 1 , col + 3 , row , row + 1 ) ;
 g_free ( str ) ;
 if ( c -> co -> DISSECTOR . user ) {
 row ++ ;
 label = gtk_label_new ( "Additional Information:" ) ;
 markup = g_markup_printf_escaped ( "<span weight=\"bold\">%s</span>" , gtk_label_get_text ( GTK_LABEL ( label ) ) ) ;
 gtk_label_set_markup ( GTK_LABEL ( label ) , markup ) ;
 gtk_misc_set_alignment ( GTK_MISC ( label ) , 0 , 0.5 ) ;
 gtk_table_attach ( GTK_TABLE ( table ) , label , col , col + 3 , row , row + 1 , GTK_FILL , GTK_FILL , 0 , 0 ) ;
 gtk_table_set_row_spacing ( GTK_TABLE ( table ) , row - 1 , 10 ) ;
 g_free ( markup ) ;
 row ++ ;
 label = gtk_label_new ( "Account:" ) ;
 gtk_misc_set_alignment ( GTK_MISC ( label ) , 0 , 0.5 ) ;
 gtk_table_attach ( GTK_TABLE ( table ) , label , col , col + 1 , row , row + 1 , GTK_FILL , GTK_FILL , 0 , 0 ) ;
 label = gtk_label_new ( c -> co -> DISSECTOR . user ) ;
 gtk_label_set_selectable ( GTK_LABEL ( label ) , TRUE ) ;
 gtk_misc_set_alignment ( GTK_MISC ( label ) , 0 , 0.5 ) ;
 gtk_table_attach_defaults ( GTK_TABLE ( table ) , label , col + 1 , col + 2 , row , row + 1 ) ;
 label = gtk_label_new ( c -> co -> DISSECTOR . pass ) ;
 gtk_label_set_selectable ( GTK_LABEL ( label ) , TRUE ) ;
 gtk_misc_set_alignment ( GTK_MISC ( label ) , 0 , 0.5 ) ;
 gtk_table_attach_defaults ( GTK_TABLE ( table ) , label , col + 2 , col + 3 , row , row + 1 ) ;
 if ( c -> co -> DISSECTOR . info ) {
 label = gtk_label_new ( "Additional info:" ) ;
 gtk_misc_set_alignment ( GTK_MISC ( label ) , 0 , 0.5 ) ;
 gtk_table_attach ( GTK_TABLE ( table ) , label , col , col + 1 , row , row + 1 , GTK_FILL , GTK_FILL , 0 , 0 ) ;
 label = gtk_label_new ( c -> co -> DISSECTOR . info ) ;
 gtk_label_set_selectable ( GTK_LABEL ( label ) , TRUE ) ;
 gtk_misc_set_alignment ( GTK_MISC ( label ) , 0 , 0.5 ) ;
 gtk_table_attach_defaults ( GTK_TABLE ( table ) , label , col + 1 , col + 3 , row , row + 1 ) ;
 }
 }
 gtk_table_resize ( GTK_TABLE ( table ) , row , ncols ) ;
 hbox = gtkui_box_new ( GTK_ORIENTATION_HORIZONTAL , 0 , FALSE ) ;
 gtk_box_pack_start ( GTK_BOX ( vbox ) , hbox , FALSE , FALSE , 0 ) ;
 button = gtk_button_new_from_stock ( GTK_STOCK_CLOSE ) ;
 g_signal_connect_swapped ( G_OBJECT ( button ) , "clicked" , G_CALLBACK ( gtkui_connection_detail_destroy ) , dwindow ) ;
 gtk_box_pack_end ( GTK_BOX ( hbox ) , button , FALSE , FALSE , 0 ) ;
 gtk_widget_grab_focus ( button ) ;
 gtk_widget_show_all ( dwindow ) ;
 }