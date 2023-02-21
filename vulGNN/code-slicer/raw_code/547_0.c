MIMEField * mime_field_create ( HdrHeap * heap , MIMEHdrImpl * mh ) {
 MIMEField * field ;
 MIMEFieldBlockImpl * tail_fblock , * new_fblock ;
 tail_fblock = mh -> m_fblock_list_tail ;
 if ( tail_fblock -> m_freetop >= MIME_FIELD_BLOCK_SLOTS ) {
 new_fblock = ( MIMEFieldBlockImpl * ) heap -> allocate_obj ( sizeof ( MIMEFieldBlockImpl ) , HDR_HEAP_OBJ_FIELD_BLOCK ) ;
 _mime_hdr_field_block_init ( new_fblock ) ;
 tail_fblock -> m_next = new_fblock ;
 tail_fblock = new_fblock ;
 mh -> m_fblock_list_tail = new_fblock ;
 }
 field = & ( tail_fblock -> m_field_slots [ tail_fblock -> m_freetop ] ) ;
 ++ tail_fblock -> m_freetop ;
 mime_field_init ( field ) ;
 return field ;
 }