static void curses_connection_detail ( void * conn ) {
 struct conn_tail * c = ( struct conn_tail * ) conn ;
 char tmp [ MAX_ASCII_ADDR_LEN ] ;
 char * proto = "" ;
 char name [ MAX_HOSTNAME_LEN ] ;
 unsigned int row = 0 ;
 DEBUG_MSG ( "curses_connection_detail" ) ;
 if ( wdg_conn_detail ) {
 wdg_destroy_object ( & wdg_conn_detail ) ;
 wdg_conn_detail = NULL ;
 }
 wdg_create_object ( & wdg_conn_detail , WDG_WINDOW , WDG_OBJ_WANT_FOCUS ) ;
 wdg_set_title ( wdg_conn_detail , "Connection detail:" , WDG_ALIGN_LEFT ) ;
 wdg_set_size ( wdg_conn_detail , 1 , 2 , 75 , 23 ) ;
 wdg_set_color ( wdg_conn_detail , WDG_COLOR_SCREEN , EC_COLOR ) ;
 wdg_set_color ( wdg_conn_detail , WDG_COLOR_WINDOW , EC_COLOR ) ;
 wdg_set_color ( wdg_conn_detail , WDG_COLOR_BORDER , EC_COLOR_BORDER ) ;
 wdg_set_color ( wdg_conn_detail , WDG_COLOR_FOCUS , EC_COLOR_FOCUS ) ;
 wdg_set_color ( wdg_conn_detail , WDG_COLOR_TITLE , EC_COLOR_TITLE ) ;
 wdg_draw_object ( wdg_conn_detail ) ;
 wdg_set_focus ( wdg_conn_detail ) ;
 wdg_add_destroy_key ( wdg_conn_detail , CTRL ( 'Q' ) , NULL ) ;
 wdg_window_print ( wdg_conn_detail , 1 , ++ row , "Source MAC address : %s" , mac_addr_ntoa ( c -> co -> L2_addr1 , tmp ) ) ;
 wdg_window_print ( wdg_conn_detail , 1 , ++ row , "Destination MAC address : %s" , mac_addr_ntoa ( c -> co -> L2_addr2 , tmp ) ) ;
 ++ row ;
 wdg_window_print ( wdg_conn_detail , 1 , ++ row , "Source IP address : %s" , ip_addr_ntoa ( & ( c -> co -> L3_addr1 ) , tmp ) ) ;
 if ( host_iptoa ( & ( c -> co -> L3_addr1 ) , name ) == E_SUCCESS ) wdg_window_print ( wdg_conn_detail , 1 , ++ row , "Source hostname : %s" , name ) ;
 # ifdef WITH_GEOIP if ( GBL_CONF -> geoip_support_enable ) wdg_window_print ( wdg_conn_detail , 1 , ++ row , "Source location : %s" , geoip_country_by_ip ( & c -> co -> L3_addr1 ) ) ;
 # endif wdg_window_print ( wdg_conn_detail , 1 , ++ row , "Destination IP address : %s" , ip_addr_ntoa ( & ( c -> co -> L3_addr2 ) , tmp ) ) ;
 if ( host_iptoa ( & ( c -> co -> L3_addr2 ) , name ) == E_SUCCESS ) wdg_window_print ( wdg_conn_detail , 1 , ++ row , "Destination hostname : %s" , name ) ;
 # ifdef WITH_GEOIP if ( GBL_CONF -> geoip_support_enable ) wdg_window_print ( wdg_conn_detail , 1 , ++ row , "Destination location : %s" , geoip_country_by_ip ( & c -> co -> L3_addr2 ) ) ;
 # endif ++ row ;
 switch ( c -> co -> L4_proto ) {
 case NL_TYPE_UDP : proto = "UDP" ;
 break ;
 case NL_TYPE_TCP : proto = "TCP" ;
 break ;
 }
 wdg_window_print ( wdg_conn_detail , 1 , ++ row , "Protocol : %s" , proto ) ;
 wdg_window_print ( wdg_conn_detail , 1 , ++ row , "Source port : %-5d %s" , ntohs ( c -> co -> L4_addr1 ) , service_search ( c -> co -> L4_addr1 , c -> co -> L4_proto ) ) ;
 wdg_window_print ( wdg_conn_detail , 1 , ++ row , "Destination port : %-5d %s" , ntohs ( c -> co -> L4_addr2 ) , service_search ( c -> co -> L4_addr2 , c -> co -> L4_proto ) ) ;
 row ++ ;
 wdg_window_print ( wdg_conn_detail , 1 , ++ row , "--> %d <-- %d total: %d " , c -> co -> tx , c -> co -> rx , c -> co -> xferred ) ;
 row ++ ;
 if ( c -> co -> DISSECTOR . user ) {
 wdg_window_print ( wdg_conn_detail , 1 , ++ row , "Account : %s / %s" , c -> co -> DISSECTOR . user , c -> co -> DISSECTOR . pass ) ;
 if ( c -> co -> DISSECTOR . info ) wdg_window_print ( wdg_conn_detail , 1 , ++ row , "Additional Info : %s" , c -> co -> DISSECTOR . info ) ;
 }
 }