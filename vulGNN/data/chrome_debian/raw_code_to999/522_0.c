inline uint32_t mime_hdr_get_accelerator_slotnum ( MIMEHdrImpl * mh , int32_t slot_id ) {
 ink_assert ( ( slot_id != MIME_SLOTID_NONE ) && ( slot_id < 32 ) ) ;
 uint32_t word_index = slot_id / 8 ;
 uint32_t word = mh -> m_slot_accelerators [ word_index ] ;
 uint32_t nybble = slot_id % 8 ;
 uint32_t slot = ( ( word >> ( nybble * 4 ) ) & 15 ) ;
 return slot ;
 }