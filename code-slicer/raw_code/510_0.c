void mime_parser_init ( MIMEParser * parser ) {
 mime_scanner_init ( & parser -> m_scanner ) ;
 _mime_parser_init ( parser ) ;
 }