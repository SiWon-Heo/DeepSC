void mime_hdr_describe ( HdrHeapObjImpl * raw , bool recurse ) {
 MIMEFieldBlockImpl * fblock ;
 MIMEHdrImpl * obj = ( MIMEHdrImpl * ) raw ;
 Debug ( "http" , "\n\t[PBITS: 0x%08X%08X, SLACC: 0x%04X%04X%04X%04X, HEADBLK: %p, TAILBLK: %p]" , ( uint32_t ) ( ( obj -> m_presence_bits >> 32 ) & ( TOK_64_CONST ( 0xFFFFFFFF ) ) ) , ( uint32_t ) ( ( obj -> m_presence_bits >> 0 ) & ( TOK_64_CONST ( 0xFFFFFFFF ) ) ) , obj -> m_slot_accelerators [ 0 ] , obj -> m_slot_accelerators [ 1 ] , obj -> m_slot_accelerators [ 2 ] , obj -> m_slot_accelerators [ 3 ] , & ( obj -> m_first_fblock ) , obj -> m_fblock_list_tail ) ;
 Debug ( "http" , "\t[CBITS: 0x%08X, T_MAXAGE: %d, T_SMAXAGE: %d, T_MAXSTALE: %d, T_MINFRESH: %d, PNO$: %d]" , obj -> m_cooked_stuff . m_cache_control . m_mask , obj -> m_cooked_stuff . m_cache_control . m_secs_max_age , obj -> m_cooked_stuff . m_cache_control . m_secs_s_maxage , obj -> m_cooked_stuff . m_cache_control . m_secs_max_stale , obj -> m_cooked_stuff . m_cache_control . m_secs_min_fresh , obj -> m_cooked_stuff . m_pragma . m_no_cache ) ;
 for ( fblock = & ( obj -> m_first_fblock ) ;
 fblock != nullptr ;
 fblock = fblock -> m_next ) {
 if ( recurse || ( fblock == & ( obj -> m_first_fblock ) ) ) {
 obj_describe ( ( HdrHeapObjImpl * ) fblock , recurse ) ;
 }
 }
 }