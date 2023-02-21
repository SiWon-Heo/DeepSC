int tls_construct_server_certificate ( SSL * s ) {
 CERT_PKEY * cpk ;
 cpk = ssl_get_server_send_pkey ( s ) ;
 if ( cpk == NULL ) {
 SSLerr ( SSL_F_TLS_CONSTRUCT_SERVER_CERTIFICATE , ERR_R_INTERNAL_ERROR ) ;
 ossl_statem_set_error ( s ) ;
 return 0 ;
 }
 if ( ! ssl3_output_cert_chain ( s , cpk ) ) {
 SSLerr ( SSL_F_TLS_CONSTRUCT_SERVER_CERTIFICATE , ERR_R_INTERNAL_ERROR ) ;
 ossl_statem_set_error ( s ) ;
 return 0 ;
 }
 return 1 ;
 }