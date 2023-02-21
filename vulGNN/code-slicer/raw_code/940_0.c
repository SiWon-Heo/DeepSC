int wpa_decrypt ( u_char * mac , u_char * data , size_t len , struct wpa_sa sa ) {
 if ( sa . algo == WPA_KEY_CCMP ) {
 return wpa_ccmp_decrypt ( mac , data , len , sa ) ;
 }
 else if ( sa . algo == WPA_KEY_TKIP ) {
 return wpa_tkip_decrypt ( mac , data , len , sa ) ;
 }
 return - E_NOTHANDLED ;
 }