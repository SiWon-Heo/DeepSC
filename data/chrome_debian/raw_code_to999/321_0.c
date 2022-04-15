static int tls_process_cke_dhe ( SSL * s , PACKET * pkt , int * al ) {
 # ifndef OPENSSL_NO_DH EVP_PKEY * skey = NULL ;
 DH * cdh ;
 unsigned int i ;
 BIGNUM * pub_key ;
 const unsigned char * data ;
 EVP_PKEY * ckey = NULL ;
 int ret = 0 ;
 if ( ! PACKET_get_net_2 ( pkt , & i ) || PACKET_remaining ( pkt ) != i ) {
 * al = SSL_AD_HANDSHAKE_FAILURE ;
 SSLerr ( SSL_F_TLS_PROCESS_CKE_DHE , SSL_R_DH_PUBLIC_VALUE_LENGTH_IS_WRONG ) ;
 goto err ;
 }
 skey = s -> s3 -> tmp . pkey ;
 if ( skey == NULL ) {
 * al = SSL_AD_HANDSHAKE_FAILURE ;
 SSLerr ( SSL_F_TLS_PROCESS_CKE_DHE , SSL_R_MISSING_TMP_DH_KEY ) ;
 goto err ;
 }
 if ( PACKET_remaining ( pkt ) == 0L ) {
 * al = SSL_AD_HANDSHAKE_FAILURE ;
 SSLerr ( SSL_F_TLS_PROCESS_CKE_DHE , SSL_R_MISSING_TMP_DH_KEY ) ;
 goto err ;
 }
 if ( ! PACKET_get_bytes ( pkt , & data , i ) ) {
 * al = SSL_AD_INTERNAL_ERROR ;
 SSLerr ( SSL_F_TLS_PROCESS_CKE_DHE , ERR_R_INTERNAL_ERROR ) ;
 goto err ;
 }
 ckey = EVP_PKEY_new ( ) ;
 if ( ckey == NULL || EVP_PKEY_copy_parameters ( ckey , skey ) == 0 ) {
 SSLerr ( SSL_F_TLS_PROCESS_CKE_DHE , SSL_R_BN_LIB ) ;
 goto err ;
 }
 cdh = EVP_PKEY_get0_DH ( ckey ) ;
 pub_key = BN_bin2bn ( data , i , NULL ) ;
 if ( pub_key == NULL || ! DH_set0_key ( cdh , pub_key , NULL ) ) {
 SSLerr ( SSL_F_TLS_PROCESS_CKE_DHE , ERR_R_INTERNAL_ERROR ) ;
 if ( pub_key != NULL ) BN_free ( pub_key ) ;
 goto err ;
 }
 if ( ssl_derive ( s , skey , ckey ) == 0 ) {
 * al = SSL_AD_INTERNAL_ERROR ;
 SSLerr ( SSL_F_TLS_PROCESS_CKE_DHE , ERR_R_INTERNAL_ERROR ) ;
 goto err ;
 }
 ret = 1 ;
 EVP_PKEY_free ( s -> s3 -> tmp . pkey ) ;
 s -> s3 -> tmp . pkey = NULL ;
 err : EVP_PKEY_free ( ckey ) ;
 return ret ;
 # else * al = SSL_AD_INTERNAL_ERROR ;
 SSLerr ( SSL_F_TLS_PROCESS_CKE_DHE , ERR_R_INTERNAL_ERROR ) ;
 return 0 ;
 # endif }