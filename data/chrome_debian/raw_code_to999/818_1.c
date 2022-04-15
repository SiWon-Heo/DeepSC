static __inline__ __u64 __fswab64 ( __u64 val ) {
 # if defined ( __arch_swab64 ) return __arch_swab64 ( val ) ;
 # elif defined ( __SWAB_64_THRU_32__ ) __u32 h = val >> 32 ;
 __u32 l = val & ( ( 1ULL << 32 ) - 1 ) ;
 return ( ( ( __u64 ) __fswab32 ( l ) ) << 32 ) | ( ( __u64 ) ( __fswab32 ( h ) ) ) ;
 # else return ___constant_swab64 ( val ) ;
 # endif }