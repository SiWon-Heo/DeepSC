void * jas_alloc2 ( size_t num_elements , size_t element_size ) {
 size_t size ;
 if ( ! jas_safe_size_mul ( num_elements , element_size , & size ) ) {
 return 0 ;
 }
 return jas_malloc ( size ) ;
 }