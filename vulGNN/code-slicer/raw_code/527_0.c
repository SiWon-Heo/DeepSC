void mime_field_value_insert_comma_val ( HdrHeap * heap , MIMEHdrImpl * mh , MIMEField * field , int idx , const char * new_piece_str , int new_piece_len ) {
 int len ;
 Str * cell , * prev ;
 StrList list ( false ) ;
 HttpCompat : : parse_tok_list ( & list , 0 , field -> m_ptr_value , field -> m_len_value , ',' ) ;
 if ( idx < 0 ) {
 idx = list . count ;
 }
 if ( idx > list . count ) {
 return ;
 }
 cell = list . new_cell ( new_piece_str , new_piece_len ) ;
 if ( idx == 0 ) {
 list . prepend ( cell ) ;
 }
 else {
 prev = list . get_idx ( idx - 1 ) ;
 list . add_after ( prev , cell ) ;
 }
 field -> m_ptr_value = mime_field_value_str_from_strlist ( heap , & len , & list ) ;
 field -> m_len_value = len ;
 field -> m_n_v_raw_printable = 0 ;
 if ( field -> is_live ( ) && field -> is_cooked ( ) ) {
 mh -> recompute_cooked_stuff ( field ) ;
 }
 }