MIMEField * _mime_hdr_field_list_search_by_wks ( MIMEHdrImpl * mh , int wks_idx ) {
 MIMEFieldBlockImpl * fblock ;
 MIMEField * field , * too_far_field ;
 ink_assert ( hdrtoken_is_valid_wks_idx ( wks_idx ) ) ;
 for ( fblock = & ( mh -> m_first_fblock ) ;
 fblock != nullptr ;
 fblock = fblock -> m_next ) {
 field = & ( fblock -> m_field_slots [ 0 ] ) ;
 too_far_field = & ( fblock -> m_field_slots [ fblock -> m_freetop ] ) ;
 while ( field < too_far_field ) {
 if ( field -> is_live ( ) && ( field -> m_wks_idx == wks_idx ) ) {
 return field ;
 }
 ++ field ;
 }
 }
 return nullptr ;
 }