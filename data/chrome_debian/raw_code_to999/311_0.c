static int tls_process_cke_psk_preamble ( SSL * s , PACKET * pkt , int * al ) {
 # ifndef OPENSSL_NO_PSK unsigned char psk [ PSK_MAX_PSK_LEN ] ;
 size_t psklen ;
 PACKET psk_identity ;
 if ( ! PACKET_get_length_prefixed_2 ( pkt , & psk_identity ) ) {
 * al = SSL_AD_DECODE_ERROR ;
 SSLerr ( SSL_F_TLS_PROCESS_CKE_PSK_PREAMBLE , SSL_R_LENGTH_MISMATCH ) ;
 return 0 ;
 }
 if ( PACKET_remaining ( & psk_identity ) > PSK_MAX_IDENTITY_LEN ) {
 * al = SSL_AD_DECODE_ERROR ;
 SSLerr ( SSL_F_TLS_PROCESS_CKE_PSK_PREAMBLE , SSL_R_DATA_LENGTH_TOO_LONG ) ;
 return 0 ;
 }
 if ( s -> psk_server_callback == NULL ) {
 * al = SSL_AD_INTERNAL_ERROR ;
 SSLerr ( SSL_F_TLS_PROCESS_CKE_PSK_PREAMBLE , SSL_R_PSK_NO_SERVER_CB ) ;
 return 0 ;
 }
 if ( ! PACKET_strndup ( & psk_identity , & s -> session -> psk_identity ) ) {
 * al = SSL_AD_INTERNAL_ERROR ;
 SSLerr ( SSL_F_TLS_PROCESS_CKE_PSK_PREAMBLE , ERR_R_INTERNAL_ERROR ) ;
 return 0 ;
 }
 psklen = s -> psk_server_callback ( s , s -> session -> psk_identity , psk , sizeof ( psk ) ) ;
 if ( psklen > PSK_MAX_PSK_LEN ) {
 * al = SSL_AD_INTERNAL_ERROR ;
 SSLerr ( SSL_F_TLS_PROCESS_CKE_PSK_PREAMBLE , ERR_R_INTERNAL_ERROR ) ;
 return 0 ;
 }
 else if ( psklen == 0 ) {
 * al = SSL_AD_UNKNOWN_PSK_IDENTITY ;
 SSLerr ( SSL_F_TLS_PROCESS_CKE_PSK_PREAMBLE , SSL_R_PSK_IDENTITY_NOT_FOUND ) ;
 return 0 ;
 }
 OPENSSL_free ( s -> s3 -> tmp . psk ) ;
 s -> s3 -> tmp . psk = OPENSSL_memdup ( psk , psklen ) ;
 OPENSSL_cleanse ( psk , psklen ) ;
 if ( s -> s3 -> tmp . psk == NULL ) {
 * al = SSL_AD_INTERNAL_ERROR ;
 SSLerr ( SSL_F_TLS_PROCESS_CKE_PSK_PREAMBLE , ERR_R_MALLOC_FAILURE ) ;
 return 0 ;
 }
 s -> s3 -> tmp . psklen = psklen ;
 return 1 ;
 # else * al = SSL_AD_INTERNAL_ERROR ;
 SSLerr ( SSL_F_TLS_PROCESS_CKE_PSK_PREAMBLE , ERR_R_INTERNAL_ERROR ) ;
 return 0 ;
 # endif }