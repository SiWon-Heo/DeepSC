static void make_key_64 ( u_char * string , u_char * key ) {
 int i , seed = 0 ;
 for ( i = 0 ;
 string [ i ] ;
 i ++ ) {
 seed ^= ( string [ i ] << ( ( i & 0x03 ) * 8 ) ) ;
 }
 for ( i = 0 ;
 i < 5 * 4 ;
 i ++ ) {
 seed *= 0x000343fd ;
 seed += 0x00269ec3 ;
 key [ i ] = seed >> 16 ;
 }
 }