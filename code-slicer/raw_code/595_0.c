void http_hdr_describe ( HdrHeapObjImpl * raw , bool recurse ) {
 HTTPHdrImpl * obj = ( HTTPHdrImpl * ) raw ;
 if ( obj -> m_polarity == HTTP_TYPE_REQUEST ) {
 Debug ( "http" , "[TYPE: REQ, V: %04X, URL: %p, METHOD: \"%.*s\", METHOD_LEN: %d, FIELDS: %p]" , obj -> m_version , obj -> u . req . m_url_impl , obj -> u . req . m_len_method , ( obj -> u . req . m_ptr_method ? obj -> u . req . m_ptr_method : "NULL" ) , obj -> u . req . m_len_method , obj -> m_fields_impl ) ;
 if ( recurse ) {
 if ( obj -> u . req . m_url_impl ) {
 obj_describe ( obj -> u . req . m_url_impl , recurse ) ;
 }
 if ( obj -> m_fields_impl ) {
 obj_describe ( obj -> m_fields_impl , recurse ) ;
 }
 }
 }
 else {
 Debug ( "http" , "[TYPE: RSP, V: %04X, STATUS: %d, REASON: \"%.*s\", REASON_LEN: %d, FIELDS: %p]" , obj -> m_version , obj -> u . resp . m_status , obj -> u . resp . m_len_reason , ( obj -> u . resp . m_ptr_reason ? obj -> u . resp . m_ptr_reason : "NULL" ) , obj -> u . resp . m_len_reason , obj -> m_fields_impl ) ;
 if ( recurse ) {
 if ( obj -> m_fields_impl ) {
 obj_describe ( obj -> m_fields_impl , recurse ) ;
 }
 }
 }
 }