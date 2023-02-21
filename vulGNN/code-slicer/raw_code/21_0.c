static int addbyter ( int output , FILE * data ) {
 struct nsprintf * infop = ( struct nsprintf * ) data ;
 unsigned char outc = ( unsigned char ) output ;
 if ( infop -> length < infop -> max ) {
 infop -> buffer [ 0 ] = outc ;
 infop -> buffer ++ ;
 infop -> length ++ ;
 return outc ;
 }
 return - 1 ;
 }