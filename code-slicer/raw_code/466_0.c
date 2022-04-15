int mime_field_value_get_comma_val_count ( const MIMEField * field ) {
 if ( ! field -> supports_commas ( ) ) {
 return ( ( field -> m_len_value == 0 ) ? 0 : 1 ) ;
 }
 else {
 StrList list ( false ) ;
 int count = mime_field_value_get_comma_list ( field , & list ) ;
 return count ;
 }
 }