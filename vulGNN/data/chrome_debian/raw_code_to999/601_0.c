inline HTTPStatus http_hdr_status_get ( HTTPHdrImpl * hh ) {
 ink_assert ( hh -> m_polarity == HTTP_TYPE_RESPONSE ) ;
 return ( HTTPStatus ) hh -> u . resp . m_status ;
 }