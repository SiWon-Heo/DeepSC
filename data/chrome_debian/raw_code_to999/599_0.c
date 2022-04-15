void http_hdr_reason_set ( HdrHeap * heap , HTTPHdrImpl * hh , const char * value , int length , bool must_copy ) {
 ink_assert ( hh -> m_polarity == HTTP_TYPE_RESPONSE ) ;
 mime_str_u16_set ( heap , value , length , & ( hh -> u . resp . m_ptr_reason ) , & ( hh -> u . resp . m_len_reason ) , must_copy ) ;
 }