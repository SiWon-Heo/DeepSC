MSG_PROCESS_RETURN tls_process_client_key_exchange ( SSL * s , PACKET * pkt ) {
 int al = - 1 ;
 unsigned long alg_k ;
 alg_k = s -> s3 -> tmp . new_cipher -> algorithm_mkey ;
 if ( ( alg_k & SSL_PSK ) && ! tls_process_cke_psk_preamble ( s , pkt , & al ) ) goto err ;
 if ( alg_k & SSL_kPSK ) {
 if ( PACKET_remaining ( pkt ) != 0 ) {
 al = SSL_AD_HANDSHAKE_FAILURE ;
 SSLerr ( SSL_F_TLS_PROCESS_CLIENT_KEY_EXCHANGE , SSL_R_LENGTH_MISMATCH ) ;
 goto err ;
 }
 if ( ! ssl_generate_master_secret ( s , NULL , 0 , 0 ) ) {
 al = SSL_AD_INTERNAL_ERROR ;
 SSLerr ( SSL_F_TLS_PROCESS_CLIENT_KEY_EXCHANGE , ERR_R_INTERNAL_ERROR ) ;
 goto err ;
 }
 }
 else if ( alg_k & ( SSL_kRSA | SSL_kRSAPSK ) ) {
 if ( ! tls_process_cke_rsa ( s , pkt , & al ) ) goto err ;
 }
 else if ( alg_k & ( SSL_kDHE | SSL_kDHEPSK ) ) {
 if ( ! tls_process_cke_dhe ( s , pkt , & al ) ) goto err ;
 }
 else if ( alg_k & ( SSL_kECDHE | SSL_kECDHEPSK ) ) {
 if ( ! tls_process_cke_ecdhe ( s , pkt , & al ) ) goto err ;
 }
 else if ( alg_k & SSL_kSRP ) {
 if ( ! tls_process_cke_srp ( s , pkt , & al ) ) goto err ;
 }
 else if ( alg_k & SSL_kGOST ) {
 if ( ! tls_process_cke_gost ( s , pkt , & al ) ) goto err ;
 }
 else {
 al = SSL_AD_HANDSHAKE_FAILURE ;
 SSLerr ( SSL_F_TLS_PROCESS_CLIENT_KEY_EXCHANGE , SSL_R_UNKNOWN_CIPHER_TYPE ) ;
 goto err ;
 }
 return MSG_PROCESS_CONTINUE_PROCESSING ;
 err : if ( al != - 1 ) ssl3_send_alert ( s , SSL3_AL_FATAL , al ) ;
 # ifndef OPENSSL_NO_PSK OPENSSL_clear_free ( s -> s3 -> tmp . psk , s -> s3 -> tmp . psklen ) ;
 s -> s3 -> tmp . psk = NULL ;
 # endif ossl_statem_set_error ( s ) ;
 return MSG_PROCESS_ERROR ;
 }