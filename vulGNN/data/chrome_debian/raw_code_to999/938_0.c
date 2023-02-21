void wpa_sess_add ( u_char * sta , struct wpa_sa * sa ) {
 struct wpa_session * e , * s ;
 char tmp [ MAX_ASCII_ADDR_LEN ] ;
 SAFE_CALLOC ( e , 1 , sizeof ( struct wpa_session ) ) ;
 if ( sta ) memcpy ( & e -> sta , sta , ETH_ADDR_LEN ) ;
 if ( sa ) {
 gettimeofday ( & sa -> tv , NULL ) ;
 memcpy ( & e -> sa , sa , sizeof ( struct wpa_sa ) ) ;
 }
 pthread_mutex_lock ( & root_mutex ) ;
 LIST_FOREACH ( s , & wpa_sess_root , next ) {
 if ( ! memcmp ( & e -> sta , & s -> sta , ETH_ADDR_LEN ) ) {
 if ( sa ) {
 memcpy ( & s -> sa , sa , sizeof ( struct wpa_sa ) ) ;
 gettimeofday ( & s -> sa . tv , NULL ) ;
 }
 USER_MSG ( "WPA session updated for [%s]\n" , mac_addr_ntoa ( e -> sta , tmp ) ) ;
 pthread_mutex_unlock ( & root_mutex ) ;
 return ;
 }
 }
 LIST_INSERT_HEAD ( & wpa_sess_root , e , next ) ;
 pthread_mutex_unlock ( & root_mutex ) ;
 USER_MSG ( "New WPA session for [%s]\n" , mac_addr_ntoa ( e -> sta , tmp ) ) ;
 }