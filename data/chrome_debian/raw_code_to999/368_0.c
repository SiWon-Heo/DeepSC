MSG_PROCESS_RETURN tls_process_finished ( SSL * s , PACKET * pkt ) {
 int al , i ;
 if ( ! s -> s3 -> change_cipher_spec ) {
 al = SSL_AD_UNEXPECTED_MESSAGE ;
 SSLerr ( SSL_F_TLS_PROCESS_FINISHED , SSL_R_GOT_A_FIN_BEFORE_A_CCS ) ;
 goto f_err ;
 }
 s -> s3 -> change_cipher_spec = 0 ;
 i = s -> s3 -> tmp . peer_finish_md_len ;
 if ( ( unsigned long ) i != PACKET_remaining ( pkt ) ) {
 al = SSL_AD_DECODE_ERROR ;
 SSLerr ( SSL_F_TLS_PROCESS_FINISHED , SSL_R_BAD_DIGEST_LENGTH ) ;
 goto f_err ;
 }
 if ( CRYPTO_memcmp ( PACKET_data ( pkt ) , s -> s3 -> tmp . peer_finish_md , i ) != 0 ) {
 al = SSL_AD_DECRYPT_ERROR ;
 SSLerr ( SSL_F_TLS_PROCESS_FINISHED , SSL_R_DIGEST_CHECK_FAILED ) ;
 goto f_err ;
 }
 if ( s -> server ) {
 OPENSSL_assert ( i <= EVP_MAX_MD_SIZE ) ;
 memcpy ( s -> s3 -> previous_client_finished , s -> s3 -> tmp . peer_finish_md , i ) ;
 s -> s3 -> previous_client_finished_len = i ;
 }
 else {
 OPENSSL_assert ( i <= EVP_MAX_MD_SIZE ) ;
 memcpy ( s -> s3 -> previous_server_finished , s -> s3 -> tmp . peer_finish_md , i ) ;
 s -> s3 -> previous_server_finished_len = i ;
 }
 return MSG_PROCESS_FINISHED_READING ;
 f_err : ssl3_send_alert ( s , SSL3_AL_FATAL , al ) ;
 ossl_statem_set_error ( s ) ;
 return MSG_PROCESS_ERROR ;
 }