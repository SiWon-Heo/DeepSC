void http_hdr_status_set ( HTTPHdrImpl * hh , HTTPStatus status ) {
 ink_assert ( hh -> m_polarity == HTTP_TYPE_RESPONSE ) ;
 hh -> u . resp . m_status = status ;
 }