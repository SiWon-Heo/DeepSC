void mime_hdr_presence_unset ( MIMEHdrImpl * h , const char * well_known_str ) {
 uint64_t mask = mime_field_presence_mask ( well_known_str ) ;
 if ( mask != 0 ) {
 h -> m_presence_bits &= ( ~ mask ) ;
 }
 }