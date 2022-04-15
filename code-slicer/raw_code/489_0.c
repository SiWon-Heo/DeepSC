inline void mime_hdr_set_accelerator_slotnum ( MIMEHdrImpl * mh , int32_t slot_id , uint32_t slot_num ) {
 ink_assert ( ( slot_id != MIME_SLOTID_NONE ) && ( slot_id < 32 ) ) ;
 ink_assert ( slot_num < 16 ) ;
 uint32_t word_index = slot_id / 8 ;
 uint32_t word = mh -> m_slot_accelerators [ word_index ] ;
 uint32_t nybble = slot_id % 8 ;
 uint32_t shift = nybble * 4 ;
 uint32_t mask = ~ ( MIME_FIELD_SLOTNUM_MASK << shift ) ;
 uint32_t graft = ( slot_num << shift ) ;
 uint32_t new_word = ( word & mask ) | graft ;
 mh -> m_slot_accelerators [ word_index ] = new_word ;
 }