static struct cvec * range ( struct vars * v , celt a , celt b , int cases ) {
 int nchrs ;
 struct cvec * cv ;
 celt c , lc , uc ;
 if ( a != b && ! before ( a , b ) ) {
 ERR ( REG_ERANGE ) ;
 return NULL ;
 }
 if ( ! cases ) {
 cv = getcvec ( v , 0 , 1 ) ;
 NOERRN ( ) ;
 addrange ( cv , a , b ) ;
 return cv ;
 }
 nchrs = ( b - a + 1 ) * 2 + 4 ;
 cv = getcvec ( v , nchrs , 0 ) ;
 NOERRN ( ) ;
 for ( c = a ;
 c <= b ;
 c ++ ) {
 addchr ( cv , c ) ;
 lc = pg_wc_tolower ( ( chr ) c ) ;
 if ( c != lc ) addchr ( cv , lc ) ;
 uc = pg_wc_toupper ( ( chr ) c ) ;
 if ( c != uc ) addchr ( cv , uc ) ;
 }
 return cv ;
 }