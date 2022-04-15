MIMEField * _mime_hdr_field_list_search_by_slotnum ( MIMEHdrImpl * mh , int slotnum ) {
 unsigned int block_num , block_index ;
 MIMEFieldBlockImpl * fblock ;
 if ( slotnum < MIME_FIELD_BLOCK_SLOTS ) {
 fblock = & ( mh -> m_first_fblock ) ;
 block_index = slotnum ;
 if ( block_index >= fblock -> m_freetop ) {
 return nullptr ;
 }
 else {
 return & ( fblock -> m_field_slots [ block_index ] ) ;
 }
 }
 else {
 block_num = slotnum / MIME_FIELD_BLOCK_SLOTS ;
 block_index = slotnum % MIME_FIELD_BLOCK_SLOTS ;
 fblock = & ( mh -> m_first_fblock ) ;
 while ( block_num -- && fblock ) {
 fblock = fblock -> m_next ;
 }
 if ( ( fblock == nullptr ) || ( block_index >= fblock -> m_freetop ) ) {
 return nullptr ;
 }
 else {
 return & ( fblock -> m_field_slots [ block_index ] ) ;
 }
 }
 }