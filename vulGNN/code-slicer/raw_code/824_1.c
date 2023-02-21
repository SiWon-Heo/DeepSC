static __inline__ __u32 __fswab32 ( __u32 val ) {
 # if defined ( __arch_swab32 ) return __arch_swab32 ( val ) ;
 # else return ___constant_swab32 ( val ) ;
 # endif }