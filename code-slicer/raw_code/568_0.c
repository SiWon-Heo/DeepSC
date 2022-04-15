ParseResult validate_hdr_content_length ( HdrHeap * heap , HTTPHdrImpl * hh ) {
 MIMEField * content_length_field = mime_hdr_field_find ( hh -> m_fields_impl , MIME_FIELD_CONTENT_LENGTH , MIME_LEN_CONTENT_LENGTH ) ;
 if ( content_length_field ) {
 if ( mime_hdr_field_find ( hh -> m_fields_impl , MIME_FIELD_TRANSFER_ENCODING , MIME_LEN_TRANSFER_ENCODING ) != nullptr ) {
 Debug ( "http" , "Transfer-Encoding header and Content-Length headers the request, removing all Content-Length headers" ) ;
 mime_hdr_field_delete ( heap , hh -> m_fields_impl , content_length_field ) ;
 return PARSE_RESULT_DONE ;
 }
 int content_length_len = 0 ;
 const char * content_length_val = content_length_field -> value_get ( & content_length_len ) ;
 while ( content_length_field -> has_dups ( ) ) {
 int content_length_len_2 = 0 ;
 const char * content_length_val_2 = content_length_field -> m_next_dup -> value_get ( & content_length_len_2 ) ;
 if ( ( content_length_len != content_length_len_2 ) || ( memcmp ( content_length_val , content_length_val_2 , content_length_len ) != 0 ) ) {
 Debug ( "http" , "Content-Length headers don't match, returning parse error" ) ;
 return PARSE_RESULT_ERROR ;
 }
 else {
 Debug ( "http" , "Deleting duplicate Content-Length header" ) ;
 mime_hdr_field_delete ( heap , hh -> m_fields_impl , content_length_field -> m_next_dup , false ) ;
 }
 }
 }
 return PARSE_RESULT_DONE ;
 }