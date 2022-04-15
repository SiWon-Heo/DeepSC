int tls_get_message_body ( SSL * s , unsigned long * len ) {
 long n ;
 unsigned char * p ;
 int i ;
 if ( s -> s3 -> tmp . message_type == SSL3_MT_CHANGE_CIPHER_SPEC ) {
 * len = ( unsigned long ) s -> init_num ;
 return 1 ;
 }
 p = s -> init_msg ;
 n = s -> s3 -> tmp . message_size - s -> init_num ;
 while ( n > 0 ) {
 i = s -> method -> ssl_read_bytes ( s , SSL3_RT_HANDSHAKE , NULL , & p [ s -> init_num ] , n , 0 ) ;
 if ( i <= 0 ) {
 s -> rwstate = SSL_READING ;
 * len = 0 ;
 return 0 ;
 }
 s -> init_num += i ;
 n -= i ;
 }
 # ifndef OPENSSL_NO_NEXTPROTONEG if ( * s -> init_buf -> data == SSL3_MT_FINISHED ) ssl3_take_mac ( s ) ;
 # endif if ( RECORD_LAYER_is_sslv2_record ( & s -> rlayer ) ) {
 if ( ! ssl3_finish_mac ( s , ( unsigned char * ) s -> init_buf -> data , s -> init_num ) ) {
 SSLerr ( SSL_F_TLS_GET_MESSAGE_BODY , ERR_R_EVP_LIB ) ;
 ssl3_send_alert ( s , SSL3_AL_FATAL , SSL_AD_INTERNAL_ERROR ) ;
 * len = 0 ;
 return 0 ;
 }
 if ( s -> msg_callback ) s -> msg_callback ( 0 , SSL2_VERSION , 0 , s -> init_buf -> data , ( size_t ) s -> init_num , s , s -> msg_callback_arg ) ;
 }
 else {
 if ( ! ssl3_finish_mac ( s , ( unsigned char * ) s -> init_buf -> data , s -> init_num + SSL3_HM_HEADER_LENGTH ) ) {
 SSLerr ( SSL_F_TLS_GET_MESSAGE_BODY , ERR_R_EVP_LIB ) ;
 ssl3_send_alert ( s , SSL3_AL_FATAL , SSL_AD_INTERNAL_ERROR ) ;
 * len = 0 ;
 return 0 ;
 }
 if ( s -> msg_callback ) s -> msg_callback ( 0 , s -> version , SSL3_RT_HANDSHAKE , s -> init_buf -> data , ( size_t ) s -> init_num + SSL3_HM_HEADER_LENGTH , s , s -> msg_callback_arg ) ;
 }
 if ( s -> init_num < 0 ) {
 SSLerr ( SSL_F_TLS_GET_MESSAGE_BODY , ERR_R_INTERNAL_ERROR ) ;
 ssl3_send_alert ( s , SSL3_AL_FATAL , SSL_AD_INTERNAL_ERROR ) ;
 * len = 0 ;
 return 0 ;
 }
 * len = ( unsigned long ) s -> init_num ;
 return 1 ;
 }