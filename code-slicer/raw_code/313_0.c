WORK_STATE tls_post_process_client_hello ( SSL * s , WORK_STATE wst ) {
 int al = SSL_AD_HANDSHAKE_FAILURE ;
 const SSL_CIPHER * cipher ;
 if ( wst == WORK_MORE_A ) {
 if ( ! s -> hit ) {
 if ( s -> cert -> cert_cb ) {
 int rv = s -> cert -> cert_cb ( s , s -> cert -> cert_cb_arg ) ;
 if ( rv == 0 ) {
 al = SSL_AD_INTERNAL_ERROR ;
 SSLerr ( SSL_F_TLS_POST_PROCESS_CLIENT_HELLO , SSL_R_CERT_CB_ERROR ) ;
 goto f_err ;
 }
 if ( rv < 0 ) {
 s -> rwstate = SSL_X509_LOOKUP ;
 return WORK_MORE_A ;
 }
 s -> rwstate = SSL_NOTHING ;
 }
 cipher = ssl3_choose_cipher ( s , s -> session -> ciphers , SSL_get_ciphers ( s ) ) ;
 if ( cipher == NULL ) {
 SSLerr ( SSL_F_TLS_POST_PROCESS_CLIENT_HELLO , SSL_R_NO_SHARED_CIPHER ) ;
 goto f_err ;
 }
 s -> s3 -> tmp . new_cipher = cipher ;
 if ( s -> not_resumable_session_cb != NULL ) s -> session -> not_resumable = s -> not_resumable_session_cb ( s , ( ( cipher -> algorithm_mkey & ( SSL_kDHE | SSL_kECDHE ) ) != 0 ) ) ;
 if ( s -> session -> not_resumable ) s -> tlsext_ticket_expected = 0 ;
 }
 else {
 s -> s3 -> tmp . new_cipher = s -> session -> cipher ;
 }
 if ( ! ( s -> verify_mode & SSL_VERIFY_PEER ) ) {
 if ( ! ssl3_digest_cached_records ( s , 0 ) ) {
 al = SSL_AD_INTERNAL_ERROR ;
 goto f_err ;
 }
 }
 if ( s -> version >= SSL3_VERSION ) {
 if ( ! ssl_check_clienthello_tlsext_late ( s , & al ) ) {
 SSLerr ( SSL_F_TLS_POST_PROCESS_CLIENT_HELLO , SSL_R_CLIENTHELLO_TLSEXT ) ;
 goto f_err ;
 }
 }
 wst = WORK_MORE_B ;
 }
 # ifndef OPENSSL_NO_SRP if ( wst == WORK_MORE_B ) {
 int ret ;
 if ( ( ret = ssl_check_srp_ext_ClientHello ( s , & al ) ) < 0 ) {
 s -> rwstate = SSL_X509_LOOKUP ;
 return WORK_MORE_B ;
 }
 if ( ret != SSL_ERROR_NONE ) {
 if ( al != TLS1_AD_UNKNOWN_PSK_IDENTITY ) SSLerr ( SSL_F_TLS_POST_PROCESS_CLIENT_HELLO , SSL_R_CLIENTHELLO_TLSEXT ) ;
 goto f_err ;
 }
 }
 # endif s -> renegotiate = 2 ;
 return WORK_FINISHED_STOP ;
 f_err : ssl3_send_alert ( s , SSL3_AL_FATAL , al ) ;
 ossl_statem_set_error ( s ) ;
 return WORK_ERROR ;
 }