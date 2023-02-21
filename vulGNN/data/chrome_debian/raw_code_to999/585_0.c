const char * http_hdr_method_get ( HTTPHdrImpl * hh , int * length ) {
 const char * str ;
 ink_assert ( hh -> m_polarity == HTTP_TYPE_REQUEST ) ;
 if ( hh -> u . req . m_method_wks_idx >= 0 ) {
 str = hdrtoken_index_to_wks ( hh -> u . req . m_method_wks_idx ) ;
 * length = hdrtoken_index_to_length ( hh -> u . req . m_method_wks_idx ) ;
 }
 else {
 str = hh -> u . req . m_ptr_method ;
 * length = hh -> u . req . m_len_method ;
 }
 return ( str ) ;
 }