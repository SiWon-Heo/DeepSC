void mime_parser_clear ( MIMEParser * parser ) {
 mime_scanner_clear ( & parser -> m_scanner ) ;
 _mime_parser_init ( parser ) ;
 }