int tls_construct_new_session_ticket ( SSL * s ) {
 unsigned char * senc = NULL ;
 EVP_CIPHER_CTX * ctx ;
 HMAC_CTX * hctx = NULL ;
 unsigned char * p , * macstart ;
 const unsigned char * const_p ;
 int len , slen_full , slen ;
 SSL_SESSION * sess ;
 unsigned int hlen ;
 SSL_CTX * tctx = s -> initial_ctx ;
 unsigned char iv [ EVP_MAX_IV_LENGTH ] ;
 unsigned char key_name [ TLSEXT_KEYNAME_LENGTH ] ;
 int iv_len ;
 slen_full = i2d_SSL_SESSION ( s -> session , NULL ) ;
 if ( slen_full == 0 || slen_full > 0xFF00 ) {
 ossl_statem_set_error ( s ) ;
 return 0 ;
 }
 senc = OPENSSL_malloc ( slen_full ) ;
 if ( senc == NULL ) {
 ossl_statem_set_error ( s ) ;
 return 0 ;
 }
 ctx = EVP_CIPHER_CTX_new ( ) ;
 hctx = HMAC_CTX_new ( ) ;
 p = senc ;
 if ( ! i2d_SSL_SESSION ( s -> session , & p ) ) goto err ;
 const_p = senc ;
 sess = d2i_SSL_SESSION ( NULL , & const_p , slen_full ) ;
 if ( sess == NULL ) goto err ;
 sess -> session_id_length = 0 ;
 slen = i2d_SSL_SESSION ( sess , NULL ) ;
 if ( slen == 0 || slen > slen_full ) {
 SSL_SESSION_free ( sess ) ;
 goto err ;
 }
 p = senc ;
 if ( ! i2d_SSL_SESSION ( sess , & p ) ) {
 SSL_SESSION_free ( sess ) ;
 goto err ;
 }
 SSL_SESSION_free ( sess ) ;
 if ( ! BUF_MEM_grow ( s -> init_buf , SSL_HM_HEADER_LENGTH ( s ) + 6 + sizeof ( key_name ) + EVP_MAX_IV_LENGTH + EVP_MAX_BLOCK_LENGTH + EVP_MAX_MD_SIZE + slen ) ) goto err ;
 p = ssl_handshake_start ( s ) ;
 if ( tctx -> tlsext_ticket_key_cb ) {
 int ret = tctx -> tlsext_ticket_key_cb ( s , key_name , iv , ctx , hctx , 1 ) ;
 if ( ret == 0 ) {
 l2n ( 0 , p ) ;
 s2n ( 0 , p ) ;
 if ( ! ssl_set_handshake_header ( s , SSL3_MT_NEWSESSION_TICKET , p - ssl_handshake_start ( s ) ) ) goto err ;
 OPENSSL_free ( senc ) ;
 EVP_CIPHER_CTX_free ( ctx ) ;
 HMAC_CTX_free ( hctx ) ;
 return 1 ;
 }
 if ( ret < 0 ) goto err ;
 iv_len = EVP_CIPHER_CTX_iv_length ( ctx ) ;
 }
 else {
 const EVP_CIPHER * cipher = EVP_aes_256_cbc ( ) ;
 iv_len = EVP_CIPHER_iv_length ( cipher ) ;
 if ( RAND_bytes ( iv , iv_len ) <= 0 ) goto err ;
 if ( ! EVP_EncryptInit_ex ( ctx , cipher , NULL , tctx -> tlsext_tick_aes_key , iv ) ) goto err ;
 if ( ! HMAC_Init_ex ( hctx , tctx -> tlsext_tick_hmac_key , sizeof ( tctx -> tlsext_tick_hmac_key ) , EVP_sha256 ( ) , NULL ) ) goto err ;
 memcpy ( key_name , tctx -> tlsext_tick_key_name , sizeof ( tctx -> tlsext_tick_key_name ) ) ;
 }
 l2n ( s -> hit ? 0 : s -> session -> timeout , p ) ;
 p += 2 ;
 macstart = p ;
 memcpy ( p , key_name , sizeof ( key_name ) ) ;
 p += sizeof ( key_name ) ;
 memcpy ( p , iv , iv_len ) ;
 p += iv_len ;
 if ( ! EVP_EncryptUpdate ( ctx , p , & len , senc , slen ) ) goto err ;
 p += len ;
 if ( ! EVP_EncryptFinal ( ctx , p , & len ) ) goto err ;
 p += len ;
 if ( ! HMAC_Update ( hctx , macstart , p - macstart ) ) goto err ;
 if ( ! HMAC_Final ( hctx , p , & hlen ) ) goto err ;
 EVP_CIPHER_CTX_free ( ctx ) ;
 HMAC_CTX_free ( hctx ) ;
 ctx = NULL ;
 hctx = NULL ;
 p += hlen ;
 len = p - ssl_handshake_start ( s ) ;
 p = ssl_handshake_start ( s ) + 4 ;
 s2n ( len - 6 , p ) ;
 if ( ! ssl_set_handshake_header ( s , SSL3_MT_NEWSESSION_TICKET , len ) ) goto err ;
 OPENSSL_free ( senc ) ;
 return 1 ;
 err : OPENSSL_free ( senc ) ;
 EVP_CIPHER_CTX_free ( ctx ) ;
 HMAC_CTX_free ( hctx ) ;
 ossl_statem_set_error ( s ) ;
 return 0 ;
 }