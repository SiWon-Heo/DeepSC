static __inline__ __u32 __fswahb32 ( __u32 val ) {
 # ifdef __arch_swahb32 return __arch_swahb32 ( val ) ;
 # else return ___constant_swahb32 ( val ) ;
 # endif }