char * xmlrpc_normalizeBuffer ( const char * buf ) {
 char * newbuf ;
 int i , len , j = 0 ;
 len = strlen ( buf ) ;
 newbuf = ( char * ) smalloc ( sizeof ( char ) * len + 1 ) ;
 for ( i = 0 ;
 i < len ;
 i ++ ) {
 switch ( buf [ i ] ) {
 case 1 : break ;
 case 2 : break ;
 case 3 : if ( isdigit ( ( unsigned char ) buf [ i + 1 ] ) ) {
 i ++ ;
 if ( isdigit ( ( unsigned char ) buf [ i + 1 ] ) ) {
 i ++ ;
 }
 if ( buf [ i + 1 ] == ',' ) {
 i ++ ;
 if ( isdigit ( ( unsigned char ) buf [ i + 1 ] ) ) {
 i ++ ;
 }
 if ( isdigit ( ( unsigned char ) buf [ i + 1 ] ) ) {
 i ++ ;
 }
 }
 }
 break ;
 case 9 : break ;
 case 10 : break ;
 case 13 : break ;
 case 22 : break ;
 case 31 : break ;
 default : if ( buf [ i ] > 31 ) {
 newbuf [ j ] = buf [ i ] ;
 j ++ ;
 }
 }
 }
 newbuf [ j ] = 0 ;
 return ( newbuf ) ;
 }