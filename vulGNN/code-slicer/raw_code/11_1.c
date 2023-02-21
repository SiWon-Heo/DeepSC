static int jbig2_find_changing_element ( const byte * line , int x , int w ) {
 int a , b ;
 if ( line == 0 ) return w ;
 if ( x == - 1 ) {
 a = 0 ;
 x = 0 ;
 }
 else {
 a = getbit ( line , x ) ;
 x ++ ;
 }
 while ( x < w ) {
 b = getbit ( line , x ) ;
 if ( a != b ) break ;
 x ++ ;
 }
 return x ;
 }