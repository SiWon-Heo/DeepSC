void mime_field_name_value_set ( HdrHeap * heap , MIMEHdrImpl * mh , MIMEField * field , int16_t name_wks_idx_or_neg1 , const char * name , int name_length , const char * value , int value_length , int n_v_raw_printable , int n_v_raw_length , bool must_copy_strings ) {
 unsigned int n_v_raw_pad = n_v_raw_length - ( name_length + value_length ) ;
 ink_assert ( field -> m_readiness == MIME_FIELD_SLOT_READINESS_DETACHED ) ;
 if ( must_copy_strings ) {
 mime_field_name_set ( heap , mh , field , name_wks_idx_or_neg1 , name , name_length , true ) ;
 mime_field_value_set ( heap , mh , field , value , value_length , true ) ;
 }
 else {
 field -> m_wks_idx = name_wks_idx_or_neg1 ;
 field -> m_ptr_name = name ;
 field -> m_ptr_value = value ;
 field -> m_len_name = name_length ;
 field -> m_len_value = value_length ;
 if ( n_v_raw_printable && ( n_v_raw_pad <= 7 ) ) {
 field -> m_n_v_raw_printable = n_v_raw_printable ;
 field -> m_n_v_raw_printable_pad = n_v_raw_pad ;
 }
 else {
 field -> m_n_v_raw_printable = 0 ;
 }
 if ( ( name_wks_idx_or_neg1 == MIME_WKSIDX_CACHE_CONTROL ) || ( name_wks_idx_or_neg1 == MIME_WKSIDX_PRAGMA ) ) {
 field -> m_flags |= MIME_FIELD_SLOT_FLAGS_COOKED ;
 }
 if ( field -> is_live ( ) && field -> is_cooked ( ) ) {
 mh -> recompute_cooked_stuff ( field ) ;
 }
 }
 }