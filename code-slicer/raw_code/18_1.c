unsigned long # define BN_BYTES 8 # endif # ifdef SIXTY_FOUR_BIT # define BN_ULONG unsigned long long # define BN_BYTES 8 # endif # ifdef THIRTY_TWO_BIT # define BN_ULONG unsigned int # define BN_BYTES 4 # endif # define BN_BITS2 ( BN_BYTES * 8 ) # define BN_BITS ( BN_BITS2 * 2 ) # define BN_TBIT ( ( BN_ULONG ) 1 << ( BN_BITS2 - 1 ) ) # define BN_FLG_MALLOCED 0x01 # define BN_FLG_STATIC_DATA 0x02 # define BN_FLG_CONSTTIME 0x04 # define BN_FLG_SECURE 0x08 # if OPENSSL_API_COMPAT < 0x00908000L # define BN_FLG_EXP_CONSTTIME BN_FLG_CONSTTIME # define BN_FLG_FREE 0x8000 # endif void BN_set_flags ( BIGNUM * b , int n ) ;
 int BN_get_flags ( const BIGNUM * b , int n ) ;
 # define BN_RAND_TOP_ANY - 1 # define BN_RAND_TOP_ONE 0 # define BN_RAND_TOP_TWO 1 # define BN_RAND_BOTTOM_ANY 0 # define BN_RAND_BOTTOM_ODD 1 void BN_with_flags ( BIGNUM * dest , const BIGNUM * b , int flags ) ;
 int BN_GENCB_call ( BN_GENCB * cb , int a , int b ) ;
 BN_GENCB * BN_GENCB_new ( void ) ;
 void BN_GENCB_free ( BN_GENCB * cb ) ;
 void BN_GENCB_set_old ( BN_GENCB * gencb , void ( * callback ) ( int , int , void * ) , void * cb_arg ) ;
 void BN_GENCB_set ( BN_GENCB * gencb , int ( * callback ) ( int , int , BN_GENCB * ) , void * cb_arg ) ;
 void * BN_GENCB_get_arg ( BN_GENCB * cb ) ;
 # define BN_prime_checks 0 # define BN_prime_checks_for_size ( b ) ( ( b ) >= 3747 ? 3 : ( b ) >= 1345 ? 4 : ( b ) >= 476 ? 5 : ( b ) >= 400 ? 6 : ( b ) >= 347 ? 7 : ( b ) >= 308 ? 8 : ( b ) >= 55 ? 27 : 34 ) # define BN_num_bytes ( a ) ( ( BN_num_bits ( a ) + 7 ) / 8 ) int BN_abs_is_word ( const BIGNUM * a , const BN_ULONG w ) ;
 int BN_is_zero ( const BIGNUM * a ) ;
 int BN_is_one ( const BIGNUM * a ) ;
 int BN_is_word ( const BIGNUM * a , const BN_ULONG w ) ;
 int BN_is_odd ( const BIGNUM * a ) ;
 # define BN_one ( a ) ( BN_set_word ( ( a ) , 1 ) ) void BN_zero_ex ( BIGNUM * a ) ;
 # if OPENSSL_API_COMPAT >= 0x00908000L # define BN_zero ( a ) BN_zero_ex ( a ) # else # define BN_zero ( a ) ( BN_set_word ( ( a ) , 0 ) ) # endif const BIGNUM * BN_value_one ( void ) ;
 char * BN_options ( void ) ;
 BN_CTX * BN_CTX_new ( void ) ;
 BN_CTX * BN_CTX_secure_new ( void ) ;
 void BN_CTX_free ( BN_CTX * c ) ;
 void BN_CTX_start ( BN_CTX * ctx ) ;
 BIGNUM * BN_CTX_get ( BN_CTX * ctx ) ;
 void BN_CTX_end ( BN_CTX * ctx ) ;
 int BN_rand ( BIGNUM * rnd , int bits , int top , int bottom ) ;
 int BN_priv_rand ( BIGNUM * rnd , int bits , int top , int bottom ) ;
 int BN_rand_range ( BIGNUM * rnd , const BIGNUM * range ) ;
 int BN_priv_rand_range ( BIGNUM * rnd , const BIGNUM * range ) ;
 int BN_pseudo_rand ( BIGNUM * rnd , int bits , int top , int bottom ) ;
 int BN_pseudo_rand_range ( BIGNUM * rnd , const BIGNUM * range ) ;
 int BN_num_bits ( const BIGNUM * a ) ;
 int BN_num_bits_word ( BN_ULONG l ) ;
 int BN_security_bits ( int L , int N ) ;
 BIGNUM * BN_new ( void ) ;
 BIGNUM * BN_secure_new ( void ) ;
 void BN_clear_free ( BIGNUM * a ) ;
 BIGNUM * BN_copy ( BIGNUM * a , const BIGNUM * b ) ;
 void BN_swap ( BIGNUM * a , BIGNUM * b ) ;
 BIGNUM * BN_bin2bn ( const unsigned char * s , int len , BIGNUM * ret ) ;
 int BN_bn2bin ( const BIGNUM * a , unsigned char * to ) ;
 int BN_bn2binpad ( const BIGNUM * a , unsigned char * to , int tolen ) ;
 BIGNUM * BN_lebin2bn ( const unsigned char * s , int len , BIGNUM * ret ) ;
 int BN_bn2lebinpad ( const BIGNUM * a , unsigned char * to , int tolen ) ;
 BIGNUM * BN_mpi2bn ( const unsigned char * s , int len , BIGNUM * ret ) ;
 int BN_bn2mpi ( const BIGNUM * a , unsigned char * to ) ;
 int BN_sub ( BIGNUM * r , const BIGNUM * a , const BIGNUM * b ) ;
 int BN_usub ( BIGNUM * r , const BIGNUM * a , const BIGNUM * b ) ;
 int BN_uadd ( BIGNUM * r , const BIGNUM * a , const BIGNUM * b ) ;
 int BN_add ( BIGNUM * r , const BIGNUM * a , const BIGNUM * b ) ;
 int BN_mul ( BIGNUM * r , const BIGNUM * a , const BIGNUM * b , BN_CTX * ctx ) ;
 int BN_sqr ( BIGNUM * r , const BIGNUM * a , BN_CTX * ctx ) ;
 void BN_set_negative ( BIGNUM * b , int n ) ;
 int BN_is_negative ( const BIGNUM * b ) ;
 int BN_div ( BIGNUM * dv , BIGNUM * rem , const BIGNUM * m , const BIGNUM * d , BN_CTX * ctx ) ;
 # define BN_mod ( rem , m , d , ctx ) BN_div ( NULL , ( rem ) , ( m ) , ( d ) , ( ctx ) ) int BN_nnmod ( BIGNUM * r , const BIGNUM * m , const BIGNUM * d , BN_CTX * ctx ) ;
 int BN_mod_add ( BIGNUM * r , const BIGNUM * a , const BIGNUM * b , const BIGNUM * m , BN_CTX * ctx ) ;
 int BN_mod_add_quick ( BIGNUM * r , const BIGNUM * a , const BIGNUM * b , const BIGNUM * m ) ;
 int BN_mod_sub ( BIGNUM * r , const BIGNUM * a , const BIGNUM * b , const BIGNUM * m , BN_CTX * ctx ) ;
 int BN_mod_sub_quick ( BIGNUM * r , const BIGNUM * a , const BIGNUM * b , const BIGNUM * m ) ;
 int BN_mod_mul ( BIGNUM * r , const BIGNUM * a , const BIGNUM * b , const BIGNUM * m , BN_CTX * ctx ) ;
 int BN_mod_sqr ( BIGNUM * r , const BIGNUM * a , const BIGNUM * m , BN_CTX * ctx ) ;
 int BN_mod_lshift1 ( BIGNUM * r , const BIGNUM * a , const BIGNUM * m , BN_CTX * ctx ) ;
 int BN_mod_lshift1_quick ( BIGNUM * r , const BIGNUM * a , const BIGNUM * m ) ;
 int BN_mod_lshift ( BIGNUM * r , const BIGNUM * a , int n , const BIGNUM * m , BN_CTX * ctx ) ;
 int BN_mod_lshift_quick ( BIGNUM * r , const BIGNUM * a , int n , const BIGNUM * m ) ;
 BN_ULONG BN_mod_word ( const BIGNUM * a , BN_ULONG w ) ;
 BN_ULONG BN_div_word ( BIGNUM * a , BN_ULONG w ) ;
 int BN_mul_word ( BIGNUM * a , BN_ULONG w ) ;
 int BN_add_word ( BIGNUM * a , BN_ULONG w ) ;
 int BN_sub_word ( BIGNUM * a , BN_ULONG w ) ;
 int BN_set_word ( BIGNUM * a , BN_ULONG w ) ;
 BN_ULONG BN_get_word ( const BIGNUM * a )