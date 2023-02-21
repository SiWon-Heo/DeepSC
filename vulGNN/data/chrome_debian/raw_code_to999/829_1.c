static __inline__ __u32 __arch_swab32 ( __u32 val ) {
 __asm__ ( "bswapl %0" : "=r" ( val ) : "0" ( val ) ) ;
 return val ;
 }