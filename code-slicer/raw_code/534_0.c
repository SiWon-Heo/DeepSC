void mime_hdr_fields_clear ( HdrHeap * heap , MIMEHdrImpl * mh ) {
 mime_hdr_destroy_field_block_list ( heap , mh -> m_first_fblock . m_next ) ;
 mime_hdr_init ( mh ) ;
 }