WORK_STATE ossl_statem_server_post_process_message ( SSL * s , WORK_STATE wst ) {
 OSSL_STATEM * st = & s -> statem ;
 switch ( st -> hand_state ) {
 case TLS_ST_SR_CLNT_HELLO : return tls_post_process_client_hello ( s , wst ) ;
 case TLS_ST_SR_KEY_EXCH : return tls_post_process_client_key_exchange ( s , wst ) ;
 case TLS_ST_SR_CERT_VRFY : # ifndef OPENSSL_NO_SCTP if ( BIO_dgram_is_sctp ( SSL_get_wbio ( s ) ) && s -> renegotiate && BIO_dgram_sctp_msg_waiting ( SSL_get_rbio ( s ) ) ) {
 s -> s3 -> in_read_app_data = 2 ;
 s -> rwstate = SSL_READING ;
 BIO_clear_retry_flags ( SSL_get_rbio ( s ) ) ;
 BIO_set_retry_read ( SSL_get_rbio ( s ) ) ;
 ossl_statem_set_sctp_read_sock ( s , 1 ) ;
 return WORK_MORE_A ;
 }
 else {
 ossl_statem_set_sctp_read_sock ( s , 0 ) ;
 }
 # endif return WORK_FINISHED_CONTINUE ;
 default : break ;
 }
 return WORK_ERROR ;
 }