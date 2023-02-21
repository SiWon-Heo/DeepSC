MSG_PROCESS_RETURN tls_process_change_cipher_spec ( SSL * s , PACKET * pkt ) {
 int al ;
 long remain ;
 remain = PACKET_remaining ( pkt ) ;
 if ( SSL_IS_DTLS ( s ) ) {
 if ( ( s -> version == DTLS1_BAD_VER && remain != DTLS1_CCS_HEADER_LENGTH + 1 ) || ( s -> version != DTLS1_BAD_VER && remain != DTLS1_CCS_HEADER_LENGTH - 1 ) ) {
 al = SSL_AD_ILLEGAL_PARAMETER ;
 SSLerr ( SSL_F_TLS_PROCESS_CHANGE_CIPHER_SPEC , SSL_R_BAD_CHANGE_CIPHER_SPEC ) ;
 goto f_err ;
 }
 }
 else {
 if ( remain != 0 ) {
 al = SSL_AD_ILLEGAL_PARAMETER ;
 SSLerr ( SSL_F_TLS_PROCESS_CHANGE_CIPHER_SPEC , SSL_R_BAD_CHANGE_CIPHER_SPEC ) ;
 goto f_err ;
 }
 }
 if ( s -> s3 -> tmp . new_cipher == NULL ) {
 al = SSL_AD_UNEXPECTED_MESSAGE ;
 SSLerr ( SSL_F_TLS_PROCESS_CHANGE_CIPHER_SPEC , SSL_R_CCS_RECEIVED_EARLY ) ;
 goto f_err ;
 }
 s -> s3 -> change_cipher_spec = 1 ;
 if ( ! ssl3_do_change_cipher_spec ( s ) ) {
 al = SSL_AD_INTERNAL_ERROR ;
 SSLerr ( SSL_F_TLS_PROCESS_CHANGE_CIPHER_SPEC , ERR_R_INTERNAL_ERROR ) ;
 goto f_err ;
 }
 if ( SSL_IS_DTLS ( s ) ) {
 dtls1_reset_seq_numbers ( s , SSL3_CC_READ ) ;
 if ( s -> version == DTLS1_BAD_VER ) s -> d1 -> handshake_read_seq ++ ;
 # ifndef OPENSSL_NO_SCTP BIO_ctrl ( SSL_get_wbio ( s ) , BIO_CTRL_DGRAM_SCTP_AUTH_CCS_RCVD , 1 , NULL ) ;
 # endif }
 return MSG_PROCESS_CONTINUE_READING ;
 f_err : ssl3_send_alert ( s , SSL3_AL_FATAL , al ) ;
 ossl_statem_set_error ( s ) ;
 return MSG_PROCESS_ERROR ;
 }