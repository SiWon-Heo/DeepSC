int mime_mem_print ( const char * src_d , int src_l , char * buf_start , int buf_length , int * buf_index_inout , int * buf_chars_to_skip_inout ) {
 int copy_l ;
 if ( buf_start == nullptr ) {
 ink_release_assert ( buf_index_inout == nullptr ) ;
 ink_release_assert ( buf_chars_to_skip_inout == nullptr ) ;
 while ( src_l -- ) {
 putchar ( * src_d ++ ) ;
 }
 return 1 ;
 }
 ink_assert ( buf_start != nullptr ) ;
 ink_assert ( src_d != nullptr ) ;
 if ( * buf_chars_to_skip_inout > 0 ) {
 if ( * buf_chars_to_skip_inout >= src_l ) {
 * buf_chars_to_skip_inout -= src_l ;
 return 1 ;
 }
 else {
 src_l -= * buf_chars_to_skip_inout ;
 src_d += * buf_chars_to_skip_inout ;
 * buf_chars_to_skip_inout = 0 ;
 }
 }
 copy_l = std : : min ( buf_length - * buf_index_inout , src_l ) ;
 if ( copy_l > 0 ) {
 memcpy ( buf_start + * buf_index_inout , src_d , copy_l ) ;
 * buf_index_inout += copy_l ;
 }
 return ( src_l == copy_l ) ;
 }