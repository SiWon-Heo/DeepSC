static gcry_err_code_t sexp_elements_extract ( gcry_sexp_t key_sexp , const char * element_names , gcry_mpi_t * elements , const char * algo_name ) {
 gcry_err_code_t err = 0 ;
 int i , idx ;
 const char * name ;
 gcry_sexp_t list ;
 for ( name = element_names , idx = 0 ;
 * name && ! err ;
 name ++ , idx ++ ) {
 list = gcry_sexp_find_token ( key_sexp , name , 1 ) ;
 if ( ! list ) elements [ idx ] = NULL ;
 else {
 elements [ idx ] = gcry_sexp_nth_mpi ( list , 1 , GCRYMPI_FMT_USG ) ;
 gcry_sexp_release ( list ) ;
 if ( ! elements [ idx ] ) err = GPG_ERR_INV_OBJ ;
 }
 }
 if ( ! err ) {
 for ( name = element_names , i = 0 ;
 * name ;
 name ++ , i ++ ) if ( ! elements [ i ] ) break ;
 if ( * name ) {
 err = GPG_ERR_NO_OBJ ;
 if ( algo_name && ! strcmp ( algo_name , "RSA" ) && ! strcmp ( element_names , "nedpqu" ) ) {
 if ( elements [ 0 ] && elements [ 1 ] && elements [ 2 ] && ! elements [ 3 ] && ! elements [ 4 ] && ! elements [ 5 ] ) err = 0 ;
 }
 }
 }
 if ( err ) {
 for ( i = 0 ;
 i < idx ;
 i ++ ) if ( elements [ i ] ) mpi_free ( elements [ i ] ) ;
 }
 return err ;
 }