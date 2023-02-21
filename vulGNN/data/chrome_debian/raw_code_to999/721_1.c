byte * _gcry_mpi_get_secure_buffer ( gcry_mpi_t a , unsigned * nbytes , int * sign ) {
 return do_get_buffer ( a , nbytes , sign , 1 ) ;
 }