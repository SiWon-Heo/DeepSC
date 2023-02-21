int mime_parse_day ( const char * & buf , const char * end , int * day ) {
 const char * e ;
 while ( ( buf != end ) && * buf && ! ParseRules : : is_alpha ( * buf ) ) {
 buf += 1 ;
 }
 e = buf ;
 while ( ( e != end ) && * e && ParseRules : : is_alpha ( * e ) ) {
 e += 1 ;
 }
 * day = day_names_dfa -> match ( buf , e - buf ) ;
 if ( * day < 0 ) {
 return 0 ;
 }
 else {
 buf = e ;
 return 1 ;
 }
 }