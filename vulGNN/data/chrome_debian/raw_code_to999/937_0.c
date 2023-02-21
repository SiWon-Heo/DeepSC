int wep_decrypt ( u_char * buf , size_t len , u_char * wkey , size_t wlen ) {
 RC4_KEY key ;
 u_char seed [ 32 ] ;
 struct wep_header * wep ;
 u_char * encbuf ;
 u_char decbuf [ len ] ;
 if ( wlen == 0 ) return - E_NOTHANDLED ;
 wep = ( struct wep_header * ) buf ;
 len -= sizeof ( struct wep_header ) ;
 wep -> key >>= 6 ;
 if ( wep -> key * 5 > ( int ) ( MAX_WKEY_LEN - wlen ) ) {
 return - E_NOTHANDLED ;
 }
 encbuf = ( u_char * ) ( wep + 1 ) ;
 memcpy ( seed , wep -> init_vector , WEP_IV_LEN ) ;
 memcpy ( seed + WEP_IV_LEN , & wkey [ wep -> key * 5 ] , wlen ) ;
 RC4_set_key ( & key , WEP_IV_LEN + wlen , seed ) ;
 RC4 ( & key , len + WEP_CRC_LEN , encbuf , decbuf ) ;
 if ( CRC_checksum ( decbuf , len + WEP_CRC_LEN , CRC_INIT ) != CRC_RESULT ) {
 return - E_NOTHANDLED ;
 }
 memcpy ( buf , decbuf , len ) ;
 memset ( buf + len , 0 , WEP_CRC_LEN ) ;
 return E_SUCCESS ;
 }