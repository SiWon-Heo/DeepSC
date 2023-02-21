static void jbig2_set_bits ( byte * line , int x0 , int x1 ) {
 int a0 , a1 , b0 , b1 , a ;
 a0 = x0 >> 3 ;
 a1 = x1 >> 3 ;
 b0 = x0 & 7 ;
 b1 = x1 & 7 ;
 if ( a0 == a1 ) {
 line [ a0 ] |= lm [ b0 ] & rm [ b1 ] ;
 }
 else {
 line [ a0 ] |= lm [ b0 ] ;
 for ( a = a0 + 1 ;
 a < a1 ;
 a ++ ) line [ a ] = 0xFF ;
 if ( b1 ) line [ a1 ] |= rm [ b1 ] ;
 }
 }