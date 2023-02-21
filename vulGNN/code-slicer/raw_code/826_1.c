static __always_inline __u32 __be32_to_cpup ( const __be32 * p ) {
 return __swab32p ( ( __u32 * ) p ) ;
 }