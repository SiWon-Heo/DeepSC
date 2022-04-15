void http_hdr_method_set ( HdrHeap * heap , HTTPHdrImpl * hh , const char * method , int16_t method_wks_idx , int method_length , bool must_copy ) {
 ink_assert ( hh -> m_polarity == HTTP_TYPE_REQUEST ) ;
 hh -> u . req . m_method_wks_idx = method_wks_idx ;
 mime_str_u16_set ( heap , method , method_length , & ( hh -> u . req . m_ptr_method ) , & ( hh -> u . req . m_len_method ) , must_copy ) ;
 }