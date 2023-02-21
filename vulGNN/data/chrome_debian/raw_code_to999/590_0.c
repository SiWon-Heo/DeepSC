int32_t http_parse_version ( const char * start , const char * end ) {
 int maj ;
 int min ;
 if ( ( end - start ) < 8 ) {
 return HTTP_VERSION ( 0 , 9 ) ;
 }
 if ( ( ( start [ 0 ] == 'H' ) || ( start [ 0 ] == 'h' ) ) && ( ( start [ 1 ] == 'T' ) || ( start [ 1 ] == 't' ) ) && ( ( start [ 2 ] == 'T' ) || ( start [ 2 ] == 't' ) ) && ( ( start [ 3 ] == 'P' ) || ( start [ 3 ] == 'p' ) ) && ( start [ 4 ] == '/' ) ) {
 start += 5 ;
 maj = 0 ;
 min = 0 ;
 while ( ( start != end ) && ParseRules : : is_digit ( * start ) ) {
 maj = ( maj * 10 ) + ( * start - '0' ) ;
 start += 1 ;
 }
 if ( * start == '.' ) {
 start += 1 ;
 }
 while ( ( start != end ) && ParseRules : : is_digit ( * start ) ) {
 min = ( min * 10 ) + ( * start - '0' ) ;
 start += 1 ;
 }
 return HTTP_VERSION ( maj , min ) ;
 }
 return HTTP_VERSION ( 0 , 9 ) ;
 }