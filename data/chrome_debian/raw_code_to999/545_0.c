const char * mime_field_value_get ( const MIMEField * field , int * length ) {
 * length = field -> m_len_value ;
 return field -> m_ptr_value ;
 }