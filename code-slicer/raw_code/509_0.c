int64_t mime_parse_int64 ( const char * buf , const char * end ) {
 int64_t num ;
 bool negative ;
 if ( ! buf || ( buf == end ) ) {
 return 0 ;
 }
 if ( is_digit ( * buf ) ) {
 num = * buf ++ - '0' ;
 while ( ( buf != end ) && is_digit ( * buf ) ) {
 num = ( num * 10 ) + ( * buf ++ - '0' ) ;
 }
 return num ;
 }
 else {
 num = 0 ;
 negative = false ;
 while ( ( buf != end ) && ParseRules : : is_space ( * buf ) ) {
 buf += 1 ;
 }
 if ( ( buf != end ) && ( * buf == '-' ) ) {
 negative = true ;
 buf += 1 ;
 }
 while ( ( buf != end ) && is_digit ( * buf ) ) {
 num = ( num * 10 ) - ( * buf ++ - '0' ) ;
 }
 if ( ! negative ) {
 num = - num ;
 }
 return num ;
 }
 }