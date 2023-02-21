static char * * decode_args ( char * args , int * nargs ) {
 char * p , * q , * arg ;
 int i = 0 ;
 char * * parsed ;
 * nargs = 0 ;
 if ( ( p = strrchr ( args , ')' ) ) != NULL ) * p = '\0' ;
 for ( ;
 * args == ' ' ;
 args ++ ) ;
 for ( q = args + strlen ( args ) - 1 ;
 * q == ' ' ;
 q -- ) * q = '\0' ;
 if ( ! strchr ( args , ',' ) && strlen ( args ) == 0 ) return NULL ;
 SAFE_CALLOC ( parsed , 1 , sizeof ( char * ) ) ;
 for ( p = strsep_quotes ( & args , ',' ) , i = 1 ;
 p != NULL ;
 p = strsep_quotes ( & args , ',' ) , i ++ ) {
 SAFE_REALLOC ( parsed , ( i + 1 ) * sizeof ( char * ) ) ;
 for ( arg = p ;
 * arg == ' ' ;
 arg ++ ) ;
 for ( q = arg + strlen ( arg ) - 1 ;
 * q == ' ' ;
 q -- ) * q = '\0' ;
 if ( * arg == '\"' && arg [ strlen ( arg ) - 1 ] == '\"' ) {
 arg [ strlen ( arg ) - 1 ] = '\0' ;
 arg ++ ;
 }
 parsed [ i - 1 ] = strdup ( arg ) ;
 ef_debug ( 5 , "ARGUMENT: %s\n" , arg ) ;
 }
 * nargs = i - 1 ;
 return parsed ;
 }