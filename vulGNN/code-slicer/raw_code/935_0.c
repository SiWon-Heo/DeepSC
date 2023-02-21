static int set_wpa_key ( char * string ) {
 char * p ;
 char * pass ;
 char * ssid ;
 char tmp [ 128 ] ;
 int i ;
 if ( ! strncasecmp ( string , "pwd" , 3 ) ) {
 if ( ( p = strchr ( string + + strlen ( "pwd" ) + 1 , ':' ) ) != NULL ) {
 * p = 0 ;
 }
 else {
 SEMIFATAL_ERROR ( "Invalid parsing of the WPA password (missing SSID)" ) ;
 }
 i = strlen ( string + strlen ( "pwd" ) + 1 ) ;
 if ( i < 8 || i > 63 ) {
 SEMIFATAL_ERROR ( "Invalid parsing of the WPA-PWD password (must be 8..63 chars)" ) ;
 }
 SAFE_STRDUP ( pass , string + strlen ( "pwd" ) + 1 ) ;
 SAFE_STRDUP ( ssid , p + 1 ) ;
 PKCS5_PBKDF2_HMAC_SHA1 ( pass , strlen ( pass ) , ( u_char * ) ssid , strlen ( ssid ) , 4096 , 32 , GBL_WIFI -> wkey ) ;
 SAFE_FREE ( pass ) ;
 SAFE_FREE ( ssid ) ;
 }
 if ( ! strncasecmp ( string , "psk" , 3 ) ) {
 if ( strlen ( string + strlen ( "psk" ) + 1 ) != 64 ) {
 SEMIFATAL_ERROR ( "Invalid parsing of the WPA-PSK password (must be 64 chars)" ) ;
 }
 str_hex_to_bytes ( string + strlen ( "psk" ) + 1 , GBL_WIFI -> wkey ) ;
 }
 USER_MSG ( "Using WPA key: %s\n" , str_tohex ( GBL_WIFI -> wkey , WPA_KEY_LEN , tmp , sizeof ( tmp ) ) ) ;
 return E_SUCCESS ;
 }