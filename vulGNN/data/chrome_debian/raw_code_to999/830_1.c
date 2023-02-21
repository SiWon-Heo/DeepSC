static __always_inline __u16 __be16_to_cpup ( const __be16 * p ) {
 return __swab16p ( ( __u16 * ) p ) ;
 }