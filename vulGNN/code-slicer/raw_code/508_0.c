uint32_t mime_parse_uint ( const char * buf , const char * end ) {
 uint32_t num ;
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
 while ( ( buf != end ) && ParseRules : : is_space ( * buf ) ) {
 buf += 1 ;
 }
 while ( ( buf != end ) && is_digit ( * buf ) ) {
 num = ( num * 10 ) + ( * buf ++ - '0' ) ;
 }
 return num ;
 }
 }