static int ec_GF2m_montgomery_point_multiply ( const EC_GROUP * group , EC_POINT * r , const BIGNUM * scalar , const EC_POINT * point , BN_CTX * ctx ) {
 BIGNUM * x1 , * x2 , * z1 , * z2 ;
 int ret = 0 , i ;
 BN_ULONG mask , word ;
 if ( r == point ) {
 ECerr ( EC_F_EC_GF2M_MONTGOMERY_POINT_MULTIPLY , EC_R_INVALID_ARGUMENT ) ;
 return 0 ;
 }
 if ( ( scalar == NULL ) || BN_is_zero ( scalar ) || ( point == NULL ) || EC_POINT_is_at_infinity ( group , point ) ) {
 return EC_POINT_set_to_infinity ( group , r ) ;
 }
 if ( ! point -> Z_is_one ) return 0 ;
 BN_CTX_start ( ctx ) ;
 x1 = BN_CTX_get ( ctx ) ;
 z1 = BN_CTX_get ( ctx ) ;
 if ( z1 == NULL ) goto err ;
 x2 = & r -> X ;
 z2 = & r -> Y ;
 if ( ! BN_GF2m_mod_arr ( x1 , & point -> X , group -> poly ) ) goto err ;
 if ( ! BN_one ( z1 ) ) goto err ;
 if ( ! group -> meth -> field_sqr ( group , z2 , x1 , ctx ) ) goto err ;
 if ( ! group -> meth -> field_sqr ( group , x2 , z2 , ctx ) ) goto err ;
 if ( ! BN_GF2m_add ( x2 , x2 , & group -> b ) ) goto err ;
 i = scalar -> top - 1 ;
 mask = BN_TBIT ;
 word = scalar -> d [ i ] ;
 while ( ! ( word & mask ) ) mask >>= 1 ;
 mask >>= 1 ;
 if ( ! mask ) {
 i -- ;
 mask = BN_TBIT ;
 }
 for ( ;
 i >= 0 ;
 i -- ) {
 word = scalar -> d [ i ] ;
 while ( mask ) {
 if ( word & mask ) {
 if ( ! gf2m_Madd ( group , & point -> X , x1 , z1 , x2 , z2 , ctx ) ) goto err ;
 if ( ! gf2m_Mdouble ( group , x2 , z2 , ctx ) ) goto err ;
 }
 else {
 if ( ! gf2m_Madd ( group , & point -> X , x2 , z2 , x1 , z1 , ctx ) ) goto err ;
 if ( ! gf2m_Mdouble ( group , x1 , z1 , ctx ) ) goto err ;
 }
 mask >>= 1 ;
 }
 mask = BN_TBIT ;
 }
 i = gf2m_Mxy ( group , & point -> X , & point -> Y , x1 , z1 , x2 , z2 , ctx ) ;
 if ( i == 0 ) goto err ;
 else if ( i == 1 ) {
 if ( ! EC_POINT_set_to_infinity ( group , r ) ) goto err ;
 }
 else {
 if ( ! BN_one ( & r -> Z ) ) goto err ;
 r -> Z_is_one = 1 ;
 }
 BN_set_negative ( & r -> X , 0 ) ;
 BN_set_negative ( & r -> Y , 0 ) ;
 ret = 1 ;
 err : BN_CTX_end ( ctx ) ;
 return ret ;
 }