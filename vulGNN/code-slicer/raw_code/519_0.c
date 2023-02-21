int mime_field_presence_get ( MIMEHdrImpl * h , const char * well_known_str ) {
 uint64_t mask = mime_field_presence_mask ( well_known_str ) ;
 return ( ( mask == 0 ) ? 1 : ( ( h -> m_presence_bits & mask ) == 0 ? 0 : 1 ) ) ;
 }