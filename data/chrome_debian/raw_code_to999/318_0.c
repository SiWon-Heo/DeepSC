int tls_construct_server_done ( SSL * s ) {
 if ( ! ssl_set_handshake_header ( s , SSL3_MT_SERVER_DONE , 0 ) ) {
 SSLerr ( SSL_F_TLS_CONSTRUCT_SERVER_DONE , ERR_R_INTERNAL_ERROR ) ;
 ossl_statem_set_error ( s ) ;
 return 0 ;
 }
 if ( ! s -> s3 -> tmp . cert_request ) {
 if ( ! ssl3_digest_cached_records ( s , 0 ) ) {
 ossl_statem_set_error ( s ) ;
 }
 }
 return 1 ;
 }