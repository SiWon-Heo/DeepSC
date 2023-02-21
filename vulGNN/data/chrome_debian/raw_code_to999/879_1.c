void mpi_set_ui ( MPI w , unsigned long u ) {
 RESIZE_IF_NEEDED ( w , 1 ) ;
 w -> d [ 0 ] = u ;
 w -> nlimbs = u ? 1 : 0 ;
 w -> sign = 0 ;
 w -> nbits = 0 ;
 w -> flags = 0 ;
 }