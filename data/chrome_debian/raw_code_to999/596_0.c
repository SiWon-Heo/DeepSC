HTTPStatus http_parse_status ( const char * start , const char * end ) {
 int status = 0 ;
 while ( ( start != end ) && ParseRules : : is_space ( * start ) ) {
 start += 1 ;
 }
 while ( ( start != end ) && ParseRules : : is_digit ( * start ) ) {
 status = ( status * 10 ) + ( * start ++ - '0' ) ;
 }
 return ( HTTPStatus ) status ;
 }