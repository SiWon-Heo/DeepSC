byte * _gcry_mpi_get_buffer ( gcry_mpi_t a , unsigned int * nbytes , int * sign ) {
 return do_get_buffer ( a , nbytes , sign , 0 ) ;
 }