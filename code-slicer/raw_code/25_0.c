void * jbig2_realloc ( Jbig2Allocator * allocator , void * p , size_t size , size_t num ) {
 if ( num > 0 && size >= ( size_t ) - 0x100 / num ) return NULL ;
 return allocator -> realloc ( allocator , p , size * num ) ;
 }