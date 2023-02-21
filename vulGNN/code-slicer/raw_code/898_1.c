const EVP_CIPHER * EVP_aes_ ## keylen ## _ ## mode ( void ) \ {
 return & aes_ ## keylen ## _ ## mode ;
 }
 # endif # if defined ( OPENSSL_CPUID_OBJ ) && ( defined ( __arm__ ) || defined ( __arm ) || defined ( __aarch64__ ) ) # include "arm_arch.h" # if __ARM_MAX_ARCH__ >= 7 # if defined ( BSAES_ASM ) # define BSAES_CAPABLE ( OPENSSL_armcap_P & ARMV7_NEON ) # endif # if defined ( VPAES_ASM ) # define VPAES_CAPABLE ( OPENSSL_armcap_P & ARMV7_NEON ) # endif # define HWAES_CAPABLE ( OPENSSL_armcap_P & ARMV8_AES ) # define HWAES_set_encrypt_key aes_v8_set_encrypt_key # define HWAES_set_decrypt_key aes_v8_set_decrypt_key # define HWAES_encrypt aes_v8_encrypt # define HWAES_decrypt aes_v8_decrypt # define HWAES_cbc_encrypt aes_v8_cbc_encrypt # define HWAES_ctr32_encrypt_blocks aes_v8_ctr32_encrypt_blocks # endif # endif # if defined ( HWAES_CAPABLE ) int HWAES_set_encrypt_key ( const unsigned char * userKey , const int bits , AES_KEY * key ) ;
 int HWAES_set_decrypt_key ( const unsigned char * userKey , const int bits , AES_KEY * key ) ;
 void HWAES_encrypt ( const unsigned char * in , unsigned char * out , const AES_KEY * key ) ;
 void HWAES_decrypt ( const unsigned char * in , unsigned char * out , const AES_KEY * key ) ;
 void HWAES_cbc_encrypt ( const unsigned char * in , unsigned char * out , size_t length , const AES_KEY * key , unsigned char * ivec , const int enc ) ;
 void HWAES_ctr32_encrypt_blocks ( const unsigned char * in , unsigned char * out , size_t len , const AES_KEY * key , const unsigned char ivec [ 16 ] ) ;
 void HWAES_xts_encrypt ( const unsigned char * inp , unsigned char * out , size_t len , const AES_KEY * key1 , const AES_KEY * key2 , const unsigned char iv [ 16 ] ) ;
 void HWAES_xts_decrypt ( const unsigned char * inp , unsigned char * out , size_t len , const AES_KEY * key1 , const AES_KEY * key2 , const unsigned char iv [ 16 ] ) ;
 # endif # define BLOCK_CIPHER_generic_pack ( nid , keylen , flags ) \ BLOCK_CIPHER_generic ( nid , keylen , 16 , 16 , cbc , cbc , CBC , flags | EVP_CIPH_FLAG_DEFAULT_ASN1 ) \ BLOCK_CIPHER_generic ( nid , keylen , 16 , 0 , ecb , ecb , ECB , flags | EVP_CIPH_FLAG_DEFAULT_ASN1 ) \ BLOCK_CIPHER_generic ( nid , keylen , 1 , 16 , ofb128 , ofb , OFB , flags | EVP_CIPH_FLAG_DEFAULT_ASN1 ) \ BLOCK_CIPHER_generic ( nid , keylen , 1 , 16 , cfb128 , cfb , CFB , flags | EVP_CIPH_FLAG_DEFAULT_ASN1 ) \ BLOCK_CIPHER_generic ( nid , keylen , 1 , 16 , cfb1 , cfb1 , CFB , flags ) \ BLOCK_CIPHER_generic ( nid , keylen , 1 , 16 , cfb8 , cfb8 , CFB , flags ) \ BLOCK_CIPHER_generic ( nid , keylen , 1 , 16 , ctr , ctr , CTR , flags ) static int aes_init_key ( EVP_CIPHER_CTX * ctx , const unsigned char * key , const unsigned char * iv , int enc ) {
 int ret , mode ;
 EVP_AES_KEY * dat = EVP_C_DATA ( EVP_AES_KEY , ctx ) ;
 mode = EVP_CIPHER_CTX_mode ( ctx ) ;
 if ( ( mode == EVP_CIPH_ECB_MODE || mode == EVP_CIPH_CBC_MODE ) && ! enc ) {
 # ifdef HWAES_CAPABLE if ( HWAES_CAPABLE ) {
 ret = HWAES_set_decrypt_key ( key , EVP_CIPHER_CTX_key_length ( ctx ) * 8 , & dat -> ks . ks ) ;
 dat -> block = ( block128_f ) HWAES_decrypt ;
 dat -> stream . cbc = NULL ;
 # ifdef HWAES_cbc_encrypt if ( mode == EVP_CIPH_CBC_MODE ) dat -> stream . cbc = ( cbc128_f ) HWAES_cbc_encrypt ;
 # endif }
 else # endif # ifdef BSAES_CAPABLE if ( BSAES_CAPABLE && mode == EVP_CIPH_CBC_MODE ) {
 ret = AES_set_decrypt_key ( key , EVP_CIPHER_CTX_key_length ( ctx ) * 8 , & dat -> ks . ks ) ;
 dat -> block = ( block128_f ) AES_decrypt ;
 dat -> stream . cbc = ( cbc128_f ) bsaes_cbc_encrypt ;
 }
 else # endif # ifdef VPAES_CAPABLE if ( VPAES_CAPABLE ) {
 ret = vpaes_set_decrypt_key ( key , EVP_CIPHER_CTX_key_length ( ctx ) * 8 , & dat -> ks . ks ) ;
 dat -> block = ( block128_f ) vpaes_decrypt ;
 dat -> stream . cbc = mode == EVP_CIPH_CBC_MODE ? ( cbc128_f ) vpaes_cbc_encrypt : NULL ;
 }
 else # endif {
 ret = AES_set_decrypt_key ( key , EVP_CIPHER_CTX_key_length ( ctx ) * 8 , & dat -> ks . ks ) ;
 dat -> block = ( block128_f ) AES_decrypt ;
 dat -> stream . cbc = mode == EVP_CIPH_CBC_MODE ? ( cbc128_f ) AES_cbc_encrypt : NULL ;
 }
 }
 else # ifdef HWAES_CAPABLE if ( HWAES_CAPABLE ) {
 ret = HWAES_set_encrypt_key ( key , EVP_CIPHER_CTX_key_length ( ctx ) * 8 , & dat -> ks . ks ) ;
 dat -> block = ( block128_f ) HWAES_encrypt ;
 dat -> stream . cbc = NULL ;
 # ifdef HWAES_cbc_encrypt if ( mode == EVP_CIPH_CBC_MODE ) dat -> stream . cbc = ( cbc128_f ) HWAES_cbc_encrypt ;
 else # endif # ifdef HWAES_ctr32_encrypt_blocks if ( mode == EVP_CIPH_CTR_MODE ) dat -> stream . ctr = ( ctr128_f ) HWAES_ctr32_encrypt_blocks ;
 else # endif ( void ) 0 ;
 }
 else # endif # ifdef BSAES_CAPABLE if ( BSAES_CAPABLE && mode == EVP_CIPH_CTR_MODE ) {
 ret = AES_set_encrypt_key ( key , EVP_CIPHER_CTX_key_length ( ctx ) * 8 , & dat -> ks . ks ) ;
 dat -> block = ( block128_f ) AES_encrypt ;
 dat -> stream . ctr = ( ctr128_f ) bsaes_ctr32_encrypt_blocks ;
 }
 else # endif # ifdef VPAES_CAPABLE if ( VPAES_CAPABLE ) {
 ret = vpaes_set_encrypt_key ( key , EVP_CIPHER_CTX_key_length ( ctx ) * 8 , & dat -> ks . ks ) ;
 dat -> block = ( block128_f ) vpaes_encrypt ;
 dat -> stream . cbc = mode == EVP_CIPH_CBC_MODE ? ( cbc128_f ) vpaes_cbc_encrypt : NULL ;
 }
 else # endif {
 ret = AES_set_encrypt_key ( key , EVP_CIPHER_CTX_key_length ( ctx ) * 8 , & dat -> ks . ks ) ;
 dat -> block = ( block128_f ) AES_encrypt ;
 dat -> stream . cbc = mode == EVP_CIPH_CBC_MODE ? ( cbc128_f ) AES_cbc_encrypt : NULL ;
 # ifdef AES_CTR_ASM if ( mode == EVP_CIPH_CTR_MODE ) dat -> stream . ctr = ( ctr128_f ) AES_ctr32_encrypt ;
 # endif }
 if ( ret < 0 ) {
 EVPerr ( EVP_F_AES_INIT_KEY , EVP_R_AES_KEY_SETUP_FAILED ) ;
 return 0 ;
 }
 return 1 ;
 }
 static int aes_cbc_cipher ( EVP_CIPHER_CTX * ctx , unsigned char * out , const unsigned char * in , size_t len ) {
 EVP_AES_KEY * dat = EVP_C_DATA ( EVP_AES_KEY , ctx ) ;
 if ( dat -> stream . cbc ) ( * dat -> stream . cbc ) ( in , out , len , & dat -> ks , EVP_CIPHER_CTX_iv_noconst ( ctx ) , EVP_CIPHER_CTX_encrypting ( ctx ) ) ;
 else if ( EVP_CIPHER_CTX_encrypting ( ctx ) ) CRYPTO_cbc128_encrypt ( in , out , len , & dat -> ks , EVP_CIPHER_CTX_iv_noconst ( ctx ) , dat -> block ) ;
 else CRYPTO_cbc128_decrypt ( in , out , len , & dat -> ks , EVP_CIPHER_CTX_iv_noconst ( ctx ) , dat -> block ) ;
 return 1 ;
 }
 static int aes_ecb_cipher ( EVP_CIPHER_CTX * ctx , unsigned char * out , const unsigned char * in , size_t len ) {
 size_t bl = EVP_CIPHER_CTX_block_size ( ctx ) ;
 size_t i ;
 EVP_AES_KEY * dat = EVP_C_DATA ( EVP_AES_KEY , ctx ) ;
 if ( len < bl ) return 1 ;
 for ( i = 0 , len -= bl ;
 i <= len ;
 i += bl ) ( * dat -> block ) ( in + i , out + i , & dat -> ks ) ;
 return 1 ;
 }
 static int aes_ofb_cipher ( EVP_CIPHER_CTX * ctx , unsigned char * out , const unsigned char * in , size_t len ) {
 EVP_AES_KEY * dat = EVP_C_DATA ( EVP_AES_KEY , ctx ) ;
 int num = EVP_CIPHER_CTX_num ( ctx ) ;
 CRYPTO_ofb128_encrypt ( in , out , len , & dat -> ks , EVP_CIPHER_CTX_iv_noconst ( ctx ) , & num , dat -> block ) ;
 EVP_CIPHER_CTX_set_num ( ctx , num ) ;
 return 1 ;
 }
 static int aes_cfb_cipher ( EVP_CIPHER_CTX * ctx , unsigned char * out , const unsigned char * in , size_t len ) {
 EVP_AES_KEY * dat = EVP_C_DATA ( EVP_AES_KEY , ctx ) ;
 int num = EVP_CIPHER_CTX_num ( ctx ) ;
 CRYPTO_cfb128_encrypt ( in , out , len , & dat -> ks , EVP_CIPHER_CTX_iv_noconst ( ctx ) , & num , EVP_CIPHER_CTX_encrypting ( ctx ) , dat -> block ) ;
 EVP_CIPHER_CTX_set_num ( ctx , num ) ;
 return 1 ;
 }
 static int aes_cfb8_cipher ( EVP_CIPHER_CTX * ctx , unsigned char * out , const unsigned char * in , size_t len ) {
 EVP_AES_KEY * dat = EVP_C_DATA ( EVP_AES_KEY , ctx ) ;
 int num = EVP_CIPHER_CTX_num ( ctx ) ;
 CRYPTO_cfb128_8_encrypt ( in , out , len , & dat -> ks , EVP_CIPHER_CTX_iv_noconst ( ctx ) , & num , EVP_CIPHER_CTX_encrypting ( ctx ) , dat -> block ) ;
 EVP_CIPHER_CTX_set_num ( ctx , num ) ;
 return 1 ;
 }
 static int aes_cfb1_cipher ( EVP_CIPHER_CTX * ctx , unsigned char * out , const unsigned char * in , size_t len ) {
 EVP_AES_KEY * dat = EVP_C_DATA ( EVP_AES_KEY , ctx ) ;
 if ( EVP_CIPHER_CTX_test_flags ( ctx , EVP_CIPH_FLAG_LENGTH_BITS ) ) {
 int num = EVP_CIPHER_CTX_num ( ctx ) ;
 CRYPTO_cfb128_1_encrypt ( in , out , len , & dat -> ks , EVP_CIPHER_CTX_iv_noconst ( ctx ) , & num , EVP_CIPHER_CTX_encrypting ( ctx ) , dat -> block ) ;
 EVP_CIPHER_CTX_set_num ( ctx , num ) ;
 return 1 ;
 }
 while ( len >= MAXBITCHUNK ) {
 int num = EVP_CIPHER_CTX_num ( ctx ) ;
 CRYPTO_cfb128_1_encrypt ( in , out , MAXBITCHUNK * 8 , & dat -> ks , EVP_CIPHER_CTX_iv_noconst ( ctx ) , & num , EVP_CIPHER_CTX_encrypting ( ctx ) , dat -> block ) ;
 EVP_CIPHER_CTX_set_num ( ctx , num ) ;
 len -= MAXBITCHUNK ;
 }
 if ( len ) {
 int num = EVP_CIPHER_CTX_num ( ctx ) ;
 CRYPTO_cfb128_1_encrypt ( in , out , len * 8 , & dat -> ks , EVP_CIPHER_CTX_iv_noconst ( ctx ) , & num , EVP_CIPHER_CTX_encrypting ( ctx ) , dat -> block ) ;
 EVP_CIPHER_CTX_set_num ( ctx , num ) ;
 }
 return 1 ;
 }
 static int aes_ctr_cipher ( EVP_CIPHER_CTX * ctx , unsigned char * out , const unsigned char * in , size_t len ) {
 unsigned int num = EVP_CIPHER_CTX_num ( ctx ) ;
 EVP_AES_KEY * dat = EVP_C_DATA ( EVP_AES_KEY , ctx ) ;
 if ( dat -> stream . ctr ) CRYPTO_ctr128_encrypt_ctr32 ( in , out , len , & dat -> ks , EVP_CIPHER_CTX_iv_noconst ( ctx ) , EVP_CIPHER_CTX_buf_noconst ( ctx ) , & num , dat -> stream . ctr ) ;
 else CRYPTO_ctr128_encrypt ( in , out , len , & dat -> ks , EVP_CIPHER_CTX_iv_noconst ( ctx ) , EVP_CIPHER_CTX_buf_noconst ( ctx ) , & num , dat -> block ) ;
 EVP_CIPHER_CTX_set_num ( ctx , num ) ;
 return 1 ;
 }
 BLOCK_CIPHER_generic_pack ( NID_aes , 128 , 0 ) BLOCK_CIPHER_generic_pack ( NID_aes , 192 , 0 ) BLOCK_CIPHER_generic_pack ( NID_aes , 256 , 0 ) static int aes_gcm_cleanup ( EVP_CIPHER_CTX * c ) {
 EVP_AES_GCM_CTX * gctx = EVP_C_DATA ( EVP_AES_GCM_CTX , c ) ;
 OPENSSL_cleanse ( & gctx -> gcm , sizeof ( gctx -> gcm ) ) ;
 if ( gctx -> iv != EVP_CIPHER_CTX_iv_noconst ( c ) ) OPENSSL_free ( gctx -> iv ) ;
 return 1 ;
 }
 static void ctr64_inc ( unsigned char * counter ) {
 int n = 8 ;
 unsigned char c ;
 do {
 -- n ;
 c = counter [ n ] ;
 ++ c ;
 counter [ n ] = c ;
 if ( c ) return ;
 }
 while ( n ) ;
 }
 static int aes_gcm_ctrl ( EVP_CIPHER_CTX * c , int type , int arg , void * ptr ) {
 EVP_AES_GCM_CTX * gctx = EVP_C_DATA ( EVP_AES_GCM_CTX , c ) ;
 switch ( type ) {
 case EVP_CTRL_INIT : gctx -> key_set = 0 ;
 gctx -> iv_set = 0 ;
 gctx -> ivlen = EVP_CIPHER_CTX_iv_length ( c ) ;
 gctx -> iv = EVP_CIPHER_CTX_iv_noconst ( c ) ;
 gctx -> taglen = - 1 ;
 gctx -> iv_gen = 0 ;
 gctx -> tls_aad_len = - 1 ;
 return 1 ;
 case EVP_CTRL_AEAD_SET_IVLEN : if ( arg <= 0 ) return 0 ;
 if ( ( arg > EVP_MAX_IV_LENGTH ) && ( arg > gctx -> ivlen ) ) {
 if ( gctx -> iv != EVP_CIPHER_CTX_iv_noconst ( c ) ) OPENSSL_free ( gctx -> iv ) ;
 gctx -> iv = OPENSSL_malloc ( arg ) ;
 if ( gctx -> iv == NULL ) return 0 ;
 }
 gctx -> ivlen = arg ;
 return 1 ;
 case EVP_CTRL_AEAD_SET_TAG : if ( arg <= 0 || arg > 16 || EVP_CIPHER_CTX_encrypting ( c ) ) return 0 ;
 memcpy ( EVP_CIPHER_CTX_buf_noconst ( c ) , ptr , arg ) ;
 gctx -> taglen = arg ;
 return 1 ;
 case EVP_CTRL_AEAD_GET_TAG : if ( arg <= 0 || arg > 16 || ! EVP_CIPHER_CTX_encrypting ( c ) || gctx -> taglen < 0 ) return 0 ;
 memcpy ( ptr , EVP_CIPHER_CTX_buf_noconst ( c ) , arg ) ;
 return 1 ;
 case EVP_CTRL_GCM_SET_IV_FIXED : if ( arg == - 1 ) {
 memcpy ( gctx -> iv , ptr , gctx -> ivlen ) ;
 gctx -> iv_gen = 1 ;
 return 1 ;
 }
 if ( ( arg < 4 ) || ( gctx -> ivlen - arg ) < 8 ) return 0 ;
 if ( arg ) memcpy ( gctx -> iv , ptr , arg ) ;
 if ( EVP_CIPHER_CTX_encrypting ( c ) && RAND_bytes ( gctx -> iv + arg , gctx -> ivlen - arg ) <= 0 ) return 0 ;
 gctx -> iv_gen = 1 ;
 return 1 ;
 case EVP_CTRL_GCM_IV_GEN : if ( gctx -> iv_gen == 0 || gctx -> key_set == 0 ) return 0 ;
 CRYPTO_gcm128_setiv ( & gctx -> gcm , gctx -> iv , gctx -> ivlen ) ;
 if ( arg <= 0 || arg > gctx -> ivlen ) arg = gctx -> ivlen ;
 memcpy ( ptr , gctx -> iv + gctx -> ivlen - arg , arg ) ;
 ctr64_inc ( gctx -> iv + gctx -> ivlen - 8 ) ;
 gctx -> iv_set = 1 ;
 return 1 ;
 case EVP_CTRL_GCM_SET_IV_INV : if ( gctx -> iv_gen == 0 || gctx -> key_set == 0 || EVP_CIPHER_CTX_encrypting ( c ) ) return 0 ;
 memcpy ( gctx -> iv + gctx -> ivlen - arg , ptr , arg ) ;
 CRYPTO_gcm128_setiv ( & gctx -> gcm , gctx -> iv , gctx -> ivlen ) ;
 gctx -> iv_set = 1 ;
 return 1 ;
 case EVP_CTRL_AEAD_TLS1_AAD : if ( arg != EVP_AEAD_TLS1_AAD_LEN ) return 0 ;
 memcpy ( EVP_CIPHER_CTX_buf_noconst ( c ) , ptr , arg ) ;
 gctx -> tls_aad_len = arg ;
 {
 unsigned int len = EVP_CIPHER_CTX_buf_noconst ( c ) [ arg - 2 ] << 8 | EVP_CIPHER_CTX_buf_noconst ( c ) [ arg - 1 ] ;
 len -= EVP_GCM_TLS_EXPLICIT_IV_LEN ;
 if ( ! EVP_CIPHER_CTX_encrypting ( c ) ) len -= EVP_GCM_TLS_TAG_LEN ;
 EVP_CIPHER_CTX_buf_noconst ( c ) [ arg - 2 ] = len >> 8 ;
 EVP_CIPHER_CTX_buf_noconst ( c ) [ arg - 1 ] = len & 0xff ;
 }
 return EVP_GCM_TLS_TAG_LEN ;
 case EVP_CTRL_COPY : {
 EVP_CIPHER_CTX * out = ptr ;
 EVP_AES_GCM_CTX * gctx_out = EVP_C_DATA ( EVP_AES_GCM_CTX , out ) ;
 if ( gctx -> gcm . key ) {
 if ( gctx -> gcm . key != & gctx -> ks ) return 0 ;
 gctx_out -> gcm . key = & gctx_out -> ks ;
 }
 if ( gctx -> iv == EVP_CIPHER_CTX_iv_noconst ( c ) ) gctx_out -> iv = EVP_CIPHER_CTX_iv_noconst ( out ) ;
 else {
 gctx_out -> iv = OPENSSL_malloc ( gctx -> ivlen ) ;
 if ( gctx_out -> iv == NULL ) return 0 ;
 memcpy ( gctx_out -> iv , gctx -> iv , gctx -> ivlen ) ;
 }
 return 1 ;
 }
 default : return - 1 ;
 }
 }
 static int aes_gcm_init_key ( EVP_CIPHER_CTX * ctx , const unsigned char * key , const unsigned char * iv , int enc ) {
 EVP_AES_GCM_CTX * gctx = EVP_C_DATA ( EVP_AES_GCM_CTX , ctx ) ;
 if ( ! iv && ! key ) return 1 ;
 if ( key ) {
 do {
 # ifdef HWAES_CAPABLE if ( HWAES_CAPABLE ) {
 HWAES_set_encrypt_key ( key , EVP_CIPHER_CTX_key_length ( ctx ) * 8 , & gctx -> ks . ks ) ;
 CRYPTO_gcm128_init ( & gctx -> gcm , & gctx -> ks , ( block128_f ) HWAES_encrypt ) ;
 # ifdef HWAES_ctr32_encrypt_blocks gctx -> ctr = ( ctr128_f ) HWAES_ctr32_encrypt_blocks ;
 # else gctx -> ctr = NULL ;
 # endif break ;
 }
 else # endif # ifdef BSAES_CAPABLE if ( BSAES_CAPABLE ) {
 AES_set_encrypt_key ( key , EVP_CIPHER_CTX_key_length ( ctx ) * 8 , & gctx -> ks . ks ) ;
 CRYPTO_gcm128_init ( & gctx -> gcm , & gctx -> ks , ( block128_f ) AES_encrypt ) ;
 gctx -> ctr = ( ctr128_f ) bsaes_ctr32_encrypt_blocks ;
 break ;
 }
 else # endif # ifdef VPAES_CAPABLE if ( VPAES_CAPABLE ) {
 vpaes_set_encrypt_key ( key , EVP_CIPHER_CTX_key_length ( ctx ) * 8 , & gctx -> ks . ks ) ;
 CRYPTO_gcm128_init ( & gctx -> gcm , & gctx -> ks , ( block128_f ) vpaes_encrypt ) ;
 gctx -> ctr = NULL ;
 break ;
 }
 else # endif ( void ) 0 ;
 AES_set_encrypt_key ( key , EVP_CIPHER_CTX_key_length ( ctx ) * 8 , & gctx -> ks . ks ) ;
 CRYPTO_gcm128_init ( & gctx -> gcm , & gctx -> ks , ( block128_f ) AES_encrypt ) ;
 # ifdef AES_CTR_ASM gctx -> ctr = ( ctr128_f ) AES_ctr32_encrypt ;
 # else gctx -> ctr = NULL ;
 # endif }
 while ( 0 ) ;
 if ( iv == NULL && gctx -> iv_set ) iv = gctx -> iv ;
 if ( iv ) {
 CRYPTO_gcm128_setiv ( & gctx -> gcm , iv , gctx -> ivlen ) ;
 gctx -> iv_set = 1 ;
 }
 gctx -> key_set = 1 ;
 }
 else {
 if ( gctx -> key_set ) CRYPTO_gcm128_setiv ( & gctx -> gcm , iv , gctx -> ivlen ) ;
 else memcpy ( gctx -> iv , iv , gctx -> ivlen ) ;
 gctx -> iv_set = 1 ;
 gctx -> iv_gen = 0 ;
 }
 return 1 ;
 }
 static int aes_gcm_tls_cipher ( EVP_CIPHER_CTX * ctx , unsigned char * out , const unsigned char * in , size_t len ) {
 EVP_AES_GCM_CTX * gctx = EVP_C_DATA ( EVP_AES_GCM_CTX , ctx ) ;
 int rv = - 1 ;
 if ( out != in || len < ( EVP_GCM_TLS_EXPLICIT_IV_LEN + EVP_GCM_TLS_TAG_LEN ) ) return - 1 ;
 if ( EVP_CIPHER_CTX_ctrl ( ctx , EVP_CIPHER_CTX_encrypting ( ctx ) ? EVP_CTRL_GCM_IV_GEN : EVP_CTRL_GCM_SET_IV_INV , EVP_GCM_TLS_EXPLICIT_IV_LEN , out ) <= 0 ) goto err ;
 if ( CRYPTO_gcm128_aad ( & gctx -> gcm , EVP_CIPHER_CTX_buf_noconst ( ctx ) , gctx -> tls_aad_len ) ) goto err ;
 in += EVP_GCM_TLS_EXPLICIT_IV_LEN ;
 out += EVP_GCM_TLS_EXPLICIT_IV_LEN ;
 len -= EVP_GCM_TLS_EXPLICIT_IV_LEN + EVP_GCM_TLS_TAG_LEN ;
 if ( EVP_CIPHER_CTX_encrypting ( ctx ) ) {
 if ( gctx -> ctr ) {
 size_t bulk = 0 ;
 # if defined ( AES_GCM_ASM ) if ( len >= 32 && AES_GCM_ASM ( gctx ) ) {
 if ( CRYPTO_gcm128_encrypt ( & gctx -> gcm , NULL , NULL , 0 ) ) return - 1 ;
 bulk = AES_gcm_encrypt ( in , out , len , gctx -> gcm . key , gctx -> gcm . Yi . c , gctx -> gcm . Xi . u ) ;
 gctx -> gcm . len . u [ 1 ] += bulk ;
 }
 # endif if ( CRYPTO_gcm128_encrypt_ctr32 ( & gctx -> gcm , in + bulk , out + bulk , len - bulk , gctx -> ctr ) ) goto err ;
 }
 else {
 size_t bulk = 0 ;
 # if defined ( AES_GCM_ASM2 ) if ( len >= 32 && AES_GCM_ASM2 ( gctx ) ) {
 if ( CRYPTO_gcm128_encrypt ( & gctx -> gcm , NULL , NULL , 0 ) ) return - 1 ;
 bulk = AES_gcm_encrypt ( in , out , len , gctx -> gcm . key , gctx -> gcm . Yi . c , gctx -> gcm . Xi . u ) ;
 gctx -> gcm . len . u [ 1 ] += bulk ;
 }
 # endif if ( CRYPTO_gcm128_encrypt ( & gctx -> gcm , in + bulk , out + bulk , len - bulk ) ) goto err ;
 }
 out += len ;
 CRYPTO_gcm128_tag ( & gctx -> gcm , out , EVP_GCM_TLS_TAG_LEN ) ;
 rv = len + EVP_GCM_TLS_EXPLICIT_IV_LEN + EVP_GCM_TLS_TAG_LEN ;
 }
 else {
 if ( gctx -> ctr ) {
 size_t bulk = 0 ;
 # if defined ( AES_GCM_ASM ) if ( len >= 16 && AES_GCM_ASM ( gctx ) ) {
 if ( CRYPTO_gcm128_decrypt ( & gctx -> gcm , NULL , NULL , 0 ) ) return - 1 ;
 bulk = AES_gcm_decrypt ( in , out , len , gctx -> gcm . key , gctx -> gcm . Yi . c , gctx -> gcm . Xi . u ) ;
 gctx -> gcm . len . u [ 1 ] += bulk ;
 }
 # endif if ( CRYPTO_gcm128_decrypt_ctr32 ( & gctx -> gcm , in + bulk , out + bulk , len - bulk , gctx -> ctr ) ) goto err ;
 }
 else {
 size_t bulk = 0 ;
 # if defined ( AES_GCM_ASM2 ) if ( len >= 16 && AES_GCM_ASM2 ( gctx ) ) {
 if ( CRYPTO_gcm128_decrypt ( & gctx -> gcm , NULL , NULL , 0 ) ) return - 1 ;
 bulk = AES_gcm_decrypt ( in , out , len , gctx -> gcm . key , gctx -> gcm . Yi . c , gctx -> gcm . Xi . u ) ;
 gctx -> gcm . len . u [ 1 ] += bulk ;
 }
 # endif if ( CRYPTO_gcm128_decrypt ( & gctx -> gcm , in + bulk , out + bulk , len - bulk ) ) goto err ;
 }
 CRYPTO_gcm128_tag ( & gctx -> gcm , EVP_CIPHER_CTX_buf_noconst ( ctx ) , EVP_GCM_TLS_TAG_LEN ) ;
 if ( CRYPTO_memcmp ( EVP_CIPHER_CTX_buf_noconst ( ctx ) , in + len , EVP_GCM_TLS_TAG_LEN ) ) {
 OPENSSL_cleanse ( out , len ) ;
 goto err ;
 }
 rv = len ;
 }
 err : gctx -> iv_set = 0 ;
 gctx -> tls_aad_len = - 1 ;
 return rv ;
 }
 static int aes_gcm_cipher ( EVP_CIPHER_CTX * ctx , unsigned char * out , const unsigned char * in , size_t len ) {
 EVP_AES_GCM_CTX * gctx = EVP_C_DATA ( EVP_AES_GCM_CTX , ctx ) ;
 if ( ! gctx -> key_set ) return - 1 ;
 if ( gctx -> tls_aad_len >= 0 ) return aes_gcm_tls_cipher ( ctx , out , in , len ) ;
 if ( ! gctx -> iv_set ) return - 1 ;
 if ( in ) {
 if ( out == NULL ) {
 if ( CRYPTO_gcm128_aad ( & gctx -> gcm , in , len ) ) return - 1 ;
 }
 else if ( EVP_CIPHER_CTX_encrypting ( ctx ) ) {
 if ( gctx -> ctr ) {
 size_t bulk = 0 ;
 # if defined ( AES_GCM_ASM ) if ( len >= 32 && AES_GCM_ASM ( gctx ) ) {
 size_t res = ( 16 - gctx -> gcm . mres ) % 16 ;
 if ( CRYPTO_gcm128_encrypt ( & gctx -> gcm , in , out , res ) ) return - 1 ;
 bulk = AES_gcm_encrypt ( in + res , out + res , len - res , gctx -> gcm . key , gctx -> gcm . Yi . c , gctx -> gcm . Xi . u ) ;
 gctx -> gcm . len . u [ 1 ] += bulk ;
 bulk += res ;
 }
 # endif if ( CRYPTO_gcm128_encrypt_ctr32 ( & gctx -> gcm , in + bulk , out + bulk , len - bulk , gctx -> ctr ) ) return - 1 ;
 }
 else {
 size_t bulk = 0 ;
 # if defined ( AES_GCM_ASM2 ) if ( len >= 32 && AES_GCM_ASM2 ( gctx ) ) {
 size_t res = ( 16 - gctx -> gcm . mres ) % 16 ;
 if ( CRYPTO_gcm128_encrypt ( & gctx -> gcm , in , out , res ) ) return - 1 ;
 bulk = AES_gcm_encrypt ( in + res , out + res , len - res , gctx -> gcm . key , gctx -> gcm . Yi . c , gctx -> gcm . Xi . u ) ;
 gctx -> gcm . len . u [ 1 ] += bulk ;
 bulk += res ;
 }
 # endif if ( CRYPTO_gcm128_encrypt ( & gctx -> gcm , in + bulk , out + bulk , len - bulk ) ) return - 1 ;
 }
 }
 else {
 if ( gctx -> ctr ) {
 size_t bulk = 0 ;
 # if defined ( AES_GCM_ASM ) if ( len >= 16 && AES_GCM_ASM ( gctx ) ) {
 size_t res = ( 16 - gctx -> gcm . mres ) % 16 ;
 if ( CRYPTO_gcm128_decrypt ( & gctx -> gcm , in , out , res ) ) return - 1 ;
 bulk = AES_gcm_decrypt ( in + res , out + res , len - res , gctx -> gcm . key , gctx -> gcm . Yi . c , gctx -> gcm . Xi . u ) ;
 gctx -> gcm . len . u [ 1 ] += bulk ;
 bulk += res ;
 }
 # endif if ( CRYPTO_gcm128_decrypt_ctr32 ( & gctx -> gcm , in + bulk , out + bulk , len - bulk , gctx -> ctr ) ) return - 1 ;
 }
 else {
 size_t bulk = 0 ;
 # if defined ( AES_GCM_ASM2 ) if ( len >= 16 && AES_GCM_ASM2 ( gctx ) ) {
 size_t res = ( 16 - gctx -> gcm . mres ) % 16 ;
 if ( CRYPTO_gcm128_decrypt ( & gctx -> gcm , in , out , res ) ) return - 1 ;
 bulk = AES_gcm_decrypt ( in + res , out + res , len - res , gctx -> gcm . key , gctx -> gcm . Yi . c , gctx -> gcm . Xi . u ) ;
 gctx -> gcm . len . u [ 1 ] += bulk ;
 bulk += res ;
 }
 # endif if ( CRYPTO_gcm128_decrypt ( & gctx -> gcm , in + bulk , out + bulk , len - bulk ) ) return - 1 ;
 }
 }
 return len ;
 }
 else {
 if ( ! EVP_CIPHER_CTX_encrypting ( ctx ) ) {
 if ( gctx -> taglen < 0 ) return - 1 ;
 if ( CRYPTO_gcm128_finish ( & gctx -> gcm , EVP_CIPHER_CTX_buf_noconst ( ctx ) , gctx -> taglen ) != 0 ) return - 1 ;
 gctx -> iv_set = 0 ;
 return 0 ;
 }
 CRYPTO_gcm128_tag ( & gctx -> gcm , EVP_CIPHER_CTX_buf_noconst ( ctx ) , 16 ) ;
 gctx -> taglen = 16 ;
 gctx -> iv_set = 0 ;
 return 0 ;
 }
 }
 # define CUSTOM_FLAGS ( EVP_CIPH_FLAG_DEFAULT_ASN1 \ | EVP_CIPH_CUSTOM_IV | EVP_CIPH_FLAG_CUSTOM_CIPHER \ | EVP_CIPH_ALWAYS_CALL_INIT | EVP_CIPH_CTRL_INIT \ | EVP_CIPH_CUSTOM_COPY ) BLOCK_CIPHER_custom ( NID_aes , 128 , 1 , 12 , gcm , GCM , EVP_CIPH_FLAG_AEAD_CIPHER | CUSTOM_FLAGS )