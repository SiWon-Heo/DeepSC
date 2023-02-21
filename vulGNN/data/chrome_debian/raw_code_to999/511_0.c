void mime_hdr_field_attach ( MIMEHdrImpl * mh , MIMEField * field , int check_for_dups , MIMEField * prev_dup ) {
 MIME_HDR_SANITY_CHECK ( mh ) ;
 if ( ! field -> is_detached ( ) ) {
 return ;
 }
 ink_assert ( field -> m_ptr_name != nullptr ) ;
 if ( check_for_dups || ( prev_dup && ( ! prev_dup -> is_dup_head ( ) ) ) ) {
 int length ;
 const char * name = mime_field_name_get ( field , & length ) ;
 prev_dup = mime_hdr_field_find ( mh , name , length ) ;
 ink_assert ( ( prev_dup == nullptr ) || ( prev_dup -> is_dup_head ( ) ) ) ;
 }
 field -> m_readiness = MIME_FIELD_SLOT_READINESS_LIVE ;
 if ( prev_dup ) {
 MIMEField * next_dup ;
 int field_slotnum , prev_slotnum , next_slotnum ;
 field_slotnum = mime_hdr_field_slotnum ( mh , field ) ;
 prev_slotnum = mime_hdr_field_slotnum ( mh , prev_dup ) ;
 next_dup = prev_dup -> m_next_dup ;
 next_slotnum = ( next_dup ? mime_hdr_field_slotnum ( mh , next_dup ) : - 1 ) ;
 ink_assert ( field_slotnum != prev_slotnum ) ;
 while ( prev_slotnum < field_slotnum ) {
 if ( next_dup == nullptr ) {
 break ;
 }
 if ( next_slotnum > field_slotnum ) {
 break ;
 }
 prev_dup = next_dup ;
 prev_slotnum = next_slotnum ;
 next_dup = prev_dup -> m_next_dup ;
 }
 if ( prev_slotnum > field_slotnum ) {
 field -> m_flags = ( field -> m_flags | MIME_FIELD_SLOT_FLAGS_DUP_HEAD ) ;
 field -> m_next_dup = prev_dup ;
 prev_dup -> m_flags = ( prev_dup -> m_flags & ~ MIME_FIELD_SLOT_FLAGS_DUP_HEAD ) ;
 mime_hdr_set_accelerators_and_presence_bits ( mh , field ) ;
 }
 else {
 ink_assert ( prev_slotnum < field_slotnum ) ;
 ink_assert ( ( next_dup == nullptr ) || ( next_slotnum > field_slotnum ) ) ;
 field -> m_flags = ( field -> m_flags & ~ MIME_FIELD_SLOT_FLAGS_DUP_HEAD ) ;
 ink_assert ( ( next_dup == nullptr ) || next_dup -> is_live ( ) ) ;
 prev_dup -> m_next_dup = field ;
 field -> m_next_dup = next_dup ;
 }
 }
 else {
 field -> m_flags = ( field -> m_flags | MIME_FIELD_SLOT_FLAGS_DUP_HEAD ) ;
 mime_hdr_set_accelerators_and_presence_bits ( mh , field ) ;
 }
 ink_assert ( field -> is_live ( ) ) ;
 if ( field -> m_ptr_value && field -> is_cooked ( ) ) {
 mh -> recompute_cooked_stuff ( field ) ;
 }
 MIME_HDR_SANITY_CHECK ( mh ) ;
 }