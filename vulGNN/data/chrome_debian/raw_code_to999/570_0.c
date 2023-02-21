static inline char * http_hdr_version_to_string ( int32_t version , char * buf9 ) {
 ink_assert ( HTTP_MAJOR ( version ) < 10 ) ;
 ink_assert ( HTTP_MINOR ( version ) < 10 ) ;
 buf9 [ 0 ] = 'H' ;
 buf9 [ 1 ] = 'T' ;
 buf9 [ 2 ] = 'T' ;
 buf9 [ 3 ] = 'P' ;
 buf9 [ 4 ] = '/' ;
 buf9 [ 5 ] = '0' + HTTP_MAJOR ( version ) ;
 buf9 [ 6 ] = '.' ;
 buf9 [ 7 ] = '0' + HTTP_MINOR ( version ) ;
 buf9 [ 8 ] = '\0' ;
 return ( buf9 ) ;
 }