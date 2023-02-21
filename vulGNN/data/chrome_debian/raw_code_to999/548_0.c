void mime_field_block_describe ( HdrHeapObjImpl * raw , bool ) {
 unsigned int i ;
 static const char * readiness_names [ ] = {
 "EMPTY" , "DETACHED" , "LIVE" , "DELETED" }
 ;
 MIMEFieldBlockImpl * obj = ( MIMEFieldBlockImpl * ) raw ;
 Debug ( "http" , "[FREETOP: %d, NEXTBLK: %p]" , obj -> m_freetop , obj -> m_next ) ;
 for ( i = 0 ;
 i < obj -> m_freetop ;
 i ++ ) {
 MIMEField * f = & ( obj -> m_field_slots [ i ] ) ;
 Debug ( "http" , "\tSLOT #%2d (%p), %-8s" , i , f , readiness_names [ f -> m_readiness ] ) ;
 switch ( f -> m_readiness ) {
 case MIME_FIELD_SLOT_READINESS_EMPTY : break ;
 case MIME_FIELD_SLOT_READINESS_DETACHED : case MIME_FIELD_SLOT_READINESS_LIVE : case MIME_FIELD_SLOT_READINESS_DELETED : Debug ( "http" , "[N: \"%.*s\", N_LEN: %d, N_IDX: %d, " , f -> m_len_name , ( f -> m_ptr_name ? f -> m_ptr_name : "NULL" ) , f -> m_len_name , f -> m_wks_idx ) ;
 Debug ( "http" , "V: \"%.*s\", V_LEN: %d, " , f -> m_len_value , ( f -> m_ptr_value ? f -> m_ptr_value : "NULL" ) , f -> m_len_value ) ;
 Debug ( "http" , "NEXTDUP: %p, RAW: %d, RAWLEN: %d, F: %d]" , f -> m_next_dup , f -> m_n_v_raw_printable , f -> m_len_name + f -> m_len_value + f -> m_n_v_raw_printable_pad , f -> m_flags ) ;
 break ;
 }
 Debug ( "http" , "\n" ) ;
 }
 }