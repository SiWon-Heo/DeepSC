static int tls_process_cke_gost ( SSL * s , PACKET * pkt , int * al ) {
 # ifndef OPENSSL_NO_GOST EVP_PKEY_CTX * pkey_ctx ;
 EVP_PKEY * client_pub_pkey = NULL , * pk = NULL ;
 unsigned char premaster_secret [ 32 ] ;
 const unsigned char * start ;
 size_t outlen = 32 , inlen ;
 unsigned long alg_a ;
 int Ttag , Tclass ;
 long Tlen ;
 long sess_key_len ;
 const unsigned char * data ;
 int ret = 0 ;
 alg_a = s -> s3 -> tmp . new_cipher -> algorithm_auth ;
 if ( alg_a & SSL_aGOST12 ) {
 pk = s -> cert -> pkeys [ SSL_PKEY_GOST12_512 ] . privatekey ;
 if ( pk == NULL ) {
 pk = s -> cert -> pkeys [ SSL_PKEY_GOST12_256 ] . privatekey ;
 }
 if ( pk == NULL ) {
 pk = s -> cert -> pkeys [ SSL_PKEY_GOST01 ] . privatekey ;
 }
 }
 else if ( alg_a & SSL_aGOST01 ) {
 pk = s -> cert -> pkeys [ SSL_PKEY_GOST01 ] . privatekey ;
 }
 pkey_ctx = EVP_PKEY_CTX_new ( pk , NULL ) ;
 if ( pkey_ctx == NULL ) {
 * al = SSL_AD_INTERNAL_ERROR ;
 SSLerr ( SSL_F_TLS_PROCESS_CKE_GOST , ERR_R_MALLOC_FAILURE ) ;
 return 0 ;
 }
 if ( EVP_PKEY_decrypt_init ( pkey_ctx ) <= 0 ) {
 * al = SSL_AD_INTERNAL_ERROR ;
 SSLerr ( SSL_F_TLS_PROCESS_CKE_GOST , ERR_R_INTERNAL_ERROR ) ;
 return 0 ;
 }
 client_pub_pkey = X509_get0_pubkey ( s -> session -> peer ) ;
 if ( client_pub_pkey ) {
 if ( EVP_PKEY_derive_set_peer ( pkey_ctx , client_pub_pkey ) <= 0 ) ERR_clear_error ( ) ;
 }
 sess_key_len = PACKET_remaining ( pkt ) ;
 if ( ! PACKET_get_bytes ( pkt , & data , sess_key_len ) ) {
 * al = SSL_AD_INTERNAL_ERROR ;
 SSLerr ( SSL_F_TLS_PROCESS_CKE_GOST , ERR_R_INTERNAL_ERROR ) ;
 goto err ;
 }
 if ( ASN1_get_object ( ( const unsigned char * * ) & data , & Tlen , & Ttag , & Tclass , sess_key_len ) != V_ASN1_CONSTRUCTED || Ttag != V_ASN1_SEQUENCE || Tclass != V_ASN1_UNIVERSAL ) {
 * al = SSL_AD_DECODE_ERROR ;
 SSLerr ( SSL_F_TLS_PROCESS_CKE_GOST , SSL_R_DECRYPTION_FAILED ) ;
 goto err ;
 }
 start = data ;
 inlen = Tlen ;
 if ( EVP_PKEY_decrypt ( pkey_ctx , premaster_secret , & outlen , start , inlen ) <= 0 ) {
 * al = SSL_AD_DECODE_ERROR ;
 SSLerr ( SSL_F_TLS_PROCESS_CKE_GOST , SSL_R_DECRYPTION_FAILED ) ;
 goto err ;
 }
 if ( ! ssl_generate_master_secret ( s , premaster_secret , sizeof ( premaster_secret ) , 0 ) ) {
 * al = SSL_AD_INTERNAL_ERROR ;
 SSLerr ( SSL_F_TLS_PROCESS_CKE_GOST , ERR_R_INTERNAL_ERROR ) ;
 goto err ;
 }
 if ( EVP_PKEY_CTX_ctrl ( pkey_ctx , - 1 , - 1 , EVP_PKEY_CTRL_PEER_KEY , 2 , NULL ) > 0 ) s -> statem . no_cert_verify = 1 ;
 ret = 1 ;
 err : EVP_PKEY_CTX_free ( pkey_ctx ) ;
 return ret ;
 # else * al = SSL_AD_INTERNAL_ERROR ;
 SSLerr ( SSL_F_TLS_PROCESS_CKE_GOST , ERR_R_INTERNAL_ERROR ) ;
 return 0 ;
 # endif }