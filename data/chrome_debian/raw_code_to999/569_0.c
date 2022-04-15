static double http_parse_qvalue ( const char * & buf , int & len ) {
 double val = 1.0 ;
 if ( * buf != ';
' ) {
 return val ;
 }
 buf += 1 ;
 len -= 1 ;
 while ( len > 0 && * buf ) {
 http_skip_ws ( buf , len ) ;
 if ( * buf == 'q' ) {
 buf += 1 ;
 len -= 1 ;
 http_skip_ws ( buf , len ) ;
 if ( * buf == '=' ) {
 double n ;
 int f ;
 buf += 1 ;
 len -= 1 ;
 http_skip_ws ( buf , len ) ;
 n = 0.0 ;
 while ( len > 0 && * buf && ParseRules : : is_digit ( * buf ) ) {
 n = ( n * 10 ) + ( * buf ++ - '0' ) ;
 len -= 1 ;
 }
 if ( * buf == '.' ) {
 buf += 1 ;
 len -= 1 ;
 f = 10 ;
 while ( len > 0 && * buf && ParseRules : : is_digit ( * buf ) ) {
 n += ( * buf ++ - '0' ) / ( double ) f ;
 f *= 10 ;
 len -= 1 ;
 }
 }
 val = n ;
 }
 }
 else {
 while ( len > 0 && * buf ) {
 if ( * buf != ';
' ) {
 buf += 1 ;
 len -= 1 ;
 }
 else {
 buf += 1 ;
 len -= 1 ;
 break ;
 }
 }
 }
 }
 return val ;
 }