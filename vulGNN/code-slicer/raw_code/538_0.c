int mime_parse_integer ( const char * & buf , const char * end , int * integer ) {
 int val ;
 bool negative ;
 negative = false ;
 while ( ( buf != end ) && * buf && ! is_digit ( * buf ) && ( * buf != '-' ) ) {
 buf += 1 ;
 }
 if ( ( buf == end ) || ( * buf == '\0' ) ) {
 return 0 ;
 }
 if ( * buf == '-' ) {
 negative = true ;
 buf += 1 ;
 }
 val = 0 ;
 while ( ( buf != end ) && is_digit ( * buf ) ) {
 val = ( val * 10 ) + ( * buf ++ - '0' ) ;
 }
 if ( negative ) {
 * integer = - val ;
 }
 else {
 * integer = val ;
 }
 return 1 ;
 }