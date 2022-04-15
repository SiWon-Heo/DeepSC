void * jbig2_alloc ( Jbig2Allocator * allocator , size_t size , size_t num ) {
 if ( num > 0 && size >= ( size_t ) - 0x100 / num ) return NULL ;
 return allocator -> alloc ( allocator , size * num ) ;
 }