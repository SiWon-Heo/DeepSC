int DTLSv1_listen ( SSL * s , BIO_ADDR * client ) {
 int next , n , ret = 0 , clearpkt = 0 ;
 unsigned char cookie [ DTLS1_COOKIE_LENGTH ] ;
 unsigned char seq [ SEQ_NUM_SIZE ] ;
 const unsigned char * data ;
 unsigned char * p , * buf ;
 unsigned long reclen , fragoff , fraglen , msglen ;
 unsigned int rectype , versmajor , msgseq , msgtype , clientvers , cookielen ;
 BIO * rbio , * wbio ;
 BUF_MEM * bufm ;
 BIO_ADDR * tmpclient = NULL ;
 PACKET pkt , msgpkt , msgpayload , session , cookiepkt ;
 if ( ! SSL_clear ( s ) ) return - 1 ;
 ERR_clear_error ( ) ;
 rbio = SSL_get_rbio ( s ) ;
 wbio = SSL_get_wbio ( s ) ;
 if ( ! rbio || ! wbio ) {
 SSLerr ( SSL_F_DTLSV1_LISTEN , SSL_R_BIO_NOT_SET ) ;
 return - 1 ;
 }
 BIO_ctrl ( SSL_get_rbio ( s ) , BIO_CTRL_DGRAM_SET_PEEK_MODE , 1 , NULL ) ;
 if ( ( s -> version & 0xff00 ) != ( DTLS1_VERSION & 0xff00 ) ) {
 SSLerr ( SSL_F_DTLSV1_LISTEN , SSL_R_UNSUPPORTED_SSL_VERSION ) ;
 return - 1 ;
 }
 if ( s -> init_buf == NULL ) {
 if ( ( bufm = BUF_MEM_new ( ) ) == NULL ) {
 SSLerr ( SSL_F_DTLSV1_LISTEN , ERR_R_MALLOC_FAILURE ) ;
 return - 1 ;
 }
 if ( ! BUF_MEM_grow ( bufm , SSL3_RT_MAX_PLAIN_LENGTH ) ) {
 BUF_MEM_free ( bufm ) ;
 SSLerr ( SSL_F_DTLSV1_LISTEN , ERR_R_MALLOC_FAILURE ) ;
 return - 1 ;
 }
 s -> init_buf = bufm ;
 }
 buf = ( unsigned char * ) s -> init_buf -> data ;
 do {
 clear_sys_error ( ) ;
 n = BIO_read ( rbio , buf , SSL3_RT_MAX_PLAIN_LENGTH ) ;
 if ( n <= 0 ) {
 if ( BIO_should_retry ( rbio ) ) {
 goto end ;
 }
 return - 1 ;
 }
 clearpkt = 1 ;
 if ( ! PACKET_buf_init ( & pkt , buf , n ) ) {
 SSLerr ( SSL_F_DTLSV1_LISTEN , ERR_R_INTERNAL_ERROR ) ;
 return - 1 ;
 }
 if ( n < DTLS1_RT_HEADER_LENGTH ) {
 SSLerr ( SSL_F_DTLSV1_LISTEN , SSL_R_RECORD_TOO_SMALL ) ;
 goto end ;
 }
 if ( s -> msg_callback ) s -> msg_callback ( 0 , 0 , SSL3_RT_HEADER , buf , DTLS1_RT_HEADER_LENGTH , s , s -> msg_callback_arg ) ;
 if ( ! PACKET_get_1 ( & pkt , & rectype ) || ! PACKET_get_1 ( & pkt , & versmajor ) ) {
 SSLerr ( SSL_F_DTLSV1_LISTEN , SSL_R_LENGTH_MISMATCH ) ;
 goto end ;
 }
 if ( rectype != SSL3_RT_HANDSHAKE ) {
 SSLerr ( SSL_F_DTLSV1_LISTEN , SSL_R_UNEXPECTED_MESSAGE ) ;
 goto end ;
 }
 if ( versmajor != DTLS1_VERSION_MAJOR ) {
 SSLerr ( SSL_F_DTLSV1_LISTEN , SSL_R_BAD_PROTOCOL_VERSION_NUMBER ) ;
 goto end ;
 }
 if ( ! PACKET_forward ( & pkt , 1 ) || ! PACKET_copy_bytes ( & pkt , seq , SEQ_NUM_SIZE ) || ! PACKET_get_length_prefixed_2 ( & pkt , & msgpkt ) ) {
 SSLerr ( SSL_F_DTLSV1_LISTEN , SSL_R_LENGTH_MISMATCH ) ;
 goto end ;
 }
 if ( seq [ 0 ] != 0 || seq [ 1 ] != 0 ) {
 SSLerr ( SSL_F_DTLSV1_LISTEN , SSL_R_UNEXPECTED_MESSAGE ) ;
 goto end ;
 }
 data = PACKET_data ( & msgpkt ) ;
 if ( ! PACKET_get_1 ( & msgpkt , & msgtype ) || ! PACKET_get_net_3 ( & msgpkt , & msglen ) || ! PACKET_get_net_2 ( & msgpkt , & msgseq ) || ! PACKET_get_net_3 ( & msgpkt , & fragoff ) || ! PACKET_get_net_3 ( & msgpkt , & fraglen ) || ! PACKET_get_sub_packet ( & msgpkt , & msgpayload , fraglen ) || PACKET_remaining ( & msgpkt ) != 0 ) {
 SSLerr ( SSL_F_DTLSV1_LISTEN , SSL_R_LENGTH_MISMATCH ) ;
 goto end ;
 }
 if ( msgtype != SSL3_MT_CLIENT_HELLO ) {
 SSLerr ( SSL_F_DTLSV1_LISTEN , SSL_R_UNEXPECTED_MESSAGE ) ;
 goto end ;
 }
 if ( msgseq > 2 ) {
 SSLerr ( SSL_F_DTLSV1_LISTEN , SSL_R_INVALID_SEQUENCE_NUMBER ) ;
 goto end ;
 }
 if ( fragoff != 0 || fraglen > msglen ) {
 SSLerr ( SSL_F_DTLSV1_LISTEN , SSL_R_FRAGMENTED_CLIENT_HELLO ) ;
 goto end ;
 }
 if ( s -> msg_callback ) s -> msg_callback ( 0 , s -> version , SSL3_RT_HANDSHAKE , data , fraglen + DTLS1_HM_HEADER_LENGTH , s , s -> msg_callback_arg ) ;
 if ( ! PACKET_get_net_2 ( & msgpayload , & clientvers ) ) {
 SSLerr ( SSL_F_DTLSV1_LISTEN , SSL_R_LENGTH_MISMATCH ) ;
 goto end ;
 }
 if ( DTLS_VERSION_LT ( clientvers , ( unsigned int ) s -> method -> version ) && s -> method -> version != DTLS_ANY_VERSION ) {
 SSLerr ( SSL_F_DTLSV1_LISTEN , SSL_R_WRONG_VERSION_NUMBER ) ;
 goto end ;
 }
 if ( ! PACKET_forward ( & msgpayload , SSL3_RANDOM_SIZE ) || ! PACKET_get_length_prefixed_1 ( & msgpayload , & session ) || ! PACKET_get_length_prefixed_1 ( & msgpayload , & cookiepkt ) ) {
 SSLerr ( SSL_F_DTLSV1_LISTEN , SSL_R_LENGTH_MISMATCH ) ;
 goto end ;
 }
 if ( PACKET_remaining ( & cookiepkt ) == 0 ) {
 next = LISTEN_SEND_VERIFY_REQUEST ;
 }
 else {
 if ( s -> ctx -> app_verify_cookie_cb == NULL ) {
 SSLerr ( SSL_F_DTLSV1_LISTEN , SSL_R_NO_VERIFY_COOKIE_CALLBACK ) ;
 return - 1 ;
 }
 if ( s -> ctx -> app_verify_cookie_cb ( s , PACKET_data ( & cookiepkt ) , PACKET_remaining ( & cookiepkt ) ) == 0 ) {
 next = LISTEN_SEND_VERIFY_REQUEST ;
 }
 else {
 next = LISTEN_SUCCESS ;
 }
 }
 if ( next == LISTEN_SEND_VERIFY_REQUEST ) {
 BIO_ctrl ( SSL_get_rbio ( s ) , BIO_CTRL_DGRAM_SET_PEEK_MODE , 0 , NULL ) ;
 BIO_read ( rbio , buf , SSL3_RT_MAX_PLAIN_LENGTH ) ;
 BIO_ctrl ( SSL_get_rbio ( s ) , BIO_CTRL_DGRAM_SET_PEEK_MODE , 1 , NULL ) ;
 if ( s -> ctx -> app_gen_cookie_cb == NULL || s -> ctx -> app_gen_cookie_cb ( s , cookie , & cookielen ) == 0 || cookielen > 255 ) {
 SSLerr ( SSL_F_DTLSV1_LISTEN , SSL_R_COOKIE_GEN_CALLBACK_FAILURE ) ;
 return - 1 ;
 }
 p = & buf [ DTLS1_RT_HEADER_LENGTH ] ;
 msglen = dtls_raw_hello_verify_request ( p + DTLS1_HM_HEADER_LENGTH , cookie , cookielen ) ;
 * p ++ = DTLS1_MT_HELLO_VERIFY_REQUEST ;
 l2n3 ( msglen , p ) ;
 s2n ( 0 , p ) ;
 l2n3 ( 0 , p ) ;
 l2n3 ( msglen , p ) ;
 reclen = msglen + DTLS1_HM_HEADER_LENGTH ;
 p = buf ;
 * ( p ++ ) = SSL3_RT_HANDSHAKE ;
 if ( s -> method -> version == DTLS_ANY_VERSION ) {
 * ( p ++ ) = DTLS1_VERSION >> 8 ;
 * ( p ++ ) = DTLS1_VERSION & 0xff ;
 }
 else {
 * ( p ++ ) = s -> version >> 8 ;
 * ( p ++ ) = s -> version & 0xff ;
 }
 memcpy ( p , seq , SEQ_NUM_SIZE ) ;
 p += SEQ_NUM_SIZE ;
 s2n ( reclen , p ) ;
 reclen += DTLS1_RT_HEADER_LENGTH ;
 if ( s -> msg_callback ) s -> msg_callback ( 1 , 0 , SSL3_RT_HEADER , buf , DTLS1_RT_HEADER_LENGTH , s , s -> msg_callback_arg ) ;
 if ( ( tmpclient = BIO_ADDR_new ( ) ) == NULL ) {
 SSLerr ( SSL_F_DTLSV1_LISTEN , ERR_R_MALLOC_FAILURE ) ;
 goto end ;
 }
 if ( BIO_dgram_get_peer ( rbio , tmpclient ) > 0 ) {
 ( void ) BIO_dgram_set_peer ( wbio , tmpclient ) ;
 }
 BIO_ADDR_free ( tmpclient ) ;
 tmpclient = NULL ;
 if ( BIO_write ( wbio , buf , reclen ) < ( int ) reclen ) {
 if ( BIO_should_retry ( wbio ) ) {
 goto end ;
 }
 return - 1 ;
 }
 if ( BIO_flush ( wbio ) <= 0 ) {
 if ( BIO_should_retry ( wbio ) ) {
 goto end ;
 }
 return - 1 ;
 }
 }
 }
 while ( next != LISTEN_SUCCESS ) ;
 s -> d1 -> handshake_read_seq = 1 ;
 s -> d1 -> handshake_write_seq = 1 ;
 s -> d1 -> next_handshake_write_seq = 1 ;
 DTLS_RECORD_LAYER_set_write_sequence ( & s -> rlayer , seq ) ;
 SSL_set_options ( s , SSL_OP_COOKIE_EXCHANGE ) ;
 ossl_statem_set_hello_verify_done ( s ) ;
 if ( BIO_dgram_get_peer ( rbio , client ) <= 0 ) BIO_ADDR_clear ( client ) ;
 ret = 1 ;
 clearpkt = 0 ;
 end : BIO_ADDR_free ( tmpclient ) ;
 BIO_ctrl ( SSL_get_rbio ( s ) , BIO_CTRL_DGRAM_SET_PEEK_MODE , 0 , NULL ) ;
 if ( clearpkt ) {
 BIO_read ( rbio , buf , SSL3_RT_MAX_PLAIN_LENGTH ) ;
 }
 return ret ;
 }