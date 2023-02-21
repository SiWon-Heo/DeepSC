int tls_construct_server_hello ( SSL * s ) {
 unsigned char * buf ;
 unsigned char * p , * d ;
 int i , sl ;
 int al = 0 ;
 unsigned long l ;
 buf = ( unsigned char * ) s -> init_buf -> data ;
 d = p = ssl_handshake_start ( s ) ;
 * ( p ++ ) = s -> version >> 8 ;
 * ( p ++ ) = s -> version & 0xff ;
 memcpy ( p , s -> s3 -> server_random , SSL3_RANDOM_SIZE ) ;
 p += SSL3_RANDOM_SIZE ;
 if ( s -> session -> not_resumable || ( ! ( s -> ctx -> session_cache_mode & SSL_SESS_CACHE_SERVER ) && ! s -> hit ) ) s -> session -> session_id_length = 0 ;
 sl = s -> session -> session_id_length ;
 if ( sl > ( int ) sizeof ( s -> session -> session_id ) ) {
 SSLerr ( SSL_F_TLS_CONSTRUCT_SERVER_HELLO , ERR_R_INTERNAL_ERROR ) ;
 ossl_statem_set_error ( s ) ;
 return 0 ;
 }
 * ( p ++ ) = sl ;
 memcpy ( p , s -> session -> session_id , sl ) ;
 p += sl ;
 i = ssl3_put_cipher_by_char ( s -> s3 -> tmp . new_cipher , p ) ;
 p += i ;
 # ifdef OPENSSL_NO_COMP * ( p ++ ) = 0 ;
 # else if ( s -> s3 -> tmp . new_compression == NULL ) * ( p ++ ) = 0 ;
 else * ( p ++ ) = s -> s3 -> tmp . new_compression -> id ;
 # endif if ( ssl_prepare_serverhello_tlsext ( s ) <= 0 ) {
 SSLerr ( SSL_F_TLS_CONSTRUCT_SERVER_HELLO , SSL_R_SERVERHELLO_TLSEXT ) ;
 ossl_statem_set_error ( s ) ;
 return 0 ;
 }
 if ( ( p = ssl_add_serverhello_tlsext ( s , p , buf + SSL3_RT_MAX_PLAIN_LENGTH , & al ) ) == NULL ) {
 ssl3_send_alert ( s , SSL3_AL_FATAL , al ) ;
 SSLerr ( SSL_F_TLS_CONSTRUCT_SERVER_HELLO , ERR_R_INTERNAL_ERROR ) ;
 ossl_statem_set_error ( s ) ;
 return 0 ;
 }
 l = ( p - d ) ;
 if ( ! ssl_set_handshake_header ( s , SSL3_MT_SERVER_HELLO , l ) ) {
 SSLerr ( SSL_F_TLS_CONSTRUCT_SERVER_HELLO , ERR_R_INTERNAL_ERROR ) ;
 ossl_statem_set_error ( s ) ;
 return 0 ;
 }
 return 1 ;
 }