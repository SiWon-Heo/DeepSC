int64_t mime_field_value_get_int64 ( const MIMEField * field ) {
 int length ;
 const char * str = mime_field_value_get ( field , & length ) ;
 return mime_parse_int64 ( str , str + length ) ;
 }