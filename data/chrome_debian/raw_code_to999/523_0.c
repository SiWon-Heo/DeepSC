int32_t mime_field_value_get_int ( const MIMEField * field ) {
 int length ;
 const char * str = mime_field_value_get ( field , & length ) ;
 return mime_parse_int ( str , str + length ) ;
 }