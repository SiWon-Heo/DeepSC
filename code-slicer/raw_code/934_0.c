int wpa_generate_PTK ( u_char * bssid , u_char * sta , u_char * pmk , u_char * snonce , u_char * anonce , u_int16 bits , u_char * kck ) {
 u_int8 i ;
 u_int len ;
 u_char buff [ 100 ] ;
 size_t offset = sizeof ( "Pairwise key expansion" ) ;
 memset ( buff , 0 , 100 ) ;
 memcpy ( buff , "Pairwise key expansion" , offset ) ;
 if ( memcmp ( sta , bssid , ETH_ADDR_LEN ) < 0 ) {
 memcpy ( buff + offset , sta , ETH_ADDR_LEN ) ;
 memcpy ( buff + offset + ETH_ADDR_LEN , bssid , ETH_ADDR_LEN ) ;
 }
 else {
 memcpy ( buff + offset , bssid , ETH_ADDR_LEN ) ;
 memcpy ( buff + offset + ETH_ADDR_LEN , sta , ETH_ADDR_LEN ) ;
 }
 offset += ETH_ADDR_LEN * 2 ;
 if ( memcmp ( snonce , anonce , WPA_NONCE_LEN ) < 0 ) {
 memcpy ( buff + offset , snonce , WPA_NONCE_LEN ) ;
 memcpy ( buff + offset + WPA_NONCE_LEN , anonce , WPA_NONCE_LEN ) ;
 }
 else {
 memcpy ( buff + offset , anonce , WPA_NONCE_LEN ) ;
 memcpy ( buff + offset + WPA_NONCE_LEN , snonce , WPA_NONCE_LEN ) ;
 }
 offset += WPA_NONCE_LEN * 2 ;
 memset ( kck , 0 , WPA_PTK_LEN ) ;
 for ( i = 0 ;
 i < ( bits + 159 ) / 160 ;
 i ++ ) {
 buff [ offset ] = i ;
 HMAC ( EVP_sha1 ( ) , pmk , WPA_KEY_LEN , buff , 100 , kck + i * 20 , & len ) ;
 }
 return E_SUCCESS ;
 }