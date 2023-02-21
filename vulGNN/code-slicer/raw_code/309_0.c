MSG_PROCESS_RETURN tls_process_client_certificate ( SSL * s , PACKET * pkt ) {
 int i , al = SSL_AD_INTERNAL_ERROR , ret = MSG_PROCESS_ERROR ;
 X509 * x = NULL ;
 unsigned long l , llen ;
 const unsigned char * certstart , * certbytes ;
 STACK_OF ( X509 ) * sk = NULL ;
 PACKET spkt ;
 if ( ( sk = sk_X509_new_null ( ) ) == NULL ) {
 SSLerr ( SSL_F_TLS_PROCESS_CLIENT_CERTIFICATE , ERR_R_MALLOC_FAILURE ) ;
 goto f_err ;
 }
 if ( ! PACKET_get_net_3 ( pkt , & llen ) || ! PACKET_get_sub_packet ( pkt , & spkt , llen ) || PACKET_remaining ( pkt ) != 0 ) {
 al = SSL_AD_DECODE_ERROR ;
 SSLerr ( SSL_F_TLS_PROCESS_CLIENT_CERTIFICATE , SSL_R_LENGTH_MISMATCH ) ;
 goto f_err ;
 }
 while ( PACKET_remaining ( & spkt ) > 0 ) {
 if ( ! PACKET_get_net_3 ( & spkt , & l ) || ! PACKET_get_bytes ( & spkt , & certbytes , l ) ) {
 al = SSL_AD_DECODE_ERROR ;
 SSLerr ( SSL_F_TLS_PROCESS_CLIENT_CERTIFICATE , SSL_R_CERT_LENGTH_MISMATCH ) ;
 goto f_err ;
 }
 certstart = certbytes ;
 x = d2i_X509 ( NULL , ( const unsigned char * * ) & certbytes , l ) ;
 if ( x == NULL ) {
 SSLerr ( SSL_F_TLS_PROCESS_CLIENT_CERTIFICATE , ERR_R_ASN1_LIB ) ;
 goto f_err ;
 }
 if ( certbytes != ( certstart + l ) ) {
 al = SSL_AD_DECODE_ERROR ;
 SSLerr ( SSL_F_TLS_PROCESS_CLIENT_CERTIFICATE , SSL_R_CERT_LENGTH_MISMATCH ) ;
 goto f_err ;
 }
 if ( ! sk_X509_push ( sk , x ) ) {
 SSLerr ( SSL_F_TLS_PROCESS_CLIENT_CERTIFICATE , ERR_R_MALLOC_FAILURE ) ;
 goto f_err ;
 }
 x = NULL ;
 }
 if ( sk_X509_num ( sk ) <= 0 ) {
 if ( s -> version == SSL3_VERSION ) {
 al = SSL_AD_HANDSHAKE_FAILURE ;
 SSLerr ( SSL_F_TLS_PROCESS_CLIENT_CERTIFICATE , SSL_R_NO_CERTIFICATES_RETURNED ) ;
 goto f_err ;
 }
 else if ( ( s -> verify_mode & SSL_VERIFY_PEER ) && ( s -> verify_mode & SSL_VERIFY_FAIL_IF_NO_PEER_CERT ) ) {
 SSLerr ( SSL_F_TLS_PROCESS_CLIENT_CERTIFICATE , SSL_R_PEER_DID_NOT_RETURN_A_CERTIFICATE ) ;
 al = SSL_AD_HANDSHAKE_FAILURE ;
 goto f_err ;
 }
 if ( s -> s3 -> handshake_buffer && ! ssl3_digest_cached_records ( s , 0 ) ) {
 goto f_err ;
 }
 }
 else {
 EVP_PKEY * pkey ;
 i = ssl_verify_cert_chain ( s , sk ) ;
 if ( i <= 0 ) {
 al = ssl_verify_alarm_type ( s -> verify_result ) ;
 SSLerr ( SSL_F_TLS_PROCESS_CLIENT_CERTIFICATE , SSL_R_CERTIFICATE_VERIFY_FAILED ) ;
 goto f_err ;
 }
 if ( i > 1 ) {
 SSLerr ( SSL_F_TLS_PROCESS_CLIENT_CERTIFICATE , i ) ;
 al = SSL_AD_HANDSHAKE_FAILURE ;
 goto f_err ;
 }
 pkey = X509_get0_pubkey ( sk_X509_value ( sk , 0 ) ) ;
 if ( pkey == NULL ) {
 al = SSL3_AD_HANDSHAKE_FAILURE ;
 SSLerr ( SSL_F_TLS_PROCESS_CLIENT_CERTIFICATE , SSL_R_UNKNOWN_CERTIFICATE_TYPE ) ;
 goto f_err ;
 }
 }
 X509_free ( s -> session -> peer ) ;
 s -> session -> peer = sk_X509_shift ( sk ) ;
 s -> session -> verify_result = s -> verify_result ;
 sk_X509_pop_free ( s -> session -> peer_chain , X509_free ) ;
 s -> session -> peer_chain = sk ;
 sk = NULL ;
 ret = MSG_PROCESS_CONTINUE_READING ;
 goto done ;
 f_err : ssl3_send_alert ( s , SSL3_AL_FATAL , al ) ;
 ossl_statem_set_error ( s ) ;
 done : X509_free ( x ) ;
 sk_X509_pop_free ( sk , X509_free ) ;
 return ret ;
 }