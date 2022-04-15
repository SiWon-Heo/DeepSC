inline void mime_hdr_init_accelerators_and_presence_bits ( MIMEHdrImpl * mh ) {
 mh -> m_presence_bits = 0 ;
 mh -> m_slot_accelerators [ 0 ] = 0xFFFFFFFF ;
 mh -> m_slot_accelerators [ 1 ] = 0xFFFFFFFF ;
 mh -> m_slot_accelerators [ 2 ] = 0xFFFFFFFF ;
 mh -> m_slot_accelerators [ 3 ] = 0xFFFFFFFF ;
 }