int dtls1_read_failed ( SSL * s , int code ) {
 if ( code > 0 ) {
 SSLerr ( SSL_F_DTLS1_READ_FAILED , ERR_R_INTERNAL_ERROR ) ;
 return 1 ;
 }
 if ( ! dtls1_is_timer_expired ( s ) ) {
 return code ;
 }
 # ifndef OPENSSL_NO_HEARTBEATS if ( ! SSL_in_init ( s ) && ! s -> tlsext_hb_pending ) # else if ( ! SSL_in_init ( s ) ) # endif {
 BIO_set_flags ( SSL_get_rbio ( s ) , BIO_FLAGS_READ ) ;
 return code ;
 }
 return dtls1_handle_timeout ( s ) ;
 }