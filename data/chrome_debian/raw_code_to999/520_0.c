void mime_hdr_init ( MIMEHdrImpl * mh ) {
 mime_hdr_init_accelerators_and_presence_bits ( mh ) ;
 mime_hdr_cooked_stuff_init ( mh , nullptr ) ;
 obj_init_header ( ( HdrHeapObjImpl * ) & ( mh -> m_first_fblock ) , HDR_HEAP_OBJ_FIELD_BLOCK , sizeof ( MIMEFieldBlockImpl ) , 0 ) ;
 _mime_hdr_field_block_init ( & ( mh -> m_first_fblock ) ) ;
 mh -> m_fblock_list_tail = & ( mh -> m_first_fblock ) ;
 MIME_HDR_SANITY_CHECK ( mh ) ;
 }