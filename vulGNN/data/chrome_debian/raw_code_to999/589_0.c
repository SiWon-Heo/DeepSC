ParseResult validate_hdr_host ( HTTPHdrImpl * hh ) {
 ParseResult ret = PARSE_RESULT_DONE ;
 MIMEField * host_field = mime_hdr_field_find ( hh -> m_fields_impl , MIME_FIELD_HOST , MIME_LEN_HOST ) ;
 if ( host_field ) {
 if ( host_field -> has_dups ( ) ) {
 ret = PARSE_RESULT_ERROR ;
 }
 else {
 int host_len = 0 ;
 const char * host_val = host_field -> value_get ( & host_len ) ;
 ts : : ConstBuffer addr , port , rest , host ( host_val , host_len ) ;
 if ( 0 == ats_ip_parse ( host , & addr , & port , & rest ) ) {
 if ( port ) {
 if ( port . size ( ) > 5 ) {
 return PARSE_RESULT_ERROR ;
 }
 int port_i = ink_atoi ( port . data ( ) , port . size ( ) ) ;
 if ( port_i >= 65536 || port_i <= 0 ) {
 return PARSE_RESULT_ERROR ;
 }
 }
 if ( ! validate_host_name ( addr ) ) {
 return PARSE_RESULT_ERROR ;
 }
 while ( rest && PARSE_RESULT_DONE == ret ) {
 if ( ! ParseRules : : is_ws ( * rest ) ) {
 return PARSE_RESULT_ERROR ;
 }
 ++ rest ;
 }
 }
 else {
 ret = PARSE_RESULT_ERROR ;
 }
 }
 }
 return ret ;
 }