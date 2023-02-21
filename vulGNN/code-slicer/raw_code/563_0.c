void mime_scanner_append ( MIMEScanner * scanner , const char * data , int data_size ) {
 int free_size = scanner -> m_line_size - scanner -> m_line_length ;
 if ( data_size > free_size ) {
 if ( scanner -> m_line_size == 0 ) {
 scanner -> m_line_size = 128 ;
 }
 while ( free_size < data_size ) {
 scanner -> m_line_size *= 2 ;
 free_size = scanner -> m_line_size - scanner -> m_line_length ;
 }
 if ( scanner -> m_line == nullptr ) {
 scanner -> m_line = ( char * ) ats_malloc ( scanner -> m_line_size ) ;
 }
 else {
 scanner -> m_line = ( char * ) ats_realloc ( scanner -> m_line , scanner -> m_line_size ) ;
 }
 }
 memcpy ( & ( scanner -> m_line [ scanner -> m_line_length ] ) , data , data_size ) ;
 scanner -> m_line_length += data_size ;
 }