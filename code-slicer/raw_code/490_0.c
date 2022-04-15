ParseResult mime_scanner_get ( MIMEScanner * S , const char * * raw_input_s , const char * raw_input_e , const char * * output_s , const char * * output_e , bool * output_shares_raw_input , bool raw_input_eof , int raw_input_scan_type ) {
 const char * raw_input_c , * lf_ptr ;
 ParseResult zret = PARSE_RESULT_CONT ;
 static const char RAW_CR = ParseRules : : CHAR_CR ;
 ink_assert ( ( raw_input_s != nullptr ) && ( * raw_input_s != nullptr ) ) ;
 ink_assert ( raw_input_e != nullptr ) ;
 raw_input_c = * raw_input_s ;
 while ( PARSE_RESULT_CONT == zret && raw_input_c < raw_input_e ) {
 ptrdiff_t runway = raw_input_e - raw_input_c ;
 switch ( S -> m_state ) {
 case MIME_PARSE_BEFORE : if ( ParseRules : : is_cr ( * raw_input_c ) ) {
 ++ raw_input_c ;
 if ( runway >= 2 && ParseRules : : is_lf ( * raw_input_c ) ) {
 ++ raw_input_c ;
 zret = PARSE_RESULT_DONE ;
 }
 else {
 S -> m_state = MIME_PARSE_FOUND_CR ;
 }
 }
 else if ( ParseRules : : is_lf ( * raw_input_c ) ) {
 ++ raw_input_c ;
 zret = PARSE_RESULT_DONE ;
 }
 else {
 S -> m_state = MIME_PARSE_INSIDE ;
 }
 break ;
 case MIME_PARSE_FOUND_CR : if ( ParseRules : : is_lf ( * raw_input_c ) ) {
 ++ raw_input_c ;
 zret = PARSE_RESULT_DONE ;
 }
 else {
 mime_scanner_append ( S , & RAW_CR , 1 ) ;
 S -> m_state = MIME_PARSE_INSIDE ;
 }
 break ;
 case MIME_PARSE_INSIDE : lf_ptr = static_cast < const char * > ( memchr ( raw_input_c , ParseRules : : CHAR_LF , runway ) ) ;
 if ( lf_ptr ) {
 raw_input_c = lf_ptr + 1 ;
 if ( MIME_SCANNER_TYPE_LINE == raw_input_scan_type ) {
 zret = PARSE_RESULT_OK ;
 S -> m_state = MIME_PARSE_BEFORE ;
 }
 else {
 S -> m_state = MIME_PARSE_AFTER ;
 }
 }
 else {
 raw_input_c = raw_input_e ;
 }
 break ;
 case MIME_PARSE_AFTER : if ( ParseRules : : is_ws ( * raw_input_c ) ) {
 char * unfold = const_cast < char * > ( raw_input_c - 1 ) ;
 * unfold -- = ' ' ;
 if ( ParseRules : : is_cr ( * unfold ) ) {
 * unfold = ' ' ;
 }
 S -> m_state = MIME_PARSE_INSIDE ;
 }
 else {
 S -> m_state = MIME_PARSE_BEFORE ;
 zret = PARSE_RESULT_OK ;
 }
 break ;
 }
 }
 ptrdiff_t data_size = raw_input_c - * raw_input_s ;
 if ( PARSE_RESULT_CONT == zret ) {
 if ( raw_input_eof ) {
 if ( 0 == data_size ) {
 if ( MIME_PARSE_INSIDE != S -> m_state ) {
 S -> m_state = MIME_PARSE_BEFORE ;
 zret = PARSE_RESULT_DONE ;
 }
 else {
 zret = PARSE_RESULT_ERROR ;
 }
 }
 else if ( MIME_PARSE_AFTER == S -> m_state ) {
 S -> m_state = MIME_PARSE_BEFORE ;
 zret = PARSE_RESULT_OK ;
 }
 else {
 zret = PARSE_RESULT_ERROR ;
 }
 }
 else if ( data_size ) {
 if ( MIME_PARSE_INSIDE == S -> m_state ) {
 mime_scanner_append ( S , * raw_input_s , data_size ) ;
 data_size = 0 ;
 }
 else if ( MIME_PARSE_AFTER == S -> m_state ) {
 S -> m_state = MIME_PARSE_BEFORE ;
 zret = PARSE_RESULT_OK ;
 }
 }
 }
 if ( data_size && S -> m_line_length ) {
 mime_scanner_append ( S , * raw_input_s , data_size ) ;
 }
 * output_shares_raw_input = 0 == S -> m_line_length ;
 if ( PARSE_RESULT_CONT != zret ) {
 if ( 0 != S -> m_line_length ) {
 * output_s = S -> m_line ;
 * output_e = * output_s + S -> m_line_length ;
 S -> m_line_length = 0 ;
 }
 else {
 * output_s = * raw_input_s ;
 * output_e = raw_input_c ;
 }
 }
 if ( zret != PARSE_RESULT_ERROR && memchr ( * raw_input_s , '\0' , raw_input_c - * raw_input_s ) != nullptr ) {
 zret = PARSE_RESULT_ERROR ;
 }
 * raw_input_s = raw_input_c ;
 return zret ;
 }