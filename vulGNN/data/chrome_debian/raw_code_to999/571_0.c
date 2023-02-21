void http_hdr_url_set ( HdrHeap * heap , HTTPHdrImpl * hh , URLImpl * url ) {
 ink_assert ( hh -> m_polarity == HTTP_TYPE_REQUEST ) ;
 if ( hh -> u . req . m_url_impl != url ) {
 if ( hh -> u . req . m_url_impl != nullptr ) {
 heap -> deallocate_obj ( hh -> u . req . m_url_impl ) ;
 }
 hh -> u . req . m_url_impl = url ;
 }
 }