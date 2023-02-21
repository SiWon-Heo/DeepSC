static int unescape_octals ( gx_device_pdf * pdev , char * src , int size ) {
 char * start , * dest ;
 start = src ;
 dest = src ;
 while ( size ) {
 if ( size > 4 && src [ 0 ] == '\\' && src [ 1 ] == '\\' && src [ 2 ] > 0x29 && src [ 2 ] < 0x35 && src [ 3 ] > 0x29 && src [ 3 ] < 0x38 && src [ 4 ] > 0x29 && src [ 4 ] < 0x38 ) {
 src ++ ;
 size -- ;
 }
 else {
 * dest ++ = * src ++ ;
 size -- ;
 }
 }
 return ( dest - start ) ;
 }