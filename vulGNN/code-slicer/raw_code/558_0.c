void mime_field_value_delete_comma_val ( HdrHeap * heap , MIMEHdrImpl * mh , MIMEField * field , int idx ) {
 int len ;
 Str * cell ;
 StrList list ( false ) ;
 HttpCompat : : parse_tok_list ( & list , 0 , field -> m_ptr_value , field -> m_len_value , ',' ) ;
 if ( ( idx < 0 ) || ( idx >= list . count ) ) {
 return ;
 }
 cell = list . get_idx ( idx ) ;
 list . detach ( cell ) ;
 if ( list . count == 0 ) {
 field -> m_ptr_value = nullptr ;
 field -> m_len_value = 0 ;
 }
 else {
 field -> m_ptr_value = mime_field_value_str_from_strlist ( heap , & len , & list ) ;
 field -> m_len_value = len ;
 }
 field -> m_n_v_raw_printable = 0 ;
 if ( field -> is_live ( ) && field -> is_cooked ( ) ) {
 mh -> recompute_cooked_stuff ( field ) ;
 }
 }