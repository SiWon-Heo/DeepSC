int mime_hdr_length_get ( MIMEHdrImpl * mh ) {
 unsigned int length , index ;
 MIMEFieldBlockImpl * fblock ;
 MIMEField * field ;
 length = 2 ;
 for ( fblock = & ( mh -> m_first_fblock ) ;
 fblock != nullptr ;
 fblock = fblock -> m_next ) {
 for ( index = 0 ;
 index < fblock -> m_freetop ;
 index ++ ) {
 field = & ( fblock -> m_field_slots [ index ] ) ;
 if ( field -> is_live ( ) ) {
 length += mime_field_length_get ( field ) ;
 }
 }
 }
 return length ;
 }