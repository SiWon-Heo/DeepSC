static void make_key_128 ( u_char * string , u_char * key ) {
 MD5_CTX ctx ;
 u_char buf [ 64 ] ;
 u_char digest [ MD5_DIGEST_LENGTH ] ;
 int i , j = 0 ;
 for ( i = 0 ;
 i < 64 ;
 i ++ ) {
 if ( string [ j ] == 0 ) j = 0 ;
 buf [ i ] = string [ j ++ ] ;
 }
 MD5_Init ( & ctx ) ;
 MD5_Update ( & ctx , buf , sizeof buf ) ;
 MD5_Final ( digest , & ctx ) ;
 memset ( key , 0 , MAX_WKEY_LEN ) ;
 memcpy ( key , digest , 13 ) ;
 }