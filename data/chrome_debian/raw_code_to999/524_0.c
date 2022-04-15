void mime_hdr_field_delete ( HdrHeap * heap , MIMEHdrImpl * mh , MIMEField * field , bool delete_all_dups ) {
 if ( delete_all_dups ) {
 while ( field ) {
 MIMEField * next = field -> m_next_dup ;
 heap -> free_string ( field -> m_ptr_name , field -> m_len_name ) ;
 heap -> free_string ( field -> m_ptr_value , field -> m_len_value ) ;
 MIME_HDR_SANITY_CHECK ( mh ) ;
 mime_hdr_field_detach ( mh , field , false ) ;
 MIME_HDR_SANITY_CHECK ( mh ) ;
 mime_field_destroy ( mh , field ) ;
 MIME_HDR_SANITY_CHECK ( mh ) ;
 field = next ;
 }
 }
 else {
 heap -> free_string ( field -> m_ptr_name , field -> m_len_name ) ;
 heap -> free_string ( field -> m_ptr_value , field -> m_len_value ) ;
 MIME_HDR_SANITY_CHECK ( mh ) ;
 mime_hdr_field_detach ( mh , field , false ) ;
 MIME_HDR_SANITY_CHECK ( mh ) ;
 mime_field_destroy ( mh , field ) ;
 }
 MIME_HDR_SANITY_CHECK ( mh ) ;
 }