void mime_field_value_append ( HdrHeap * heap , MIMEHdrImpl * mh , MIMEField * field , const char * value , int length , bool prepend_comma , const char separator ) {
 int new_length = field -> m_len_value + length ;
 if ( prepend_comma && field -> m_len_value ) {
 new_length += 2 ;
 }
 char * new_str = heap -> expand_str ( field -> m_ptr_value , field -> m_len_value , new_length ) ;
 if ( new_str == nullptr ) {
 new_str = heap -> allocate_str ( new_length ) ;
 memcpy ( new_str , field -> m_ptr_value , field -> m_len_value ) ;
 }
 char * ptr = new_str + field -> m_len_value ;
 if ( prepend_comma && field -> m_len_value ) {
 * ptr ++ = separator ;
 * ptr ++ = ' ' ;
 }
 memcpy ( ptr , value , length ) ;
 field -> m_ptr_value = new_str ;
 field -> m_len_value = new_length ;
 field -> m_n_v_raw_printable = 0 ;
 if ( field -> is_live ( ) && field -> is_cooked ( ) ) {
 mh -> recompute_cooked_stuff ( field ) ;
 }
 }