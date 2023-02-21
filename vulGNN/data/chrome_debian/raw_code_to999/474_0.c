const char * mime_field_name_get ( const MIMEField * field , int * length ) {
 * length = field -> m_len_name ;
 if ( field -> m_wks_idx >= 0 ) {
 return hdrtoken_index_to_wks ( field -> m_wks_idx ) ;
 }
 else {
 return field -> m_ptr_name ;
 }
 }