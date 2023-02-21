int tls_construct_finished ( SSL * s , const char * sender , int slen ) {
 unsigned char * p ;
 int i ;
 unsigned long l ;
 p = ssl_handshake_start ( s ) ;
 i = s -> method -> ssl3_enc -> final_finish_mac ( s , sender , slen , s -> s3 -> tmp . finish_md ) ;
 if ( i <= 0 ) return 0 ;
 s -> s3 -> tmp . finish_md_len = i ;
 memcpy ( p , s -> s3 -> tmp . finish_md , i ) ;
 l = i ;
 if ( ! s -> server ) {
 OPENSSL_assert ( i <= EVP_MAX_MD_SIZE ) ;
 memcpy ( s -> s3 -> previous_client_finished , s -> s3 -> tmp . finish_md , i ) ;
 s -> s3 -> previous_client_finished_len = i ;
 }
 else {
 OPENSSL_assert ( i <= EVP_MAX_MD_SIZE ) ;
 memcpy ( s -> s3 -> previous_server_finished , s -> s3 -> tmp . finish_md , i ) ;
 s -> s3 -> previous_server_finished_len = i ;
 }
 if ( ! ssl_set_handshake_header ( s , SSL3_MT_FINISHED , l ) ) {
 SSLerr ( SSL_F_TLS_CONSTRUCT_FINISHED , ERR_R_INTERNAL_ERROR ) ;
 return 0 ;
 }
 return 1 ;
 }