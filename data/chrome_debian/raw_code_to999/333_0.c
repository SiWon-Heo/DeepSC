unsigned int dtls_raw_hello_verify_request ( unsigned char * buf , unsigned char * cookie , unsigned char cookie_len ) {
 unsigned int msg_len ;
 unsigned char * p ;
 p = buf ;
 * ( p ++ ) = DTLS1_VERSION >> 8 ;
 * ( p ++ ) = DTLS1_VERSION & 0xFF ;
 * ( p ++ ) = ( unsigned char ) cookie_len ;
 memcpy ( p , cookie , cookie_len ) ;
 p += cookie_len ;
 msg_len = p - buf ;
 return msg_len ;
 }