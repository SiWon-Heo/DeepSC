static int version_cmp ( const SSL * s , int a , int b ) {
 int dtls = SSL_IS_DTLS ( s ) ;
 if ( a == b ) return 0 ;
 if ( ! dtls ) return a < b ? - 1 : 1 ;
 return DTLS_VERSION_LT ( a , b ) ? - 1 : 1 ;
 }