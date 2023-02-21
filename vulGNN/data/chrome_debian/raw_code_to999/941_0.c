int wpa_sess_get ( u_char * sta , struct wpa_sa * sa ) {
 struct wpa_session * e ;
 pthread_mutex_lock ( & root_mutex ) ;
 LIST_FOREACH ( e , & wpa_sess_root , next ) {
 if ( ! memcmp ( & e -> sta , sta , ETH_ADDR_LEN ) ) {
 memcpy ( sa , & e -> sa , sizeof ( struct wpa_sa ) ) ;
 pthread_mutex_unlock ( & root_mutex ) ;
 return E_SUCCESS ;
 }
 }
 pthread_mutex_unlock ( & root_mutex ) ;
 return - E_NOTFOUND ;
 }