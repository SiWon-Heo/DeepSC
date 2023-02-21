const char * http_hdr_reason_get ( HTTPHdrImpl * hh , int * length ) {
 ink_assert ( hh -> m_polarity == HTTP_TYPE_RESPONSE ) ;
 * length = hh -> u . resp . m_len_reason ;
 return ( hh -> u . resp . m_ptr_reason ) ;
 }