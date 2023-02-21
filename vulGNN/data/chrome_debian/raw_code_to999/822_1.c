static __inline__ __u64 __arch_swab64 ( __u64 val ) {
 # ifdef __i386__ union {
 struct {
 __u32 a ;
 __u32 b ;
 }
 s ;
 __u64 u ;
 }
 v ;
 v . u = val ;
 __asm__ ( "bswapl %0 ;
 bswapl %1 ;
 xchgl %0,%1" : "=r" ( v . s . a ) , "=r" ( v . s . b ) : "0" ( v . s . a ) , "1" ( v . s . b ) ) ;
 return v . u ;
 # else __asm__ ( "bswapq %0" : "=r" ( val ) : "0" ( val ) ) ;
 return val ;
 # endif }