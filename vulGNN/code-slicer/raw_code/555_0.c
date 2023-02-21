void mime_hdr_field_detach ( MIMEHdrImpl * mh , MIMEField * field , bool detach_all_dups ) {
 MIMEField * next_dup = field -> m_next_dup ;
 if ( field -> is_detached ( ) ) {
 ink_assert ( next_dup == nullptr ) ;
 return ;
 }
 ink_assert ( field -> is_live ( ) ) ;
 MIME_HDR_SANITY_CHECK ( mh ) ;
 if ( field -> m_flags & MIME_FIELD_SLOT_FLAGS_DUP_HEAD ) {
 if ( ! next_dup ) {
 mime_hdr_unset_accelerators_and_presence_bits ( mh , field ) ;
 }
 else {
 next_dup -> m_flags |= MIME_FIELD_SLOT_FLAGS_DUP_HEAD ;
 mime_hdr_set_accelerators_and_presence_bits ( mh , next_dup ) ;
 }
 }
 else {
 int name_length ;
 const char * name = mime_field_name_get ( field , & name_length ) ;
 MIMEField * prev = mime_hdr_field_find ( mh , name , name_length ) ;
 while ( prev && ( prev -> m_next_dup != field ) ) {
 prev = prev -> m_next_dup ;
 }
 ink_assert ( prev != nullptr ) ;
 if ( prev -> m_next_dup == field ) {
 prev -> m_next_dup = next_dup ;
 }
 }
 field -> m_readiness = MIME_FIELD_SLOT_READINESS_DETACHED ;
 field -> m_next_dup = nullptr ;
 if ( field -> is_cooked ( ) ) {
 mh -> recompute_cooked_stuff ( field ) ;
 }
 MIME_HDR_SANITY_CHECK ( mh ) ;
 if ( detach_all_dups && next_dup ) {
 mime_hdr_field_detach ( mh , next_dup , detach_all_dups ) ;
 }
 }