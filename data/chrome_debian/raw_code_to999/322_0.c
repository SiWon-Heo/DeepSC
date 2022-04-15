WORK_STATE ossl_statem_server_post_work ( SSL * s , WORK_STATE wst ) {
 OSSL_STATEM * st = & s -> statem ;
 s -> init_num = 0 ;
 switch ( st -> hand_state ) {
 case TLS_ST_SW_HELLO_REQ : if ( statem_flush ( s ) != 1 ) return WORK_MORE_A ;
 if ( ! ssl3_init_finished_mac ( s ) ) {
 ossl_statem_set_error ( s ) ;
 return WORK_ERROR ;
 }
 break ;
 case DTLS_ST_SW_HELLO_VERIFY_REQUEST : if ( statem_flush ( s ) != 1 ) return WORK_MORE_A ;
 if ( s -> version != DTLS1_BAD_VER && ! ssl3_init_finished_mac ( s ) ) {
 ossl_statem_set_error ( s ) ;
 return WORK_ERROR ;
 }
 s -> first_packet = 1 ;
 break ;
 case TLS_ST_SW_SRVR_HELLO : # ifndef OPENSSL_NO_SCTP if ( SSL_IS_DTLS ( s ) && s -> hit ) {
 unsigned char sctpauthkey [ 64 ] ;
 char labelbuffer [ sizeof ( DTLS1_SCTP_AUTH_LABEL ) ] ;
 memcpy ( labelbuffer , DTLS1_SCTP_AUTH_LABEL , sizeof ( DTLS1_SCTP_AUTH_LABEL ) ) ;
 if ( SSL_export_keying_material ( s , sctpauthkey , sizeof ( sctpauthkey ) , labelbuffer , sizeof ( labelbuffer ) , NULL , 0 , 0 ) <= 0 ) {
 ossl_statem_set_error ( s ) ;
 return WORK_ERROR ;
 }
 BIO_ctrl ( SSL_get_wbio ( s ) , BIO_CTRL_DGRAM_SCTP_ADD_AUTH_KEY , sizeof ( sctpauthkey ) , sctpauthkey ) ;
 }
 # endif break ;
 case TLS_ST_SW_CHANGE : # ifndef OPENSSL_NO_SCTP if ( SSL_IS_DTLS ( s ) && ! s -> hit ) {
 BIO_ctrl ( SSL_get_wbio ( s ) , BIO_CTRL_DGRAM_SCTP_NEXT_AUTH_KEY , 0 , NULL ) ;
 }
 # endif if ( ! s -> method -> ssl3_enc -> change_cipher_state ( s , SSL3_CHANGE_CIPHER_SERVER_WRITE ) ) {
 ossl_statem_set_error ( s ) ;
 return WORK_ERROR ;
 }
 if ( SSL_IS_DTLS ( s ) ) dtls1_reset_seq_numbers ( s , SSL3_CC_WRITE ) ;
 break ;
 case TLS_ST_SW_SRVR_DONE : if ( statem_flush ( s ) != 1 ) return WORK_MORE_A ;
 break ;
 case TLS_ST_SW_FINISHED : if ( statem_flush ( s ) != 1 ) return WORK_MORE_A ;
 # ifndef OPENSSL_NO_SCTP if ( SSL_IS_DTLS ( s ) && s -> hit ) {
 BIO_ctrl ( SSL_get_wbio ( s ) , BIO_CTRL_DGRAM_SCTP_NEXT_AUTH_KEY , 0 , NULL ) ;
 }
 # endif break ;
 default : break ;
 }
 return WORK_FINISHED_CONTINUE ;
 }