ParseResult http_parser_parse_req ( HTTPParser * parser , HdrHeap * heap , HTTPHdrImpl * hh , const char * * start , const char * end , bool must_copy_strings , bool eof , bool strict_uri_parsing ) {
 if ( parser -> m_parsing_http ) {
 MIMEScanner * scanner = & parser -> m_mime_parser . m_scanner ;
 URLImpl * url ;
 ParseResult err ;
 bool line_is_real ;
 const char * cur ;
 const char * line_start ;
 const char * real_end ;
 const char * method_start ;
 const char * method_end ;
 const char * url_start ;
 const char * url_end ;
 const char * version_start ;
 const char * version_end ;
 real_end = end ;
 start : hh -> m_polarity = HTTP_TYPE_REQUEST ;
 if ( scanner -> m_line_length >= UINT16_MAX ) {
 return PARSE_RESULT_ERROR ;
 }
 err = mime_scanner_get ( scanner , start , real_end , & line_start , & end , & line_is_real , eof , MIME_SCANNER_TYPE_LINE ) ;
 if ( err < 0 ) {
 return err ;
 }
 if ( err == PARSE_RESULT_DONE ) {
 return PARSE_RESULT_ERROR ;
 }
 if ( err == PARSE_RESULT_CONT ) {
 return err ;
 }
 cur = line_start ;
 ink_assert ( ( end - cur ) >= 0 ) ;
 ink_assert ( ( end - cur ) < UINT16_MAX ) ;
 must_copy_strings = ( must_copy_strings || ( ! line_is_real ) ) ;
 # if ( ENABLE_PARSER_FAST_PATHS ) if ( end - cur >= 16 ) {
 if ( ( ( cur [ 0 ] ^ 'G' ) | ( cur [ 1 ] ^ 'E' ) | ( cur [ 2 ] ^ 'T' ) ) != 0 ) {
 goto slow_case ;
 }
 if ( ( ( end [ - 10 ] ^ 'H' ) | ( end [ - 9 ] ^ 'T' ) | ( end [ - 8 ] ^ 'T' ) | ( end [ - 7 ] ^ 'P' ) | ( end [ - 6 ] ^ '/' ) | ( end [ - 4 ] ^ '.' ) | ( end [ - 2 ] ^ '\r' ) | ( end [ - 1 ] ^ '\n' ) ) != 0 ) {
 goto slow_case ;
 }
 if ( ! ( is_digit ( end [ - 5 ] ) && is_digit ( end [ - 3 ] ) ) ) {
 goto slow_case ;
 }
 if ( ! ( ParseRules : : is_space ( cur [ 3 ] ) && ( ! ParseRules : : is_space ( cur [ 4 ] ) ) && ( ! ParseRules : : is_space ( end [ - 12 ] ) ) && ParseRules : : is_space ( end [ - 11 ] ) ) ) {
 goto slow_case ;
 }
 if ( & ( cur [ 4 ] ) >= & ( end [ - 11 ] ) ) {
 goto slow_case ;
 }
 int32_t version = HTTP_VERSION ( end [ - 5 ] - '0' , end [ - 3 ] - '0' ) ;
 http_hdr_method_set ( heap , hh , & ( cur [ 0 ] ) , hdrtoken_wks_to_index ( HTTP_METHOD_GET ) , 3 , must_copy_strings ) ;
 ink_assert ( hh -> u . req . m_url_impl != nullptr ) ;
 url = hh -> u . req . m_url_impl ;
 url_start = & ( cur [ 4 ] ) ;
 err = : : url_parse ( heap , url , & url_start , & ( end [ - 11 ] ) , must_copy_strings , strict_uri_parsing ) ;
 if ( err < 0 ) {
 return err ;
 }
 http_hdr_version_set ( hh , version ) ;
 end = real_end ;
 parser -> m_parsing_http = false ;
 if ( version == HTTP_VERSION ( 0 , 9 ) ) {
 return PARSE_RESULT_ERROR ;
 }
 ParseResult ret = mime_parser_parse ( & parser -> m_mime_parser , heap , hh -> m_fields_impl , start , end , must_copy_strings , eof ) ;
 if ( ret == PARSE_RESULT_DONE ) {
 ret = validate_hdr_host ( hh ) ;
 }
 return ret ;
 }
 # endif slow_case : method_start = nullptr ;
 method_end = nullptr ;
 url_start = nullptr ;
 url_end = nullptr ;
 version_start = nullptr ;
 version_end = nullptr ;
 url = nullptr ;
 if ( ParseRules : : is_cr ( * cur ) ) GETNEXT ( done ) ;
 if ( ParseRules : : is_lf ( * cur ) ) {
 goto start ;
 }
 parse_method1 : if ( ParseRules : : is_ws ( * cur ) ) {
 GETNEXT ( done ) ;
 goto parse_method1 ;
 }
 if ( ! ParseRules : : is_token ( * cur ) ) {
 goto done ;
 }
 method_start = cur ;
 GETNEXT ( done ) ;
 parse_method2 : if ( ParseRules : : is_ws ( * cur ) ) {
 method_end = cur ;
 goto parse_version1 ;
 }
 if ( ! ParseRules : : is_token ( * cur ) ) {
 goto done ;
 }
 GETNEXT ( done ) ;
 goto parse_method2 ;
 parse_version1 : cur = end - 1 ;
 if ( ParseRules : : is_lf ( * cur ) && ( cur >= line_start ) ) {
 cur -= 1 ;
 }
 if ( ParseRules : : is_cr ( * cur ) && ( cur >= line_start ) ) {
 cur -= 1 ;
 }
 while ( ParseRules : : is_ws ( * cur ) && ( cur >= line_start ) ) {
 cur -= 1 ;
 }
 version_end = cur + 1 ;
 parse_version2 : if ( ParseRules : : is_digit ( * cur ) ) {
 GETPREV ( parse_url ) ;
 goto parse_version2 ;
 }
 if ( * cur == '.' ) {
 GETPREV ( parse_url ) ;
 goto parse_version3 ;
 }
 goto parse_url ;
 parse_version3 : if ( ParseRules : : is_digit ( * cur ) ) {
 GETPREV ( parse_url ) ;
 goto parse_version3 ;
 }
 if ( * cur == '/' ) {
 GETPREV ( parse_url ) ;
 goto parse_version4 ;
 }
 goto parse_url ;
 parse_version4 : if ( ( * cur != 'P' ) && ( * cur != 'p' ) ) {
 goto parse_url ;
 }
 GETPREV ( parse_url ) ;
 if ( ( * cur != 'T' ) && ( * cur != 't' ) ) {
 goto parse_url ;
 }
 GETPREV ( parse_url ) ;
 if ( ( * cur != 'T' ) && ( * cur != 't' ) ) {
 goto parse_url ;
 }
 GETPREV ( parse_url ) ;
 if ( ( * cur != 'H' ) && ( * cur != 'h' ) ) {
 goto parse_url ;
 }
 version_start = cur ;
 parse_url : url_start = method_end + 1 ;
 if ( version_start ) {
 url_end = version_start - 1 ;
 }
 else {
 url_end = end - 1 ;
 }
 while ( ( url_start < end ) && ParseRules : : is_ws ( * url_start ) ) {
 url_start += 1 ;
 }
 while ( ( url_end >= line_start ) && ParseRules : : is_wslfcr ( * url_end ) ) {
 url_end -= 1 ;
 }
 url_end += 1 ;
 done : if ( ! method_start || ! method_end ) {
 return PARSE_RESULT_ERROR ;
 }
 ink_assert ( url_start ) ;
 ink_assert ( url_end ) ;
 int method_wks_idx = hdrtoken_tokenize ( method_start , ( int ) ( method_end - method_start ) ) ;
 http_hdr_method_set ( heap , hh , method_start , method_wks_idx , ( int ) ( method_end - method_start ) , must_copy_strings ) ;
 ink_assert ( hh -> u . req . m_url_impl != nullptr ) ;
 url = hh -> u . req . m_url_impl ;
 err = : : url_parse ( heap , url , & url_start , url_end , must_copy_strings , strict_uri_parsing ) ;
 if ( err < 0 ) {
 return err ;
 }
 int32_t version ;
 if ( version_start && version_end ) {
 version = http_parse_version ( version_start , version_end ) ;
 }
 else {
 return PARSE_RESULT_ERROR ;
 }
 if ( version == HTTP_VERSION ( 0 , 9 ) ) {
 return PARSE_RESULT_ERROR ;
 }
 http_hdr_version_set ( hh , version ) ;
 end = real_end ;
 parser -> m_parsing_http = false ;
 ParseResult ret = mime_parser_parse ( & parser -> m_mime_parser , heap , hh -> m_fields_impl , start , end , must_copy_strings , eof ) ;
 if ( ret == PARSE_RESULT_DONE ) {
 ret = validate_hdr_host ( hh ) ;
 }
 return ret ;
 }
 return mime_parser_parse ( & parser -> m_mime_parser , heap , hh -> m_fields_impl , start , end , must_copy_strings , eof ) ;
 }