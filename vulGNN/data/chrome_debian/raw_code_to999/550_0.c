int mime_format_int64 ( char * buf , int64_t val , size_t buf_len ) {
 return ink_fast_ltoa ( val , buf , buf_len ) ;
 }