static int tls_process_cke_srp ( SSL * s , PACKET * pkt , int * al ) {
 # ifndef OPENSSL_NO_SRP unsigned int i ;
 const unsigned char * data ;
 if ( ! PACKET_get_net_2 ( pkt , & i ) || ! PACKET_get_bytes ( pkt , & data , i ) ) {
 * al = SSL_AD_DECODE_ERROR ;
 SSLerr ( SSL_F_TLS_PROCESS_CKE_SRP , SSL_R_BAD_SRP_A_LENGTH ) ;
 return 0 ;
 }
 if ( ( s -> srp_ctx . A = BN_bin2bn ( data , i , NULL ) ) == NULL ) {
 SSLerr ( SSL_F_TLS_PROCESS_CKE_SRP , ERR_R_BN_LIB ) ;
 return 0 ;
 }
 if ( BN_ucmp ( s -> srp_ctx . A , s -> srp_ctx . N ) >= 0 || BN_is_zero ( s -> srp_ctx . A ) ) {
 * al = SSL_AD_ILLEGAL_PARAMETER ;
 SSLerr ( SSL_F_TLS_PROCESS_CKE_SRP , SSL_R_BAD_SRP_PARAMETERS ) ;
 return 0 ;
 }
 OPENSSL_free ( s -> session -> srp_username ) ;
 s -> session -> srp_username = OPENSSL_strdup ( s -> srp_ctx . login ) ;
 if ( s -> session -> srp_username == NULL ) {
 SSLerr ( SSL_F_TLS_PROCESS_CKE_SRP , ERR_R_MALLOC_FAILURE ) ;
 return 0 ;
 }
 if ( ! srp_generate_server_master_secret ( s ) ) {
 SSLerr ( SSL_F_TLS_PROCESS_CKE_SRP , ERR_R_INTERNAL_ERROR ) ;
 return 0 ;
 }
 return 1 ;
 # else * al = SSL_AD_INTERNAL_ERROR ;
 SSLerr ( SSL_F_TLS_PROCESS_CKE_SRP , ERR_R_INTERNAL_ERROR ) ;
 return 0 ;
 # endif }