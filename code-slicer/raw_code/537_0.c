inline void mime_hdr_reset_accelerators_and_presence_bits ( MIMEHdrImpl * mh ) {
 mime_hdr_init_accelerators_and_presence_bits ( mh ) ;
 for ( MIMEFieldBlockImpl * fblock = & ( mh -> m_first_fblock ) ;
 fblock != nullptr ;
 fblock = fblock -> m_next ) {
 for ( MIMEField * field = fblock -> m_field_slots , * limit = field + fblock -> m_freetop ;
 field < limit ;
 ++ field ) {
 if ( field -> is_live ( ) ) {
 field -> m_wks_idx = hdrtoken_tokenize ( field -> m_ptr_name , field -> m_len_name ) ;
 if ( field -> is_dup_head ( ) ) {
 mime_hdr_set_accelerators_and_presence_bits ( mh , field ) ;
 }
 }
 }
 }
 }