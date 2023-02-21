int mime_hdr_fields_count ( MIMEHdrImpl * mh ) {
 unsigned int index ;
 MIMEFieldBlockImpl * fblock ;
 MIMEField * field ;
 int count ;
 count = 0 ;
 for ( fblock = & ( mh -> m_first_fblock ) ;
 fblock != nullptr ;
 fblock = fblock -> m_next ) {
 for ( index = 0 ;
 index < fblock -> m_freetop ;
 index ++ ) {
 field = & ( fblock -> m_field_slots [ index ] ) ;
 if ( field -> is_live ( ) ) {
 ++ count ;
 }
 }
 }
 return count ;
 }