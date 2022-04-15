int mime_field_value_get_comma_list ( const MIMEField * field , StrList * list ) {
 const char * str ;
 int len ;
 str = mime_field_value_get ( field , & len ) ;
 if ( ! field -> supports_commas ( ) ) {
 list -> append_string ( str , len ) ;
 }
 else {
 HttpCompat : : parse_tok_list ( list , 1 , str , len , ',' ) ;
 }
 return list -> count ;
 }