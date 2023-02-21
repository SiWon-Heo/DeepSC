const char * mime_field_value_get_comma_val ( const MIMEField * field , int * length , int idx ) {
 if ( ! field -> supports_commas ( ) ) {
 if ( idx == 0 ) {
 return mime_field_value_get ( field , length ) ;
 }
 else {
 return nullptr ;
 }
 }
 else {
 Str * str ;
 StrList list ( false ) ;
 mime_field_value_get_comma_list ( field , & list ) ;
 str = list . get_idx ( idx ) ;
 if ( str != nullptr ) {
 * length = ( int ) ( str -> len ) ;
 return str -> str ;
 }
 else {
 * length = 0 ;
 return nullptr ;
 }
 }
 }