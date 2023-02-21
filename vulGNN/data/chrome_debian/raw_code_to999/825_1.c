static __always_inline __be32 __cpu_to_be32p ( const __u32 * p ) {
 return ( __be32 ) __swab32p ( p ) ;
 }