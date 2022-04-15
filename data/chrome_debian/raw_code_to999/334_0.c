int dtls_construct_hello_verify_request ( SSL * s ) {
 unsigned int len ;
 unsigned char * buf ;
 buf = ( unsigned char * ) s -> init_buf -> data ;
 if ( s -> ctx -> app_gen_cookie_cb == NULL || s -> ctx -> app_gen_cookie_cb ( s , s -> d1 -> cookie , & ( s -> d1 -> cookie_len ) ) == 0 || s -> d1 -> cookie_len > 255 ) {
 SSLerr ( SSL_F_DTLS_CONSTRUCT_HELLO_VERIFY_REQUEST , SSL_R_COOKIE_GEN_CALLBACK_FAILURE ) ;
 ossl_statem_set_error ( s ) ;
 return 0 ;
 }
 len = dtls_raw_hello_verify_request ( & buf [ DTLS1_HM_HEADER_LENGTH ] , s -> d1 -> cookie , s -> d1 -> cookie_len ) ;
 dtls1_set_message_header ( s , DTLS1_MT_HELLO_VERIFY_REQUEST , len , 0 , len ) ;
 len += DTLS1_HM_HEADER_LENGTH ;
 s -> init_num = len ;
 s -> init_off = 0 ;
 return 1 ;
 }