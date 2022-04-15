static int alloc_addbyter ( int output , FILE * data ) {
 struct asprintf * infop = ( struct asprintf * ) data ;
 unsigned char outc = ( unsigned char ) output ;
 if ( ! infop -> buffer ) {
 infop -> buffer = malloc ( 32 ) ;
 if ( ! infop -> buffer ) {
 infop -> fail = 1 ;
 return - 1 ;
 }
 infop -> alloc = 32 ;
 infop -> len = 0 ;
 }
 else if ( infop -> len + 1 >= infop -> alloc ) {
 char * newptr = NULL ;
 size_t newsize = infop -> alloc * 2 ;
 if ( newsize > infop -> alloc ) newptr = realloc ( infop -> buffer , newsize ) ;
 if ( ! newptr ) {
 infop -> fail = 1 ;
 return - 1 ;
 }
 infop -> buffer = newptr ;
 infop -> alloc = newsize ;
 }
 infop -> buffer [ infop -> len ] = outc ;
 infop -> len ++ ;
 return outc ;
 }