int str_hex_to_bytes ( char * string , u_char * bytes ) {
 char value [ 3 ] ;
 unsigned int value_bin ;
 u_int i ;
 size_t slen ;
 slen = strlen ( string ) ;
 for ( i = 0 ;
 i < slen ;
 i ++ ) {
 strncpy ( value , string + i * 2 , 2 ) ;
 if ( sscanf ( value , "%02X" , & value_bin ) != 1 ) return - E_INVALID ;
 bytes [ i ] = value_bin & 0x000000FF ;
 }
 return 0 ;
 }