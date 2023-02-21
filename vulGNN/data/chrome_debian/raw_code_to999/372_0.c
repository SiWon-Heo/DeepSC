unsigned long ssl3_output_cert_chain ( SSL * s , CERT_PKEY * cpk ) {
 unsigned char * p ;
 unsigned long l = 3 + SSL_HM_HEADER_LENGTH ( s ) ;
 if ( ! ssl_add_cert_chain ( s , cpk , & l ) ) return 0 ;
 l -= 3 + SSL_HM_HEADER_LENGTH ( s ) ;
 p = ssl_handshake_start ( s ) ;
 l2n3 ( l , p ) ;
 l += 3 ;
 if ( ! ssl_set_handshake_header ( s , SSL3_MT_CERTIFICATE , l ) ) {
 SSLerr ( SSL_F_SSL3_OUTPUT_CERT_CHAIN , ERR_R_INTERNAL_ERROR ) ;
 return 0 ;
 }
 return l + SSL_HM_HEADER_LENGTH ( s ) ;
 }