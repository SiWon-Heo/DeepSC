void http_hdr_copy_onto ( HTTPHdrImpl * s_hh , HdrHeap * s_heap , HTTPHdrImpl * d_hh , HdrHeap * d_heap , bool inherit_strs ) {
 MIMEHdrImpl * s_mh , * d_mh ;
 URLImpl * s_url , * d_url ;
 HTTPType d_polarity ;
 s_mh = s_hh -> m_fields_impl ;
 s_url = s_hh -> u . req . m_url_impl ;
 d_mh = d_hh -> m_fields_impl ;
 d_url = d_hh -> u . req . m_url_impl ;
 d_polarity = d_hh -> m_polarity ;
 ink_assert ( s_hh -> m_polarity != HTTP_TYPE_UNKNOWN ) ;
 ink_assert ( s_mh != nullptr ) ;
 ink_assert ( d_mh != nullptr ) ;
 memcpy ( d_hh , s_hh , sizeof ( HTTPHdrImpl ) ) ;
 d_hh -> m_fields_impl = d_mh ;
 if ( s_hh -> m_polarity == HTTP_TYPE_REQUEST ) {
 if ( d_polarity == HTTP_TYPE_REQUEST ) {
 d_hh -> u . req . m_url_impl = d_url ;
 }
 else {
 d_url = d_hh -> u . req . m_url_impl = url_create ( d_heap ) ;
 }
 url_copy_onto ( s_url , s_heap , d_url , d_heap , false ) ;
 }
 else if ( d_polarity == HTTP_TYPE_REQUEST ) {
 url_clear ( d_url ) ;
 }
 mime_hdr_copy_onto ( s_mh , s_heap , d_mh , d_heap , false ) ;
 if ( inherit_strs ) {
 d_heap -> inherit_string_heaps ( s_heap ) ;
 }
 }