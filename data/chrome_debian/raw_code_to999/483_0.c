void _mime_scanner_init ( MIMEScanner * scanner ) {
 scanner -> m_line = nullptr ;
 scanner -> m_line_size = 0 ;
 scanner -> m_line_length = 0 ;
 scanner -> m_state = MIME_PARSE_BEFORE ;
 }