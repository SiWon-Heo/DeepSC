int mime_hdr_print ( HdrHeap * , MIMEHdrImpl * mh , char * buf_start , int buf_length , int * buf_index_inout , int * buf_chars_to_skip_inout ) {
 MIMEFieldBlockImpl * fblock ;
 MIMEField * field ;
 uint32_t index ;
 # define SIMPLE_MIME_HDR_PRINT # ifdef SIMPLE_MIME_HDR_PRINT for ( fblock = & ( mh -> m_first_fblock ) ;
 fblock != nullptr ;
 fblock = fblock -> m_next ) {
 for ( index = 0 ;
 index < fblock -> m_freetop ;
 index ++ ) {
 field = & ( fblock -> m_field_slots [ index ] ) ;
 if ( field -> is_live ( ) ) {
 if ( ! mime_field_print ( field , buf_start , buf_length , buf_index_inout , buf_chars_to_skip_inout ) ) {
 return 0 ;
 }
 }
 }
 }
 # else for ( fblock = & ( mh -> m_first_fblock ) ;
 fblock != NULL ;
 fblock = fblock -> m_next ) {
 const char * contig_start = NULL ;
 int this_length , contig_length = 0 ;
 for ( index = 0 ;
 index < fblock -> m_freetop ;
 index ++ ) {
 field = & ( fblock -> m_field_slots [ index ] ) ;
 this_length = field -> m_len_name + field -> m_len_value + field -> m_n_v_raw_printable_pad ;
 if ( field -> is_live ( ) ) {
 if ( ( field -> m_ptr_name == contig_start + contig_length ) && field -> m_n_v_raw_printable && ( ( buf_index_inout == NULL ) || ( contig_length + this_length <= buf_length - * buf_index_inout ) ) ) {
 contig_length += this_length ;
 }
 else {
 if ( contig_length > 0 ) {
 if ( ! mime_mem_print ( contig_start , contig_length , buf_start , buf_length , buf_index_inout , buf_chars_to_skip_inout ) ) return 0 ;
 }
 contig_start = field -> m_ptr_name ;
 contig_length = this_length ;
 }
 }
 }
 if ( contig_length > 0 ) {
 if ( ! mime_mem_print ( contig_start , contig_length , buf_start , buf_length , buf_index_inout , buf_chars_to_skip_inout ) ) return 0 ;
 }
 }
 # endif if ( ! mime_mem_print ( "\r\n" , 2 , buf_start , buf_length , buf_index_inout , buf_chars_to_skip_inout ) ) {
 return 0 ;
 }
 return 1 ;
 }