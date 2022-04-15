static char * xmlrpc_parse ( char * buffer ) {
 char * tmp = NULL ;
 tmp = strstr ( buffer , "<?xml" ) ;
 if ( tmp ) {
 return xmlrpc_normalizeBuffer ( tmp ) ;
 }
 return NULL ;
 }