MIMEField * _mime_hdr_field_list_search_by_string ( MIMEHdrImpl * mh , const char * field_name_str , int field_name_len ) {
 MIMEFieldBlockImpl * fblock ;
 MIMEField * field , * too_far_field ;
 ink_assert ( mh ) ;
 for ( fblock = & ( mh -> m_first_fblock ) ;
 fblock != nullptr ;
 fblock = fblock -> m_next ) {
 field = & ( fblock -> m_field_slots [ 0 ] ) ;
 too_far_field = & ( fblock -> m_field_slots [ fblock -> m_freetop ] ) ;
 while ( field < too_far_field ) {
 if ( field -> is_live ( ) && ( field_name_len == field -> m_len_name ) && ( strncasecmp ( field -> m_ptr_name , field_name_str , field_name_len ) == 0 ) ) {
 return field ;
 }
 ++ field ;
 }
 }
 return nullptr ;
 }