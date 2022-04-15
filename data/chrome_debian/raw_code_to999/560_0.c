void mime_field_value_extend_comma_val ( HdrHeap * heap , MIMEHdrImpl * mh , MIMEField * field , int idx , const char * new_piece_str , int new_piece_len ) {
 Str * cell ;
 StrList list ( false ) ;
 int trimmed , len ;
 size_t extended_len ;
 char * dest , * temp_ptr , temp_buf [ 128 ] ;
 HttpCompat : : parse_tok_list ( & list , 0 , field -> m_ptr_value , field -> m_len_value , ',' ) ;
 if ( ( idx < 0 ) || ( idx >= list . count ) ) {
 return ;
 }
 cell = list . get_idx ( idx ) ;
 ink_assert ( cell != nullptr ) ;
 if ( ( cell -> len >= 2 ) && ( cell -> str [ 0 ] == '\"' ) && ( cell -> str [ cell -> len - 1 ] == '\"' ) ) {
 trimmed = 1 ;
 cell -> str += 1 ;
 cell -> len -= 2 ;
 }
 else {
 trimmed = 0 ;
 }
 extended_len = cell -> len + new_piece_len + ( trimmed ? 2 : 0 ) ;
 if ( extended_len <= sizeof ( temp_buf ) ) {
 temp_ptr = temp_buf ;
 }
 else {
 temp_ptr = ( char * ) ats_malloc ( extended_len ) ;
 }
 dest = temp_ptr ;
 if ( trimmed ) {
 * dest ++ = '\"' ;
 }
 memcpy ( dest , cell -> str , cell -> len ) ;
 dest += cell -> len ;
 memcpy ( dest , new_piece_str , new_piece_len ) ;
 dest += new_piece_len ;
 if ( trimmed ) {
 * dest ++ = '\"' ;
 }
 ink_assert ( ( size_t ) ( dest - temp_ptr ) == extended_len ) ;
 cell -> str = temp_ptr ;
 cell -> len = extended_len ;
 field -> m_ptr_value = mime_field_value_str_from_strlist ( heap , & len , & list ) ;
 field -> m_len_value = len ;
 field -> m_n_v_raw_printable = 0 ;
 if ( field -> is_live ( ) && field -> is_cooked ( ) ) {
 mh -> recompute_cooked_stuff ( field ) ;
 }
 if ( extended_len > sizeof ( temp_buf ) ) {
 ats_free ( temp_ptr ) ;
 }
 }