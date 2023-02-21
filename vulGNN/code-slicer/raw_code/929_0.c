char getchar_buffer ( char * * buf ) {
 char ret ;
 DEBUG_MSG ( "getchar_buffer: %s" , * buf ) ;
 if ( * * buf == 0 ) return 0 ;
 if ( * ( * buf + 0 ) == 's' && * ( * buf + 1 ) == '(' ) {
 char * p ;
 int time = 0 ;
 p = strchr ( * buf , ')' ) ;
 if ( p != NULL ) {
 * p = '\0' ;
 time = atoi ( * buf + 2 ) ;
 DEBUG_MSG ( "getchar_buffer: sleeping %d secs" , time ) ;
 * buf = p + 1 ;
 ec_usleep ( SEC2MICRO ( time ) ) ;
 }
 }
 ret = * buf [ 0 ] ;
 * buf = * buf + 1 ;
 DEBUG_MSG ( "getchar_buffer: returning %c" , ret ) ;
 return ret ;
 }