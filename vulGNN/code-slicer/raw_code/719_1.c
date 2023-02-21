static unsigned char * do_get_buffer ( gcry_mpi_t a , unsigned int * nbytes , int * sign , int force_secure ) {
 unsigned char * p , * buffer ;
 mpi_limb_t alimb ;
 int i ;
 size_t n ;
 if ( sign ) * sign = a -> sign ;
 * nbytes = a -> nlimbs * BYTES_PER_MPI_LIMB ;
 n = * nbytes ? * nbytes : 1 ;
 p = buffer = ( force_secure || mpi_is_secure ( a ) ) ? gcry_malloc_secure ( n ) : gcry_malloc ( n ) ;
 if ( ! buffer ) return NULL ;
 for ( i = a -> nlimbs - 1 ;
 i >= 0 ;
 i -- ) {
 alimb = a -> d [ i ] ;
 # if BYTES_PER_MPI_LIMB == 4 * p ++ = alimb >> 24 ;
 * p ++ = alimb >> 16 ;
 * p ++ = alimb >> 8 ;
 * p ++ = alimb ;
 # elif BYTES_PER_MPI_LIMB == 8 * p ++ = alimb >> 56 ;
 * p ++ = alimb >> 48 ;
 * p ++ = alimb >> 40 ;
 * p ++ = alimb >> 32 ;
 * p ++ = alimb >> 24 ;
 * p ++ = alimb >> 16 ;
 * p ++ = alimb >> 8 ;
 * p ++ = alimb ;
 # else # error please implement for this limb size . # endif }
 for ( p = buffer ;
 * nbytes && ! * p ;
 p ++ , -- * nbytes ) ;
 if ( p != buffer ) memmove ( buffer , p , * nbytes ) ;
 return buffer ;
 }