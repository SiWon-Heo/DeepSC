static void init_mv ( FourXContext * f ) {
 int i ;
 for ( i = 0 ;
 i < 256 ;
 i ++ ) {
 if ( f -> version > 1 ) f -> mv [ i ] = mv [ i ] [ 0 ] + mv [ i ] [ 1 ] * f -> current_picture -> linesize [ 0 ] / 2 ;
 else f -> mv [ i ] = ( i & 15 ) - 8 + ( ( i >> 4 ) - 8 ) * f -> current_picture -> linesize [ 0 ] / 2 ;
 }
 }