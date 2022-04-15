static __inline__ __u16 __fswab16 ( __u16 val ) {
 # if defined ( __arch_swab16 ) return __arch_swab16 ( val ) ;
 # else return ___constant_swab16 ( val ) ;
 # endif }