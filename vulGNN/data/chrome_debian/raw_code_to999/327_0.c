int tls_construct_hello_request ( SSL * s ) {
 if ( ! ssl_set_handshake_header ( s , SSL3_MT_HELLO_REQUEST , 0 ) ) {
 SSLerr ( SSL_F_TLS_CONSTRUCT_HELLO_REQUEST , ERR_R_INTERNAL_ERROR ) ;
 ossl_statem_set_error ( s ) ;
 return 0 ;
 }
 return 1 ;
 }