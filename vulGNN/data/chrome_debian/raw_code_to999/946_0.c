int wifi_key_prepare ( char * key_string ) {
 int status = - E_INVALID ;
 char * ks ;
 char * p ;
 if ( key_string == NULL ) return - E_INVALID ;
 ks = strdup ( key_string ) ;
 if ( ( p = strchr ( ks , ':' ) ) != NULL ) * p = 0 ;
 if ( ! strcasecmp ( ks , "wep" ) ) {
 GBL_WIFI -> wifi_schema = WIFI_WEP ;
 status = set_wep_key ( p + 1 ) ;
 }
 if ( ! strcasecmp ( ks , "wpa" ) ) {
 GBL_WIFI -> wifi_schema = WIFI_WPA ;
 status = set_wpa_key ( p + 1 ) ;
 }
 SAFE_FREE ( ks ) ;
 return status ;
 }