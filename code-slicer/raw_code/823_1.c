static __always_inline __u64 __be64_to_cpup ( const __be64 * p ) {
 return __swab64p ( ( __u64 * ) p ) ;
 }