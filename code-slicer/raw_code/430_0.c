void * jas_calloc ( size_t num_elements , size_t element_size ) {
 void * ptr ;
 size_t size ;
 if ( ! jas_safe_size_mul ( num_elements , element_size , & size ) ) {
 return 0 ;
 }
 if ( ! ( ptr = jas_malloc ( size ) ) ) {
 return 0 ;
 }
 memset ( ptr , 0 , size ) ;
 return ptr ;
 }