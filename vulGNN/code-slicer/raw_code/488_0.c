void _mime_hdr_field_block_init ( MIMEFieldBlockImpl * fblock ) {
 fblock -> m_freetop = 0 ;
 fblock -> m_next = nullptr ;
 # ifdef BLOCK_INIT_PARANOIA int i ;
 for ( i = 0 ;
 i < MIME_FIELD_BLOCK_SLOTS ;
 i ++ ) {
 MIMEField * field = & ( fblock -> m_field_slots [ i ] ) ;
 field -> m_readiness = MIME_FIELD_SLOT_READINESS_EMPTY ;
 }
 # endif }