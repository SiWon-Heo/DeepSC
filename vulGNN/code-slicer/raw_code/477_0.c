int mime_hdr_field_slotnum ( MIMEHdrImpl * mh , MIMEField * field ) {
 int slots_so_far ;
 MIMEFieldBlockImpl * fblock ;
 slots_so_far = 0 ;
 for ( fblock = & ( mh -> m_first_fblock ) ;
 fblock != nullptr ;
 fblock = fblock -> m_next ) {
 if ( fblock -> contains ( field ) ) {
 MIMEField * first = & ( fblock -> m_field_slots [ 0 ] ) ;
 ptrdiff_t block_slot = field - first ;
 return slots_so_far + block_slot ;
 }
 slots_so_far += MIME_FIELD_BLOCK_SLOTS ;
 }
 return - 1 ;
 }