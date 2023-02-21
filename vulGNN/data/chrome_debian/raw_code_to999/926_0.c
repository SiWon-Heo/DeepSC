size_t strlen_utf8 ( const char * s ) {
 u_char c ;
 size_t len = 0 ;
 while ( ( c = * s ++ ) ) {
 if ( ( c & 0xC0 ) != 0x80 ) ++ len ;
 }
 return len ;
 }