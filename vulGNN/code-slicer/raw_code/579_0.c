int http_hdr_length_get ( HTTPHdrImpl * hdr ) {
 int length = 0 ;
 if ( hdr -> m_polarity == HTTP_TYPE_REQUEST ) {
 if ( hdr -> u . req . m_ptr_method ) {
 length = hdr -> u . req . m_len_method ;
 }
 else {
 length = 0 ;
 }
 length += 1 ;
 if ( hdr -> u . req . m_url_impl ) {
 length += url_length_get ( hdr -> u . req . m_url_impl ) ;
 }
 length += 1 ;
 length += 8 ;
 length += 2 ;
 }
 else if ( hdr -> m_polarity == HTTP_TYPE_RESPONSE ) {
 if ( hdr -> u . resp . m_ptr_reason ) {
 length = hdr -> u . resp . m_len_reason ;
 }
 else {
 length = 0 ;
 }
 length += 8 ;
 length += 1 ;
 length += 3 ;
 length += 1 ;
 length += 2 ;
 }
 length += mime_hdr_length_get ( hdr -> m_fields_impl ) ;
 return length ;
 }