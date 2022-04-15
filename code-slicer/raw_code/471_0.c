MIMEField * mime_hdr_field_get ( MIMEHdrImpl * mh , int idx ) {
 unsigned int index ;
 MIMEFieldBlockImpl * fblock ;
 MIMEField * field ;
 int got_idx ;
 got_idx = - 1 ;
 for ( fblock = & ( mh -> m_first_fblock ) ;
 fblock != nullptr ;
 fblock = fblock -> m_next ) {
 for ( index = 0 ;
 index < fblock -> m_freetop ;
 index ++ ) {
 field = & ( fblock -> m_field_slots [ index ] ) ;
 if ( field -> is_live ( ) ) {
 ++ got_idx ;
 }
 if ( got_idx == idx ) {
 return field ;
 }
 }
 }
 return nullptr ;
 }