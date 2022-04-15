static int jbig2_find_changing_element_of_color ( const byte * line , uint32_t x , uint32_t w , int color ) {
 if ( line == 0 ) return w ;
 x = jbig2_find_changing_element ( line , x , w ) ;
 if ( x < w && getbit ( line , x ) != color ) x = jbig2_find_changing_element ( line , x , w ) ;
 return x ;
 }