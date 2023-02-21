void http_parser_init ( HTTPParser * parser ) {
 _http_parser_init ( parser ) ;
 mime_parser_init ( & parser -> m_mime_parser ) ;
 }