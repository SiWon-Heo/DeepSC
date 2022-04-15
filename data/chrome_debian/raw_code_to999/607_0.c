static int phar_tar_octal ( char * buf , php_uint32 val , int len ) {
 char * p = buf ;
 int s = len ;
 p += len ;
 while ( s -- > 0 ) {
 * -- p = ( char ) ( '0' + ( val & 7 ) ) ;
 val >>= 3 ;
 }
 if ( val == 0 ) return SUCCESS ;
 while ( len -- > 0 ) * p ++ = '7' ;
 return FAILURE ;
 }