void mime_hdr_cooked_stuff_init ( MIMEHdrImpl * mh , MIMEField * changing_field_or_null ) {
 if ( ( changing_field_or_null == nullptr ) || ( changing_field_or_null -> m_wks_idx != MIME_WKSIDX_PRAGMA ) ) {
 mh -> m_cooked_stuff . m_cache_control . m_mask = 0 ;
 mh -> m_cooked_stuff . m_cache_control . m_secs_max_age = 0 ;
 mh -> m_cooked_stuff . m_cache_control . m_secs_s_maxage = 0 ;
 mh -> m_cooked_stuff . m_cache_control . m_secs_max_stale = 0 ;
 mh -> m_cooked_stuff . m_cache_control . m_secs_min_fresh = 0 ;
 }
 if ( ( changing_field_or_null == nullptr ) || ( changing_field_or_null -> m_wks_idx != MIME_WKSIDX_CACHE_CONTROL ) ) {
 mh -> m_cooked_stuff . m_pragma . m_no_cache = false ;
 }
 }