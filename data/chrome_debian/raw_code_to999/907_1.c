static int chacha20_poly1305_ctrl ( EVP_CIPHER_CTX * ctx , int type , int arg , void * ptr ) {
 EVP_CHACHA_AEAD_CTX * actx = aead_data ( ctx ) ;
 switch ( type ) {
 case EVP_CTRL_INIT : if ( actx == NULL ) actx = ctx -> cipher_data = OPENSSL_zalloc ( sizeof ( * actx ) + Poly1305_ctx_size ( ) ) ;
 if ( actx == NULL ) {
 EVPerr ( EVP_F_CHACHA20_POLY1305_CTRL , EVP_R_INITIALIZATION_ERROR ) ;
 return 0 ;
 }
 actx -> len . aad = 0 ;
 actx -> len . text = 0 ;
 actx -> aad = 0 ;
 actx -> mac_inited = 0 ;
 actx -> tag_len = 0 ;
 actx -> nonce_len = 12 ;
 actx -> tls_payload_length = NO_TLS_PAYLOAD_LENGTH ;
 return 1 ;
 case EVP_CTRL_COPY : if ( actx ) {
 EVP_CIPHER_CTX * dst = ( EVP_CIPHER_CTX * ) ptr ;
 dst -> cipher_data = OPENSSL_memdup ( actx , sizeof ( * actx ) + Poly1305_ctx_size ( ) ) ;
 if ( dst -> cipher_data == NULL ) {
 EVPerr ( EVP_F_CHACHA20_POLY1305_CTRL , EVP_R_COPY_ERROR ) ;
 return 0 ;
 }
 }
 return 1 ;
 case EVP_CTRL_AEAD_SET_IVLEN : if ( arg <= 0 || arg > CHACHA_CTR_SIZE ) return 0 ;
 actx -> nonce_len = arg ;
 return 1 ;
 case EVP_CTRL_AEAD_SET_IV_FIXED : if ( arg != 12 ) return 0 ;
 actx -> nonce [ 0 ] = actx -> key . counter [ 1 ] = CHACHA_U8TOU32 ( ( unsigned char * ) ptr ) ;
 actx -> nonce [ 1 ] = actx -> key . counter [ 2 ] = CHACHA_U8TOU32 ( ( unsigned char * ) ptr + 4 ) ;
 actx -> nonce [ 2 ] = actx -> key . counter [ 3 ] = CHACHA_U8TOU32 ( ( unsigned char * ) ptr + 8 ) ;
 return 1 ;
 case EVP_CTRL_AEAD_SET_TAG : if ( arg <= 0 || arg > POLY1305_BLOCK_SIZE ) return 0 ;
 if ( ptr != NULL ) {
 memcpy ( actx -> tag , ptr , arg ) ;
 actx -> tag_len = arg ;
 }
 return 1 ;
 case EVP_CTRL_AEAD_GET_TAG : if ( arg <= 0 || arg > POLY1305_BLOCK_SIZE || ! ctx -> encrypt ) return 0 ;
 memcpy ( ptr , actx -> tag , arg ) ;
 return 1 ;
 case EVP_CTRL_AEAD_TLS1_AAD : if ( arg != EVP_AEAD_TLS1_AAD_LEN ) return 0 ;
 {
 unsigned int len ;
 unsigned char * aad = ptr , temp [ POLY1305_BLOCK_SIZE ] ;
 len = aad [ EVP_AEAD_TLS1_AAD_LEN - 2 ] << 8 | aad [ EVP_AEAD_TLS1_AAD_LEN - 1 ] ;
 if ( ! ctx -> encrypt ) {
 len -= POLY1305_BLOCK_SIZE ;
 memcpy ( temp , aad , EVP_AEAD_TLS1_AAD_LEN - 2 ) ;
 aad = temp ;
 temp [ EVP_AEAD_TLS1_AAD_LEN - 2 ] = ( unsigned char ) ( len >> 8 ) ;
 temp [ EVP_AEAD_TLS1_AAD_LEN - 1 ] = ( unsigned char ) len ;
 }
 actx -> tls_payload_length = len ;
 actx -> key . counter [ 1 ] = actx -> nonce [ 0 ] ;
 actx -> key . counter [ 2 ] = actx -> nonce [ 1 ] ^ CHACHA_U8TOU32 ( aad ) ;
 actx -> key . counter [ 3 ] = actx -> nonce [ 2 ] ^ CHACHA_U8TOU32 ( aad + 4 ) ;
 actx -> mac_inited = 0 ;
 chacha20_poly1305_cipher ( ctx , NULL , aad , EVP_AEAD_TLS1_AAD_LEN ) ;
 return POLY1305_BLOCK_SIZE ;
 }
 case EVP_CTRL_AEAD_SET_MAC_KEY : return 1 ;
 default : return - 1 ;
 }
 }