void wpa_sess_del ( u_char * sta ) {
 struct wpa_session * e , * tmp ;
 char tmac [ MAX_ASCII_ADDR_LEN ] ;
 pthread_mutex_lock ( & root_mutex ) ;
 LIST_FOREACH_SAFE ( e , & wpa_sess_root , next , tmp ) {
 if ( ! memcmp ( & e -> sta , sta , ETH_ADDR_LEN ) ) {
 LIST_REMOVE ( e , next ) ;
 USER_MSG ( "WPA session deleted for [%s]\n" , mac_addr_ntoa ( e -> sta , tmac ) ) ;
 SAFE_FREE ( e ) ;
 break ;
 }
 }
 pthread_mutex_unlock ( & root_mutex ) ;
 }