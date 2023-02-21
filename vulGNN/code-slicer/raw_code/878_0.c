static int set_SUSP_CE ( unsigned char * p , int location , int offset , int size ) {
 unsigned char * bp = p - 1 ;
 bp [ 1 ] = 'C' ;
 bp [ 2 ] = 'E' ;
 bp [ 3 ] = RR_CE_SIZE ;
 bp [ 4 ] = 1 ;
 set_num_733 ( bp + 5 , location ) ;
 set_num_733 ( bp + 13 , offset ) ;
 set_num_733 ( bp + 21 , size ) ;
 return ( RR_CE_SIZE ) ;
 }