void mime_hdr_copy_onto ( MIMEHdrImpl * s_mh , HdrHeap * s_heap , MIMEHdrImpl * d_mh , HdrHeap * d_heap , bool inherit_strs ) {
 int block_count ;
 MIMEFieldBlockImpl * s_fblock , * d_fblock , * prev_d_fblock ;
 if ( d_mh -> m_first_fblock . m_next ) {
 mime_hdr_destroy_field_block_list ( d_heap , d_mh -> m_first_fblock . m_next ) ;
 }
 ink_assert ( ( ( char * ) & ( s_mh -> m_first_fblock . m_field_slots [ MIME_FIELD_BLOCK_SLOTS ] ) - ( char * ) s_mh ) == sizeof ( struct MIMEHdrImpl ) ) ;
 int top = s_mh -> m_first_fblock . m_freetop ;
 char * end = ( char * ) & ( s_mh -> m_first_fblock . m_field_slots [ top ] ) ;
 int bytes_below_top = end - ( char * ) s_mh ;
 memcpy ( d_mh , s_mh , bytes_below_top ) ;
 if ( d_mh -> m_first_fblock . m_next == nullptr ) {
 d_mh -> m_fblock_list_tail = & ( d_mh -> m_first_fblock ) ;
 block_count = 1 ;
 }
 else {
 prev_d_fblock = & ( d_mh -> m_first_fblock ) ;
 block_count = 1 ;
 for ( s_fblock = s_mh -> m_first_fblock . m_next ;
 s_fblock != nullptr ;
 s_fblock = s_fblock -> m_next ) {
 ++ block_count ;
 d_fblock = _mime_field_block_copy ( s_fblock , s_heap , d_heap ) ;
 prev_d_fblock -> m_next = d_fblock ;
 prev_d_fblock = d_fblock ;
 }
 d_mh -> m_fblock_list_tail = prev_d_fblock ;
 }
 if ( inherit_strs ) {
 d_heap -> inherit_string_heaps ( s_heap ) ;
 }
 mime_hdr_field_block_list_adjust ( block_count , & ( s_mh -> m_first_fblock ) , & ( d_mh -> m_first_fblock ) ) ;
 MIME_HDR_SANITY_CHECK ( s_mh ) ;
 MIME_HDR_SANITY_CHECK ( d_mh ) ;
 }