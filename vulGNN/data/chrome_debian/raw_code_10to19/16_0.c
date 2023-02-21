static long dprintf_DollarString ( char * input , char * * end ) {
 int number = 0 ;
 while ( ISDIGIT ( * input ) ) {
 number *= 10 ;
 number += * input - '0' ;
 input ++ ;
 }
 if ( number && ( '$' == * input ++ ) ) {
 * end = input ;
 return number ;
 }
 return 0 ;
 }