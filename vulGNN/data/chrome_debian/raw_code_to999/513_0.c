void mime_hdr_destroy ( HdrHeap * heap , MIMEHdrImpl * mh ) {
 mime_hdr_destroy_field_block_list ( heap , mh -> m_first_fblock . m_next ) ;
 }