static char * http_str_store ( Arena * arena , const char * str , int length ) {
 const char * wks ;
 int idx = hdrtoken_tokenize ( str , length , & wks ) ;
 if ( idx < 0 ) {
 return arena -> str_store ( str , length ) ;
 }
 else {
 return ( char * ) wks ;
 }
 }