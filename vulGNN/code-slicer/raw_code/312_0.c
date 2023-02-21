WORK_STATE tls_post_process_client_key_exchange ( SSL * s , WORK_STATE wst ) {
 # ifndef OPENSSL_NO_SCTP if ( wst == WORK_MORE_A ) {
 if ( SSL_IS_DTLS ( s ) ) {
 unsigned char sctpauthkey [ 64 ] ;
 char labelbuffer [ sizeof ( DTLS1_SCTP_AUTH_LABEL ) ] ;
 memcpy ( labelbuffer , DTLS1_SCTP_AUTH_LABEL , sizeof ( DTLS1_SCTP_AUTH_LABEL ) ) ;
 if ( SSL_export_keying_material ( s , sctpauthkey , sizeof ( sctpauthkey ) , labelbuffer , sizeof ( labelbuffer ) , NULL , 0 , 0 ) <= 0 ) {
 ossl_statem_set_error ( s ) ;
 return WORK_ERROR ;
 ;
 }
 BIO_ctrl ( SSL_get_wbio ( s ) , BIO_CTRL_DGRAM_SCTP_ADD_AUTH_KEY , sizeof ( sctpauthkey ) , sctpauthkey ) ;
 }
 wst = WORK_MORE_B ;
 }
 if ( ( wst == WORK_MORE_B ) && BIO_dgram_is_sctp ( SSL_get_wbio ( s ) ) && s -> renegotiate && ( s -> session -> peer == NULL || s -> statem . no_cert_verify ) && BIO_dgram_sctp_msg_waiting ( SSL_get_rbio ( s ) ) ) {
 s -> s3 -> in_read_app_data = 2 ;
 s -> rwstate = SSL_READING ;
 BIO_clear_retry_flags ( SSL_get_rbio ( s ) ) ;
 BIO_set_retry_read ( SSL_get_rbio ( s ) ) ;
 ossl_statem_set_sctp_read_sock ( s , 1 ) ;
 return WORK_MORE_B ;
 }
 else {
 ossl_statem_set_sctp_read_sock ( s , 0 ) ;
 }
 # endif if ( s -> statem . no_cert_verify || ! s -> session -> peer ) {
 if ( ! ssl3_digest_cached_records ( s , 0 ) ) {
 ossl_statem_set_error ( s ) ;
 return WORK_ERROR ;
 }
 return WORK_FINISHED_CONTINUE ;
 }
 else {
 if ( ! s -> s3 -> handshake_buffer ) {
 SSLerr ( SSL_F_TLS_POST_PROCESS_CLIENT_KEY_EXCHANGE , ERR_R_INTERNAL_ERROR ) ;
 ossl_statem_set_error ( s ) ;
 return WORK_ERROR ;
 }
 if ( ! ssl3_digest_cached_records ( s , 1 ) ) {
 ossl_statem_set_error ( s ) ;
 return WORK_ERROR ;
 }
 }
 return WORK_FINISHED_CONTINUE ;
 }