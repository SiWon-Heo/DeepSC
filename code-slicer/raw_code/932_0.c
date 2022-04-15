char * str_tohex ( u_char * bin , size_t len , char * dst , size_t dst_len ) {
 size_t i ;
 memset ( dst , 0 , dst_len ) ;
 for ( i = 0 ;
 i < len ;
 i ++ ) sprintf ( dst + i * 2 , "%02X" , bin [ i ] ) ;
 return dst ;
 }