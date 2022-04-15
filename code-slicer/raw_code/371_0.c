int ssl_allow_compression ( SSL * s ) {
 if ( s -> options & SSL_OP_NO_COMPRESSION ) return 0 ;
 return ssl_security ( s , SSL_SECOP_COMPRESSION , 0 , 0 , NULL ) ;
 }