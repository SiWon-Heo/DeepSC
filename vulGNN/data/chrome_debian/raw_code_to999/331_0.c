int tls_construct_server_key_exchange ( SSL * s ) {
 # ifndef OPENSSL_NO_DH EVP_PKEY * pkdh = NULL ;
 int j ;
 # endif # ifndef OPENSSL_NO_EC unsigned char * encodedPoint = NULL ;
 int encodedlen = 0 ;
 int curve_id = 0 ;
 # endif EVP_PKEY * pkey ;
 const EVP_MD * md = NULL ;
 unsigned char * p , * d ;
 int al , i ;
 unsigned long type ;
 int n ;
 const BIGNUM * r [ 4 ] ;
 int nr [ 4 ] , kn ;
 BUF_MEM * buf ;
 EVP_MD_CTX * md_ctx = EVP_MD_CTX_new ( ) ;
 if ( md_ctx == NULL ) {
 SSLerr ( SSL_F_TLS_CONSTRUCT_SERVER_KEY_EXCHANGE , ERR_R_MALLOC_FAILURE ) ;
 al = SSL_AD_INTERNAL_ERROR ;
 goto f_err ;
 }
 type = s -> s3 -> tmp . new_cipher -> algorithm_mkey ;
 buf = s -> init_buf ;
 r [ 0 ] = r [ 1 ] = r [ 2 ] = r [ 3 ] = NULL ;
 n = 0 ;
 # ifndef OPENSSL_NO_PSK if ( type & SSL_PSK ) {
 n += 2 ;
 if ( s -> cert -> psk_identity_hint ) n += strlen ( s -> cert -> psk_identity_hint ) ;
 }
 if ( type & ( SSL_kPSK | SSL_kRSAPSK ) ) {
 }
 else # endif # ifndef OPENSSL_NO_DH if ( type & ( SSL_kDHE | SSL_kDHEPSK ) ) {
 CERT * cert = s -> cert ;
 EVP_PKEY * pkdhp = NULL ;
 DH * dh ;
 if ( s -> cert -> dh_tmp_auto ) {
 DH * dhp = ssl_get_auto_dh ( s ) ;
 pkdh = EVP_PKEY_new ( ) ;
 if ( pkdh == NULL || dhp == NULL ) {
 DH_free ( dhp ) ;
 al = SSL_AD_INTERNAL_ERROR ;
 SSLerr ( SSL_F_TLS_CONSTRUCT_SERVER_KEY_EXCHANGE , ERR_R_INTERNAL_ERROR ) ;
 goto f_err ;
 }
 EVP_PKEY_assign_DH ( pkdh , dhp ) ;
 pkdhp = pkdh ;
 }
 else {
 pkdhp = cert -> dh_tmp ;
 }
 if ( ( pkdhp == NULL ) && ( s -> cert -> dh_tmp_cb != NULL ) ) {
 DH * dhp = s -> cert -> dh_tmp_cb ( s , 0 , 1024 ) ;
 pkdh = ssl_dh_to_pkey ( dhp ) ;
 if ( pkdh == NULL ) {
 al = SSL_AD_INTERNAL_ERROR ;
 SSLerr ( SSL_F_TLS_CONSTRUCT_SERVER_KEY_EXCHANGE , ERR_R_INTERNAL_ERROR ) ;
 goto f_err ;
 }
 pkdhp = pkdh ;
 }
 if ( pkdhp == NULL ) {
 al = SSL_AD_HANDSHAKE_FAILURE ;
 SSLerr ( SSL_F_TLS_CONSTRUCT_SERVER_KEY_EXCHANGE , SSL_R_MISSING_TMP_DH_KEY ) ;
 goto f_err ;
 }
 if ( ! ssl_security ( s , SSL_SECOP_TMP_DH , EVP_PKEY_security_bits ( pkdhp ) , 0 , pkdhp ) ) {
 al = SSL_AD_HANDSHAKE_FAILURE ;
 SSLerr ( SSL_F_TLS_CONSTRUCT_SERVER_KEY_EXCHANGE , SSL_R_DH_KEY_TOO_SMALL ) ;
 goto f_err ;
 }
 if ( s -> s3 -> tmp . pkey != NULL ) {
 SSLerr ( SSL_F_TLS_CONSTRUCT_SERVER_KEY_EXCHANGE , ERR_R_INTERNAL_ERROR ) ;
 goto err ;
 }
 s -> s3 -> tmp . pkey = ssl_generate_pkey ( pkdhp ) ;
 if ( s -> s3 -> tmp . pkey == NULL ) {
 SSLerr ( SSL_F_TLS_CONSTRUCT_SERVER_KEY_EXCHANGE , ERR_R_EVP_LIB ) ;
 goto err ;
 }
 dh = EVP_PKEY_get0_DH ( s -> s3 -> tmp . pkey ) ;
 EVP_PKEY_free ( pkdh ) ;
 pkdh = NULL ;
 DH_get0_pqg ( dh , & r [ 0 ] , NULL , & r [ 1 ] ) ;
 DH_get0_key ( dh , & r [ 2 ] , NULL ) ;
 }
 else # endif # ifndef OPENSSL_NO_EC if ( type & ( SSL_kECDHE | SSL_kECDHEPSK ) ) {
 int nid ;
 if ( s -> s3 -> tmp . pkey != NULL ) {
 SSLerr ( SSL_F_TLS_CONSTRUCT_SERVER_KEY_EXCHANGE , ERR_R_INTERNAL_ERROR ) ;
 goto err ;
 }
 nid = tls1_shared_curve ( s , - 2 ) ;
 curve_id = tls1_ec_nid2curve_id ( nid ) ;
 if ( curve_id == 0 ) {
 SSLerr ( SSL_F_TLS_CONSTRUCT_SERVER_KEY_EXCHANGE , SSL_R_UNSUPPORTED_ELLIPTIC_CURVE ) ;
 goto err ;
 }
 s -> s3 -> tmp . pkey = ssl_generate_pkey_curve ( curve_id ) ;
 if ( s -> s3 -> tmp . pkey == NULL ) {
 al = SSL_AD_INTERNAL_ERROR ;
 SSLerr ( SSL_F_TLS_CONSTRUCT_SERVER_KEY_EXCHANGE , ERR_R_EVP_LIB ) ;
 goto f_err ;
 }
 encodedlen = EVP_PKEY_get1_tls_encodedpoint ( s -> s3 -> tmp . pkey , & encodedPoint ) ;
 if ( encodedlen == 0 ) {
 SSLerr ( SSL_F_TLS_CONSTRUCT_SERVER_KEY_EXCHANGE , ERR_R_EC_LIB ) ;
 goto err ;
 }
 n += 4 + encodedlen ;
 r [ 0 ] = NULL ;
 r [ 1 ] = NULL ;
 r [ 2 ] = NULL ;
 r [ 3 ] = NULL ;
 }
 else # endif # ifndef OPENSSL_NO_SRP if ( type & SSL_kSRP ) {
 if ( ( s -> srp_ctx . N == NULL ) || ( s -> srp_ctx . g == NULL ) || ( s -> srp_ctx . s == NULL ) || ( s -> srp_ctx . B == NULL ) ) {
 SSLerr ( SSL_F_TLS_CONSTRUCT_SERVER_KEY_EXCHANGE , SSL_R_MISSING_SRP_PARAM ) ;
 goto err ;
 }
 r [ 0 ] = s -> srp_ctx . N ;
 r [ 1 ] = s -> srp_ctx . g ;
 r [ 2 ] = s -> srp_ctx . s ;
 r [ 3 ] = s -> srp_ctx . B ;
 }
 else # endif {
 al = SSL_AD_HANDSHAKE_FAILURE ;
 SSLerr ( SSL_F_TLS_CONSTRUCT_SERVER_KEY_EXCHANGE , SSL_R_UNKNOWN_KEY_EXCHANGE_TYPE ) ;
 goto f_err ;
 }
 for ( i = 0 ;
 i < 4 && r [ i ] != NULL ;
 i ++ ) {
 nr [ i ] = BN_num_bytes ( r [ i ] ) ;
 # ifndef OPENSSL_NO_SRP if ( ( i == 2 ) && ( type & SSL_kSRP ) ) n += 1 + nr [ i ] ;
 else # endif # ifndef OPENSSL_NO_DH if ( ( i == 2 ) && ( type & ( SSL_kDHE | SSL_kDHEPSK ) ) ) n += 2 + nr [ 0 ] ;
 else # endif n += 2 + nr [ i ] ;
 }
 if ( ! ( s -> s3 -> tmp . new_cipher -> algorithm_auth & ( SSL_aNULL | SSL_aSRP ) ) && ! ( s -> s3 -> tmp . new_cipher -> algorithm_mkey & SSL_PSK ) ) {
 if ( ( pkey = ssl_get_sign_pkey ( s , s -> s3 -> tmp . new_cipher , & md ) ) == NULL ) {
 al = SSL_AD_DECODE_ERROR ;
 goto f_err ;
 }
 kn = EVP_PKEY_size ( pkey ) ;
 if ( SSL_USE_SIGALGS ( s ) ) kn += 2 ;
 kn += 2 ;
 }
 else {
 pkey = NULL ;
 kn = 0 ;
 }
 if ( ! BUF_MEM_grow_clean ( buf , n + SSL_HM_HEADER_LENGTH ( s ) + kn ) ) {
 SSLerr ( SSL_F_TLS_CONSTRUCT_SERVER_KEY_EXCHANGE , ERR_LIB_BUF ) ;
 goto err ;
 }
 d = p = ssl_handshake_start ( s ) ;
 # ifndef OPENSSL_NO_PSK if ( type & SSL_PSK ) {
 if ( s -> cert -> psk_identity_hint ) {
 size_t len = strlen ( s -> cert -> psk_identity_hint ) ;
 if ( len > PSK_MAX_IDENTITY_LEN ) {
 SSLerr ( SSL_F_TLS_CONSTRUCT_SERVER_KEY_EXCHANGE , ERR_R_INTERNAL_ERROR ) ;
 goto err ;
 }
 s2n ( len , p ) ;
 memcpy ( p , s -> cert -> psk_identity_hint , len ) ;
 p += len ;
 }
 else {
 s2n ( 0 , p ) ;
 }
 }
 # endif for ( i = 0 ;
 i < 4 && r [ i ] != NULL ;
 i ++ ) {
 # ifndef OPENSSL_NO_SRP if ( ( i == 2 ) && ( type & SSL_kSRP ) ) {
 * p = nr [ i ] ;
 p ++ ;
 }
 else # endif # ifndef OPENSSL_NO_DH if ( ( i == 2 ) && ( type & ( SSL_kDHE | SSL_kDHEPSK ) ) ) {
 s2n ( nr [ 0 ] , p ) ;
 for ( j = 0 ;
 j < ( nr [ 0 ] - nr [ 2 ] ) ;
 ++ j ) {
 * p = 0 ;
 ++ p ;
 }
 }
 else # endif s2n ( nr [ i ] , p ) ;
 BN_bn2bin ( r [ i ] , p ) ;
 p += nr [ i ] ;
 }
 # ifndef OPENSSL_NO_EC if ( type & ( SSL_kECDHE | SSL_kECDHEPSK ) ) {
 * p = NAMED_CURVE_TYPE ;
 p += 1 ;
 * p = 0 ;
 p += 1 ;
 * p = curve_id ;
 p += 1 ;
 * p = encodedlen ;
 p += 1 ;
 memcpy ( p , encodedPoint , encodedlen ) ;
 OPENSSL_free ( encodedPoint ) ;
 encodedPoint = NULL ;
 p += encodedlen ;
 }
 # endif if ( pkey != NULL ) {
 if ( md ) {
 if ( SSL_USE_SIGALGS ( s ) ) {
 if ( ! tls12_get_sigandhash ( p , pkey , md ) ) {
 al = SSL_AD_INTERNAL_ERROR ;
 SSLerr ( SSL_F_TLS_CONSTRUCT_SERVER_KEY_EXCHANGE , ERR_R_INTERNAL_ERROR ) ;
 goto f_err ;
 }
 p += 2 ;
 }
 # ifdef SSL_DEBUG fprintf ( stderr , "Using hash %s\n" , EVP_MD_name ( md ) ) ;
 # endif if ( EVP_SignInit_ex ( md_ctx , md , NULL ) <= 0 || EVP_SignUpdate ( md_ctx , & ( s -> s3 -> client_random [ 0 ] ) , SSL3_RANDOM_SIZE ) <= 0 || EVP_SignUpdate ( md_ctx , & ( s -> s3 -> server_random [ 0 ] ) , SSL3_RANDOM_SIZE ) <= 0 || EVP_SignUpdate ( md_ctx , d , n ) <= 0 || EVP_SignFinal ( md_ctx , & ( p [ 2 ] ) , ( unsigned int * ) & i , pkey ) <= 0 ) {
 SSLerr ( SSL_F_TLS_CONSTRUCT_SERVER_KEY_EXCHANGE , ERR_LIB_EVP ) ;
 al = SSL_AD_INTERNAL_ERROR ;
 goto f_err ;
 }
 s2n ( i , p ) ;
 n += i + 2 ;
 if ( SSL_USE_SIGALGS ( s ) ) n += 2 ;
 }
 else {
 al = SSL_AD_HANDSHAKE_FAILURE ;
 SSLerr ( SSL_F_TLS_CONSTRUCT_SERVER_KEY_EXCHANGE , SSL_R_UNKNOWN_PKEY_TYPE ) ;
 goto f_err ;
 }
 }
 if ( ! ssl_set_handshake_header ( s , SSL3_MT_SERVER_KEY_EXCHANGE , n ) ) {
 al = SSL_AD_HANDSHAKE_FAILURE ;
 SSLerr ( SSL_F_TLS_CONSTRUCT_SERVER_KEY_EXCHANGE , ERR_R_INTERNAL_ERROR ) ;
 goto f_err ;
 }
 EVP_MD_CTX_free ( md_ctx ) ;
 return 1 ;
 f_err : ssl3_send_alert ( s , SSL3_AL_FATAL , al ) ;
 err : # ifndef OPENSSL_NO_DH EVP_PKEY_free ( pkdh ) ;
 # endif # ifndef OPENSSL_NO_EC OPENSSL_free ( encodedPoint ) ;
 # endif EVP_MD_CTX_free ( md_ctx ) ;
 ossl_statem_set_error ( s ) ;
 return 0 ;
 }