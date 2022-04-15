void mime_field_name_set ( HdrHeap * heap , MIMEHdrImpl * , MIMEField * field , int16_t name_wks_idx_or_neg1 , const char * name , int length , bool must_copy_string ) {
 ink_assert ( field -> m_readiness == MIME_FIELD_SLOT_READINESS_DETACHED ) ;
 field -> m_wks_idx = name_wks_idx_or_neg1 ;
 mime_str_u16_set ( heap , name , length , & ( field -> m_ptr_name ) , & ( field -> m_len_name ) , must_copy_string ) ;
 if ( ( name_wks_idx_or_neg1 == MIME_WKSIDX_CACHE_CONTROL ) || ( name_wks_idx_or_neg1 == MIME_WKSIDX_PRAGMA ) ) {
 field -> m_flags |= MIME_FIELD_SLOT_FLAGS_COOKED ;
 }
 }