int mime_parse_year ( const char * & buf , const char * end , int * year ) {
 int val ;
 while ( ( buf != end ) && * buf && ! is_digit ( * buf ) ) {
 buf += 1 ;
 }
 if ( ( buf == end ) || ( * buf == '\0' ) ) {
 return 0 ;
 }
 val = 0 ;
 while ( ( buf != end ) && * buf && is_digit ( * buf ) ) {
 val = ( val * 10 ) + ( * buf ++ - '0' ) ;
 }
 if ( val >= 1900 ) {
 val -= 1900 ;
 }
 else if ( val < 70 ) {
 val += 100 ;
 }
 * year = val ;
 return 1 ;
 }