static gcry_err_code_t sexp_to_sig ( gcry_sexp_t sexp , gcry_mpi_t * * retarray , gcry_module_t * retalgo ) {
 gcry_err_code_t err = 0 ;
 gcry_sexp_t list , l2 ;
 char * name ;
 const char * elems ;
 gcry_mpi_t * array ;
 gcry_module_t module ;
 gcry_pk_spec_t * pubkey ;
 list = gcry_sexp_find_token ( sexp , "sig-val" , 0 ) ;
 if ( ! list ) return GPG_ERR_INV_OBJ ;
 l2 = gcry_sexp_nth ( list , 1 ) ;
 if ( ! l2 ) {
 gcry_sexp_release ( list ) ;
 return GPG_ERR_NO_OBJ ;
 }
 name = _gcry_sexp_nth_string ( l2 , 0 ) ;
 if ( ! name ) {
 gcry_sexp_release ( list ) ;
 gcry_sexp_release ( l2 ) ;
 return GPG_ERR_INV_OBJ ;
 }
 else if ( ! strcmp ( name , "flags" ) ) {
 gcry_free ( name ) ;
 gcry_sexp_release ( l2 ) ;
 l2 = gcry_sexp_nth ( list , 2 ) ;
 if ( ! l2 ) {
 gcry_sexp_release ( list ) ;
 return GPG_ERR_INV_OBJ ;
 }
 name = _gcry_sexp_nth_string ( l2 , 0 ) ;
 }
 ath_mutex_lock ( & pubkeys_registered_lock ) ;
 module = gcry_pk_lookup_name ( name ) ;
 ath_mutex_unlock ( & pubkeys_registered_lock ) ;
 gcry_free ( name ) ;
 name = NULL ;
 if ( ! module ) {
 gcry_sexp_release ( l2 ) ;
 gcry_sexp_release ( list ) ;
 return GPG_ERR_PUBKEY_ALGO ;
 }
 else pubkey = ( gcry_pk_spec_t * ) module -> spec ;
 elems = pubkey -> elements_sig ;
 array = gcry_calloc ( strlen ( elems ) + 1 , sizeof * array ) ;
 if ( ! array ) err = gpg_err_code_from_syserror ( ) ;
 if ( ! err ) err = sexp_elements_extract ( list , elems , array , NULL ) ;
 gcry_sexp_release ( l2 ) ;
 gcry_sexp_release ( list ) ;
 if ( err ) {
 ath_mutex_lock ( & pubkeys_registered_lock ) ;
 _gcry_module_release ( module ) ;
 ath_mutex_unlock ( & pubkeys_registered_lock ) ;
 gcry_free ( array ) ;
 }
 else {
 * retarray = array ;
 * retalgo = module ;
 }
 return err ;
 }