void appendConnStrVal ( PQExpBuffer buf , const char * str ) {
 const char * s ;
 bool needquotes ;
 needquotes = false ;
 for ( s = str ;
 * s ;
 s ++ ) {
 if ( ! ( ( * s >= 'a' && * s <= 'z' ) || ( * s >= 'A' && * s <= 'Z' ) || ( * s >= '0' && * s <= '9' ) || * s == '_' || * s == '.' ) ) {
 needquotes = true ;
 break ;
 }
 }
 if ( needquotes ) {
 appendPQExpBufferChar ( buf , '\'' ) ;
 while ( * str ) {
 if ( * str == '\'' || * str == '\\' ) appendPQExpBufferChar ( buf , '\\' ) ;
 appendPQExpBufferChar ( buf , * str ) ;
 str ++ ;
 }
 appendPQExpBufferChar ( buf , '\'' ) ;
 }
 else appendPQExpBufferStr ( buf , str ) ;
 }