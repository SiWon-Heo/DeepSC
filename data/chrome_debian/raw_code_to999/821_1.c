static __inline__ __u32 __fswahw32 ( __u32 val ) {
 # ifdef __arch_swahw32 return __arch_swahw32 ( val ) ;
 # else return ___constant_swahw32 ( val ) ;
 # endif }