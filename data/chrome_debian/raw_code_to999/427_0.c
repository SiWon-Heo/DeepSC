void * jas_alloc3 ( size_t num_arrays , size_t array_size , size_t element_size ) {
 size_t size ;
 if ( ! jas_safe_size_mul ( array_size , element_size , & size ) || ! jas_safe_size_mul ( size , num_arrays , & size ) ) {
 return 0 ;
 }
 return jas_malloc ( size ) ;
 }