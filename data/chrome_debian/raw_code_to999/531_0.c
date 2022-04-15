inline void mime_hdr_set_accelerators_and_presence_bits ( MIMEHdrImpl * mh , MIMEField * field ) {
 int slot_id ;
 ptrdiff_t slot_num ;
 if ( field -> m_wks_idx < 0 ) {
 return ;
 }
 mime_hdr_presence_set ( mh , field -> m_wks_idx ) ;
 slot_id = hdrtoken_index_to_slotid ( field -> m_wks_idx ) ;
 if ( slot_id != MIME_SLOTID_NONE ) {
 if ( mh -> m_first_fblock . contains ( field ) ) {
 slot_num = ( field - & ( mh -> m_first_fblock . m_field_slots [ 0 ] ) ) ;
 if ( slot_num >= MIME_FIELD_SLOTNUM_UNKNOWN ) {
 slot_num = MIME_FIELD_SLOTNUM_UNKNOWN ;
 }
 }
 else {
 slot_num = MIME_FIELD_SLOTNUM_UNKNOWN ;
 }
 mime_hdr_set_accelerator_slotnum ( mh , slot_id , slot_num ) ;
 }
 }