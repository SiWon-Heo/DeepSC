int strescape ( char * dst , char * src , size_t len ) {
 char * olddst = dst ;
 char * oldsrc = src ;
 int c ;
 int val ;
 while ( ( c = * src ++ ) != '\0' && ( size_t ) ( src - oldsrc ) <= len ) {
 if ( c == '\\' ) {
 switch ( ( c = * src ++ ) ) {
 case '\0' : goto strend ;
 default : * dst ++ = ( char ) c ;
 break ;
 case 'n' : * dst ++ = '\n' ;
 break ;
 case 'r' : * dst ++ = '\r' ;
 break ;
 case 'b' : * dst ++ = '\b' ;
 break ;
 case 't' : * dst ++ = '\t' ;
 break ;
 case 'f' : * dst ++ = '\f' ;
 break ;
 case 'v' : * dst ++ = '\v' ;
 break ;
 case '0' : case '1' : case '2' : case '3' : case '4' : case '5' : case '6' : case '7' : val = c - '0' ;
 c = * src ++ ;
 if ( c >= '0' && c <= '7' ) {
 val = ( val << 3 ) | ( c - '0' ) ;
 c = * src ++ ;
 if ( c >= '0' && c <= '7' ) val = ( val << 3 ) | ( c - '0' ) ;
 else if ( src > oldsrc ) -- src ;
 }
 else if ( src > oldsrc ) -- src ;
 * dst ++ = ( char ) val ;
 break ;
 case 'x' : val = 'x' ;
 c = hextoint ( * src ++ ) ;
 if ( c >= 0 ) {
 val = c ;
 c = hextoint ( * src ++ ) ;
 if ( c >= 0 ) val = ( val << 4 ) + c ;
 else if ( src > oldsrc ) -- src ;
 }
 else if ( src > oldsrc ) -- src ;
 * dst ++ = ( char ) val ;
 break ;
 }
 }
 else if ( c == 8 || c == 263 ) {
 if ( dst > oldsrc ) dst -- ;
 }
 else * dst ++ = ( char ) c ;
 }
 strend : * dst = '\0' ;
 return ( dst - olddst ) ;
 }