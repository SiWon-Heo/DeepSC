static char * strsep_quotes ( char * * stringp , const char delim ) {
 char * s ;
 int c ;
 char * tok ;
 if ( ( s = * stringp ) == NULL ) return ( NULL ) ;
 for ( tok = s ;
 ;
 ) {
 if ( * s == '\"' ) while ( * ( ++ s ) != '\"' && * s != '\0' ) ;
 c = * s ++ ;
 if ( c == delim || c == 0 ) {
 if ( c == 0 ) s = NULL ;
 else s [ - 1 ] = 0 ;
 * stringp = s ;
 return ( tok ) ;
 }
 }
 }