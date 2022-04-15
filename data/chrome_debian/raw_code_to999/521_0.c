int mime_format_uint ( char * buf , uint32_t val , size_t buf_len ) {
 return ink_fast_uitoa ( val , buf , buf_len ) ;
 }