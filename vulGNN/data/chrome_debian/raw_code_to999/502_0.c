void mime_hdr_destroy_field_block_list ( HdrHeap * heap , MIMEFieldBlockImpl * head ) {
 MIMEFieldBlockImpl * next ;
 while ( head != nullptr ) {
 next = head -> m_next ;
 _mime_field_block_destroy ( heap , head ) ;
 head = next ;
 }
 }