int str_replace ( char * * text , const char * s , const char * d ) {
 size_t slen = strlen ( s ) ;
 size_t dlen = strlen ( d ) ;
 int diff = dlen - slen ;
 char * p , * q = * text ;
 size_t size ;
 if ( strstr ( * text , s ) == NULL ) return - E_NOTFOUND ;
 while ( ( p = strstr ( q , s ) ) != NULL ) {
 if ( diff > 0 ) size = strlen ( q ) + diff + 1 ;
 else size = strlen ( q ) + 1 ;
 SAFE_REALLOC ( * text , size ) ;
 q = * text ;
 p = strstr ( q , s ) ;
 if ( p == NULL ) continue ;
 memmove ( p + dlen , p + slen , strlen ( p + slen ) + 1 ) ;
 memcpy ( p , d , dlen ) ;
 q = p + dlen ;
 }
 return E_SUCCESS ;
 }