static int tls_process_cke_ecdhe ( SSL * s , PACKET * pkt , int * al ) {
 # ifndef OPENSSL_NO_EC EVP_PKEY * skey = s -> s3 -> tmp . pkey ;
 EVP_PKEY * ckey = NULL ;
 int ret = 0 ;
 if ( PACKET_remaining ( pkt ) == 0L ) {
 * al = SSL_AD_HANDSHAKE_FAILURE ;
 SSLerr ( SSL_F_TLS_PROCESS_CKE_ECDHE , SSL_R_MISSING_TMP_ECDH_KEY ) ;
 goto err ;
 }
 else {
 unsigned int i ;
 const unsigned char * data ;
 if ( ! PACKET_get_1 ( pkt , & i ) || ! PACKET_get_bytes ( pkt , & data , i ) || PACKET_remaining ( pkt ) != 0 ) {
 * al = SSL_AD_DECODE_ERROR ;
 SSLerr ( SSL_F_TLS_PROCESS_CKE_ECDHE , SSL_R_LENGTH_MISMATCH ) ;
 goto err ;
 }
 ckey = EVP_PKEY_new ( ) ;
 if ( ckey == NULL || EVP_PKEY_copy_parameters ( ckey , skey ) <= 0 ) {
 SSLerr ( SSL_F_TLS_PROCESS_CKE_ECDHE , ERR_R_EVP_LIB ) ;
 goto err ;
 }
 if ( EVP_PKEY_set1_tls_encodedpoint ( ckey , data , i ) == 0 ) {
 * al = SSL_AD_HANDSHAKE_FAILURE ;
 SSLerr ( SSL_F_TLS_PROCESS_CKE_ECDHE , ERR_R_EC_LIB ) ;
 goto err ;
 }
 }
 if ( ssl_derive ( s , skey , ckey ) == 0 ) {
 * al = SSL_AD_INTERNAL_ERROR ;
 SSLerr ( SSL_F_TLS_PROCESS_CKE_ECDHE , ERR_R_INTERNAL_ERROR ) ;
 goto err ;
 }
 ret = 1 ;
 EVP_PKEY_free ( s -> s3 -> tmp . pkey ) ;
 s -> s3 -> tmp . pkey = NULL ;
 err : EVP_PKEY_free ( ckey ) ;
 return ret ;
 # else * al = SSL_AD_INTERNAL_ERROR ;
 SSLerr ( SSL_F_TLS_PROCESS_CKE_ECDHE , ERR_R_INTERNAL_ERROR ) ;
 return 0 ;
 # endif }