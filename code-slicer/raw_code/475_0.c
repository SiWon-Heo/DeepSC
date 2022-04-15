int mime_format_int ( char * buf , int32_t val , size_t buf_len ) {
 return ink_fast_itoa ( val , buf , buf_len ) ;
 }