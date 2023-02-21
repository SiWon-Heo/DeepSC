static void http_skip_ws ( const char * & buf , int & len ) {
 while ( len > 0 && * buf && ParseRules : : is_ws ( * buf ) ) {
 buf += 1 ;
 len -= 1 ;
 }
 }