inline void mime_hdr_unset_accelerators_and_presence_bits ( MIMEHdrImpl * mh , MIMEField * field ) {
 int slot_id ;
 if ( field -> m_wks_idx < 0 ) {
 return ;
 }
 mime_hdr_presence_unset ( mh , field -> m_wks_idx ) ;
 slot_id = hdrtoken_index_to_slotid ( field -> m_wks_idx ) ;
 if ( slot_id != MIME_SLOTID_NONE ) {
 mime_hdr_set_accelerator_slotnum ( mh , slot_id , MIME_FIELD_SLOTNUM_MAX ) ;
 }
 }