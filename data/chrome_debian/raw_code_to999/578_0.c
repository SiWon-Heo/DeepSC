void http_parser_clear ( HTTPParser * parser ) {
 _http_parser_init ( parser ) ;
 mime_parser_clear ( & parser -> m_mime_parser ) ;
 }