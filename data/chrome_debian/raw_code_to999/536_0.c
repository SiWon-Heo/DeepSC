const char * mime_str_u16_set ( HdrHeap * heap , const char * s_str , int s_len , const char * * d_str , uint16_t * d_len , bool must_copy ) {
 ink_assert ( s_len >= 0 && s_len < UINT16_MAX ) ;
 heap -> free_string ( * d_str , * d_len ) ;
 if ( must_copy && s_str ) {
 s_str = heap -> duplicate_str ( s_str , s_len ) ;
 }
 * d_str = s_str ;
 * d_len = s_len ;
 return s_str ;
 }