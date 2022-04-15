WORK_STATE ossl_statem_server_pre_work ( SSL * s , WORK_STATE wst ) {
 OSSL_STATEM * st = & s -> statem ;
 switch ( st -> hand_state ) {
 case TLS_ST_SW_HELLO_REQ : s -> shutdown = 0 ;
 if ( SSL_IS_DTLS ( s ) ) dtls1_clear_sent_buffer ( s ) ;
 break ;
 case DTLS_ST_SW_HELLO_VERIFY_REQUEST : s -> shutdown = 0 ;
 if ( SSL_IS_DTLS ( s ) ) {
 dtls1_clear_sent_buffer ( s ) ;
 st -> use_timer = 0 ;
 }
 break ;
 case TLS_ST_SW_SRVR_HELLO : if ( SSL_IS_DTLS ( s ) ) {
 st -> use_timer = 1 ;
 }
 break ;
 case TLS_ST_SW_SRVR_DONE : # ifndef OPENSSL_NO_SCTP if ( SSL_IS_DTLS ( s ) && BIO_dgram_is_sctp ( SSL_get_wbio ( s ) ) ) return dtls_wait_for_dry ( s ) ;
 # endif return WORK_FINISHED_CONTINUE ;
 case TLS_ST_SW_SESSION_TICKET : if ( SSL_IS_DTLS ( s ) ) {
 st -> use_timer = 0 ;
 }
 break ;
 case TLS_ST_SW_CHANGE : s -> session -> cipher = s -> s3 -> tmp . new_cipher ;
 if ( ! s -> method -> ssl3_enc -> setup_key_block ( s ) ) {
 ossl_statem_set_error ( s ) ;
 return WORK_ERROR ;
 }
 if ( SSL_IS_DTLS ( s ) ) {
 st -> use_timer = 0 ;
 }
 return WORK_FINISHED_CONTINUE ;
 case TLS_ST_OK : return tls_finish_handshake ( s , wst ) ;
 default : break ;
 }
 return WORK_FINISHED_CONTINUE ;
 }