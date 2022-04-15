void http_hdr_init ( HdrHeap * heap , HTTPHdrImpl * hh , HTTPType polarity ) {
 memset ( & ( hh -> u ) , 0 , sizeof ( hh -> u ) ) ;
 hh -> m_polarity = polarity ;
 hh -> m_version = HTTP_VERSION ( 1 , 0 ) ;
 hh -> m_fields_impl = mime_hdr_create ( heap ) ;
 if ( polarity == HTTP_TYPE_REQUEST ) {
 hh -> u . req . m_url_impl = url_create ( heap ) ;
 hh -> u . req . m_method_wks_idx = - 1 ;
 }
 }