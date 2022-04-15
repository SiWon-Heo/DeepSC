time_t mime_field_value_get_date ( const MIMEField * field ) {
 int length ;
 const char * str = mime_field_value_get ( field , & length ) ;
 return mime_parse_date ( str , str + length ) ;
 }