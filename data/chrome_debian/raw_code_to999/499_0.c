void mime_scanner_clear ( MIMEScanner * scanner ) {
 ats_free ( scanner -> m_line ) ;
 _mime_scanner_init ( scanner ) ;
 }