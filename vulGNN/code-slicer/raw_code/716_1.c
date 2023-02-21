gpg_err_code_t _gcry_ecc_fill_in_curve ( unsigned int nbits , const char * name , elliptic_curve_t * curve , unsigned int * r_nbits ) {
 int idx , aliasno ;
 const char * resname = NULL ;
 if ( name ) {
 for ( idx = 0 ;
 domain_parms [ idx ] . desc ;
 idx ++ ) if ( ! strcmp ( name , domain_parms [ idx ] . desc ) ) {
 resname = domain_parms [ idx ] . desc ;
 break ;
 }
 if ( ! domain_parms [ idx ] . desc ) {
 for ( aliasno = 0 ;
 curve_aliases [ aliasno ] . name ;
 aliasno ++ ) if ( ! strcmp ( name , curve_aliases [ aliasno ] . other ) ) break ;
 if ( curve_aliases [ aliasno ] . name ) {
 for ( idx = 0 ;
 domain_parms [ idx ] . desc ;
 idx ++ ) if ( ! strcmp ( curve_aliases [ aliasno ] . name , domain_parms [ idx ] . desc ) ) {
 resname = domain_parms [ idx ] . desc ;
 break ;
 }
 }
 }
 }
 else {
 for ( idx = 0 ;
 domain_parms [ idx ] . desc ;
 idx ++ ) if ( nbits == domain_parms [ idx ] . nbits ) break ;
 }
 if ( ! domain_parms [ idx ] . desc ) return GPG_ERR_UNKNOWN_CURVE ;
 if ( fips_mode ( ) && ! domain_parms [ idx ] . fips ) return GPG_ERR_NOT_SUPPORTED ;
 switch ( domain_parms [ idx ] . model ) {
 case MPI_EC_WEIERSTRASS : case MPI_EC_TWISTEDEDWARDS : break ;
 case MPI_EC_MONTGOMERY : return GPG_ERR_NOT_SUPPORTED ;
 default : return GPG_ERR_BUG ;
 }
 if ( r_nbits ) * r_nbits = domain_parms [ idx ] . nbits ;
 curve -> model = domain_parms [ idx ] . model ;
 curve -> p = scanval ( domain_parms [ idx ] . p ) ;
 curve -> a = scanval ( domain_parms [ idx ] . a ) ;
 curve -> b = scanval ( domain_parms [ idx ] . b ) ;
 curve -> n = scanval ( domain_parms [ idx ] . n ) ;
 curve -> G . x = scanval ( domain_parms [ idx ] . g_x ) ;
 curve -> G . y = scanval ( domain_parms [ idx ] . g_y ) ;
 curve -> G . z = mpi_alloc_set_ui ( 1 ) ;
 curve -> name = resname ;
 return 0 ;
 }