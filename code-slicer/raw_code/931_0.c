int ec_strsplit_ipport ( char * input , char * ip , u_int16 * port ) {
 static char ip_tmp [ MAX_ASCII_ADDR_LEN ] ;
 if ( sscanf ( input , "%20[0-9.]:%hu" , ip_tmp , port ) == 2 ) {
 strncpy ( ip , ip_tmp , strlen ( ip_tmp ) + 1 ) ;
 return E_SUCCESS ;
 }
 if ( sscanf ( input , "[%40[0-9a-fA-F:.]]:%hu" , ip_tmp , port ) == 2 ) {
 strncpy ( ip , ip_tmp , strlen ( ip_tmp ) + 1 ) ;
 return E_SUCCESS ;
 }
 DEBUG_MSG ( "ec_strsplit_ipport(): error splitting ip:port: '%s'\n" , input ) ;
 return - E_INVALID ;
 }