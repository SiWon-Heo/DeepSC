int match_pattern ( const char * s , const char * pattern ) {
 for ( ;
 ;
 ) {
 if ( ! * pattern ) return ( ! * s ) ;
 if ( * pattern == '*' ) {
 pattern ++ ;
 if ( ! * pattern ) return ( 1 ) ;
 if ( * pattern != '?' && * pattern != '*' ) {
 for ( ;
 * s ;
 s ++ ) {
 if ( * s == * pattern && match_pattern ( s + 1 , pattern + 1 ) ) return ( 1 ) ;
 }
 return ( 0 ) ;
 }
 for ( ;
 * s ;
 s ++ ) {
 if ( match_pattern ( s , pattern ) ) return ( 1 ) ;
 }
 return ( 0 ) ;
 }
 if ( ! * s ) return ( 0 ) ;
 if ( * pattern != '?' && * pattern != * s ) return ( 0 ) ;
 s ++ ;
 pattern ++ ;
 }
 }