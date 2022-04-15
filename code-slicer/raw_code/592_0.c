ParseResult http_parser_parse_resp ( HTTPParser * parser , HdrHeap * heap , HTTPHdrImpl * hh , const char * * start , const char * end , bool must_copy_strings , bool eof ) {
 if ( parser -> m_parsing_http ) {
 MIMEScanner * scanner = & parser -> m_mime_parser . m_scanner ;
 ParseResult err ;
 bool line_is_real ;
 const char * cur ;
 const char * line_start ;
 const char * real_end ;
 const char * version_start ;
 const char * version_end ;
 const char * status_start ;
 const char * status_end ;
 const char * reason_start ;
 const char * reason_end ;
 const char * old_start ;
 real_end = end ;
 old_start = * start ;
 hh -> m_polarity = HTTP_TYPE_RESPONSE ;
 if ( scanner -> m_line_length >= UINT16_MAX ) {
 return PARSE_RESULT_ERROR ;
 }
 err = mime_scanner_get ( scanner , start , real_end , & line_start , & end , & line_is_real , eof , MIME_SCANNER_TYPE_LINE ) ;
 if ( err < 0 ) {
 return err ;
 }
 if ( ( err == PARSE_RESULT_DONE ) || ( err == PARSE_RESULT_CONT ) ) {
 return err ;
 }
 cur = line_start ;
 ink_assert ( ( end - cur ) >= 0 ) ;
 ink_assert ( ( end - cur ) < UINT16_MAX ) ;
 must_copy_strings = ( must_copy_strings || ( ! line_is_real ) ) ;
 # if ( ENABLE_PARSER_FAST_PATHS ) if ( end - cur >= 16 ) {
 int http_match = ( ( cur [ 0 ] ^ 'H' ) | ( cur [ 1 ] ^ 'T' ) | ( cur [ 2 ] ^ 'T' ) | ( cur [ 3 ] ^ 'P' ) | ( cur [ 4 ] ^ '/' ) | ( cur [ 6 ] ^ '.' ) | ( cur [ 8 ] ^ ' ' ) ) ;
 if ( ( http_match != 0 ) || ( ! ( is_digit ( cur [ 5 ] ) && is_digit ( cur [ 7 ] ) && is_digit ( cur [ 9 ] ) && is_digit ( cur [ 10 ] ) && is_digit ( cur [ 11 ] ) && ( ! ParseRules : : is_space ( cur [ 13 ] ) ) ) ) ) {
 goto slow_case ;
 }
 reason_start = & ( cur [ 13 ] ) ;
 reason_end = end - 1 ;
 while ( ( reason_end > reason_start + 1 ) && ( ParseRules : : is_space ( reason_end [ - 1 ] ) ) ) {
 -- reason_end ;
 }
 int32_t version = HTTP_VERSION ( cur [ 5 ] - '0' , cur [ 7 ] - '0' ) ;
 HTTPStatus status = ( HTTPStatus ) ( ( cur [ 9 ] - '0' ) * 100 + ( cur [ 10 ] - '0' ) * 10 + ( cur [ 11 ] - '0' ) ) ;
 http_hdr_version_set ( hh , version ) ;
 http_hdr_status_set ( hh , status ) ;
 http_hdr_reason_set ( heap , hh , reason_start , ( int ) ( reason_end - reason_start ) , must_copy_strings ) ;
 end = real_end ;
 parser -> m_parsing_http = false ;
 return mime_parser_parse ( & parser -> m_mime_parser , heap , hh -> m_fields_impl , start , end , must_copy_strings , eof ) ;
 }
 # endif slow_case : version_start = nullptr ;
 version_end = nullptr ;
 status_start = nullptr ;
 status_end = nullptr ;
 reason_start = nullptr ;
 reason_end = nullptr ;
 version_start = cur = line_start ;
 if ( ( * cur != 'H' ) && ( * cur != 'h' ) ) {
 goto eoh ;
 }
 GETNEXT ( eoh ) ;
 if ( ( * cur != 'T' ) && ( * cur != 't' ) ) {
 goto eoh ;
 }
 GETNEXT ( eoh ) ;
 if ( ( * cur != 'T' ) && ( * cur != 't' ) ) {
 goto eoh ;
 }
 GETNEXT ( eoh ) ;
 if ( ( * cur != 'P' ) && ( * cur != 'p' ) ) {
 goto eoh ;
 }
 GETNEXT ( eoh ) ;
 if ( * cur != '/' ) {
 goto eoh ;
 }
 GETNEXT ( eoh ) ;
 parse_version2 : if ( ParseRules : : is_digit ( * cur ) ) {
 GETNEXT ( eoh ) ;
 goto parse_version2 ;
 }
 if ( * cur == '.' ) {
 GETNEXT ( eoh ) ;
 goto parse_version3 ;
 }
 goto eoh ;
 parse_version3 : if ( ParseRules : : is_digit ( * cur ) ) {
 GETNEXT ( eoh ) ;
 goto parse_version3 ;
 }
 if ( ParseRules : : is_ws ( * cur ) ) {
 version_end = cur ;
 GETNEXT ( eoh ) ;
 goto parse_status1 ;
 }
 goto eoh ;
 parse_status1 : if ( ParseRules : : is_ws ( * cur ) ) {
 GETNEXT ( done ) ;
 goto parse_status1 ;
 }
 status_start = cur ;
 parse_status2 : status_end = cur ;
 if ( ParseRules : : is_digit ( * cur ) ) {
 GETNEXT ( done ) ;
 goto parse_status2 ;
 }
 if ( ParseRules : : is_ws ( * cur ) ) {
 GETNEXT ( done ) ;
 goto parse_reason1 ;
 }
 goto done ;
 parse_reason1 : if ( ParseRules : : is_ws ( * cur ) ) {
 GETNEXT ( done ) ;
 goto parse_reason1 ;
 }
 reason_start = cur ;
 reason_end = end - 1 ;
 while ( ( reason_end >= line_start ) && ( ParseRules : : is_cr ( * reason_end ) || ParseRules : : is_lf ( * reason_end ) ) ) {
 reason_end -= 1 ;
 }
 reason_end += 1 ;
 goto done ;
 eoh : * start = old_start ;
 if ( parser -> m_allow_non_http ) {
 return PARSE_RESULT_DONE ;
 }
 else {
 return PARSE_RESULT_ERROR ;
 }
 done : if ( ! version_start || ! version_end ) {
 return PARSE_RESULT_ERROR ;
 }
 int32_t version ;
 version = http_parse_version ( version_start , version_end ) ;
 if ( version == HTTP_VERSION ( 0 , 9 ) ) {
 return PARSE_RESULT_ERROR ;
 }
 http_hdr_version_set ( hh , version ) ;
 if ( status_start && status_end ) {
 http_hdr_status_set ( hh , http_parse_status ( status_start , status_end ) ) ;
 }
 if ( reason_start && reason_end ) {
 http_hdr_reason_set ( heap , hh , reason_start , ( int ) ( reason_end - reason_start ) , must_copy_strings ) ;
 }
 end = real_end ;
 parser -> m_parsing_http = false ;
 }
 return mime_parser_parse ( & parser -> m_mime_parser , heap , hh -> m_fields_impl , start , end , must_copy_strings , eof ) ;
 }