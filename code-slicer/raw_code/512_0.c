int mime_field_print ( MIMEField * field , char * buf_start , int buf_length , int * buf_index_inout , int * buf_chars_to_skip_inout ) {
 # define TRY ( x ) if ( ! x ) return 0 int total_len ;
 if ( field -> m_ptr_name [ 0 ] == '@' ) {
 return 1 ;
 }
 if ( field -> m_n_v_raw_printable ) {
 total_len = field -> m_len_name + field -> m_len_value + field -> m_n_v_raw_printable_pad ;
 if ( ( buf_start != nullptr ) && ( * buf_chars_to_skip_inout == 0 ) && ( total_len <= ( buf_length - * buf_index_inout ) ) ) {
 buf_start += * buf_index_inout ;
 memcpy ( buf_start , field -> m_ptr_name , total_len ) ;
 * buf_index_inout += total_len ;
 }
 else {
 TRY ( mime_mem_print ( field -> m_ptr_name , total_len , buf_start , buf_length , buf_index_inout , buf_chars_to_skip_inout ) ) ;
 }
 }
 else {
 total_len = field -> m_len_name + field -> m_len_value + 2 + 2 ;
 if ( ( buf_start != nullptr ) && ( * buf_chars_to_skip_inout == 0 ) && ( total_len <= ( buf_length - * buf_index_inout ) ) ) {
 buf_start += * buf_index_inout ;
 memcpy ( buf_start , field -> m_ptr_name , field -> m_len_name ) ;
 buf_start += field -> m_len_name ;
 buf_start [ 0 ] = ':' ;
 buf_start [ 1 ] = ' ' ;
 buf_start += 2 ;
 memcpy ( buf_start , field -> m_ptr_value , field -> m_len_value ) ;
 buf_start += field -> m_len_value ;
 buf_start [ 0 ] = '\r' ;
 buf_start [ 1 ] = '\n' ;
 * buf_index_inout += total_len ;
 }
 else {
 TRY ( mime_mem_print ( field -> m_ptr_name , field -> m_len_name , buf_start , buf_length , buf_index_inout , buf_chars_to_skip_inout ) ) ;
 TRY ( mime_mem_print ( ": " , 2 , buf_start , buf_length , buf_index_inout , buf_chars_to_skip_inout ) ) ;
 TRY ( mime_mem_print ( field -> m_ptr_value , field -> m_len_value , buf_start , buf_length , buf_index_inout , buf_chars_to_skip_inout ) ) ;
 TRY ( mime_mem_print ( "\r\n" , 2 , buf_start , buf_length , buf_index_inout , buf_chars_to_skip_inout ) ) ;
 }
 }
 return 1 ;
 # undef TRY }