gcry_error_t gcry_pk_verify ( gcry_sexp_t s_sig , gcry_sexp_t s_hash , gcry_sexp_t s_pkey ) {
 gcry_module_t module_key = NULL , module_sig = NULL ;
 gcry_mpi_t * pkey = NULL , hash = NULL , * sig = NULL ;
 struct pk_encoding_ctx ctx ;
 gcry_err_code_t rc ;
 REGISTER_DEFAULT_PUBKEYS ;
 rc = sexp_to_key ( s_pkey , 0 , GCRY_PK_USAGE_SIGN , NULL , & pkey , & module_key , NULL ) ;
 if ( rc ) goto leave ;
 rc = sexp_to_sig ( s_sig , & sig , & module_sig ) ;
 if ( rc ) goto leave ;
 if ( module_key -> mod_id != module_sig -> mod_id ) {
 rc = GPG_ERR_CONFLICT ;
 goto leave ;
 }
 init_encoding_ctx ( & ctx , PUBKEY_OP_VERIFY , gcry_pk_get_nbits ( s_pkey ) ) ;
 rc = sexp_data_to_mpi ( s_hash , & hash , & ctx ) ;
 if ( rc ) goto leave ;
 rc = pubkey_verify ( module_key -> mod_id , hash , sig , pkey , & ctx ) ;
 leave : if ( pkey ) {
 release_mpi_array ( pkey ) ;
 gcry_free ( pkey ) ;
 }
 if ( sig ) {
 release_mpi_array ( sig ) ;
 gcry_free ( sig ) ;
 }
 if ( hash ) mpi_free ( hash ) ;
 if ( module_key || module_sig ) {
 ath_mutex_lock ( & pubkeys_registered_lock ) ;
 if ( module_key ) _gcry_module_release ( module_key ) ;
 if ( module_sig ) _gcry_module_release ( module_sig ) ;
 ath_mutex_unlock ( & pubkeys_registered_lock ) ;
 }
 return gcry_error ( rc ) ;
 }