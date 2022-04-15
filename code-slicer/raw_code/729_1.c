gcry_mpi_t gcry_sexp_nth_mpi ( gcry_sexp_t list , int number , int mpifmt ) {
 const char * s ;
 size_t n ;
 gcry_mpi_t a ;
 if ( ! mpifmt ) mpifmt = GCRYMPI_FMT_STD ;
 s = sexp_nth_data ( list , number , & n ) ;
 if ( ! s ) return NULL ;
 if ( gcry_mpi_scan ( & a , mpifmt , s , n , NULL ) ) return NULL ;
 return a ;
 }