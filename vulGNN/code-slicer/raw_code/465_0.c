void mime_field_value_set ( HdrHeap * heap , MIMEHdrImpl * mh , MIMEField * field , const char * value , int length , bool must_copy_string ) {
 heap -> free_string ( field -> m_ptr_value , field -> m_len_value ) ;
 if ( must_copy_string && value ) {
 field -> m_ptr_value = heap -> duplicate_str ( value , length ) ;
 }
 else {
 field -> m_ptr_value = value ;
 }
 field -> m_len_value = length ;
 field -> m_n_v_raw_printable = 0 ;
 if ( field -> is_live ( ) && field -> is_cooked ( ) ) {
 mh -> recompute_cooked_stuff ( field ) ;
 }
 }