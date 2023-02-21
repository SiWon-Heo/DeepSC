static void basic_ec_math_simplified ( void ) {
 gpg_error_t err ;
 gcry_ctx_t ctx ;
 gcry_mpi_point_t G , Q ;
 gcry_mpi_t d ;
 gcry_mpi_t x , y , z ;
 gcry_sexp_t sexp ;
 wherestr = "basic_ec_math_simplified" ;
 show ( "checking basic math functions for EC (variant)\n" ) ;
 d = hex2mpi ( "D4EF27E32F8AD8E2A1C6DDEBB1D235A69E3CEF9BCE90273D" ) ;
 Q = gcry_mpi_point_new ( 0 ) ;
 err = gcry_mpi_ec_new ( & ctx , NULL , "NIST P-192" ) ;
 if ( err ) die ( "gcry_mpi_ec_new failed: %s\n" , gpg_strerror ( err ) ) ;
 G = gcry_mpi_ec_get_point ( "g" , ctx , 1 ) ;
 if ( ! G ) die ( "gcry_mpi_ec_get_point(G) failed\n" ) ;
 gcry_mpi_ec_mul ( Q , d , G , ctx ) ;
 x = gcry_mpi_new ( 0 ) ;
 y = gcry_mpi_new ( 0 ) ;
 z = gcry_mpi_new ( 0 ) ;
 gcry_mpi_point_get ( x , y , z , Q ) ;
 if ( cmp_mpihex ( x , "222D9EC717C89D047E0898C9185B033CD11C0A981EE6DC66" ) || cmp_mpihex ( y , "605DE0A82D70D3E0F84A127D0739ED33D657DF0D054BFDE8" ) || cmp_mpihex ( z , "00B06B519071BC536999AC8F2D3934B3C1FC9EACCD0A31F88F" ) ) fail ( "computed public key does not match\n" ) ;
 if ( debug ) {
 print_mpi ( "Q.x" , x ) ;
 print_mpi ( "Q.y" , y ) ;
 print_mpi ( "Q.z" , z ) ;
 }
 if ( gcry_mpi_ec_get_affine ( x , y , Q , ctx ) ) fail ( "failed to get affine coordinates\n" ) ;
 if ( cmp_mpihex ( x , "008532093BA023F4D55C0424FA3AF9367E05F309DC34CDC3FE" ) || cmp_mpihex ( y , "00C13CA9E617C6C8487BFF6A726E3C4F277913D97117939966" ) ) fail ( "computed affine coordinates of public key do not match\n" ) ;
 if ( debug ) {
 print_mpi ( "q.x" , x ) ;
 print_mpi ( "q.y" , y ) ;
 }
 gcry_mpi_release ( z ) ;
 gcry_mpi_release ( y ) ;
 gcry_mpi_release ( x ) ;
 err = gcry_mpi_ec_set_point ( "g" , G , ctx ) ;
 if ( err ) die ( "gcry_mpi_ec_set_point(G) failed\n" ) ;
 err = gcry_mpi_ec_set_mpi ( "d" , d , ctx ) ;
 if ( err ) die ( "gcry_mpi_ec_set_mpi(d) failed\n" ) ;
 err = gcry_pubkey_get_sexp ( & sexp , 0 , ctx ) ;
 if ( err ) fail ( "gcry_pubkey_get_sexp(0) failed: %s\n" , gpg_strerror ( err ) ) ;
 else if ( verbose ) print_sexp ( "Result of gcry_pubkey_get_sexp (0):\n" , sexp ) ;
 gcry_sexp_release ( sexp ) ;
 err = gcry_pubkey_get_sexp ( & sexp , GCRY_PK_GET_PUBKEY , ctx ) ;
 if ( err ) fail ( "gcry_pubkey_get_sexp(GET_PUBKEY) failed: %s\n" , gpg_strerror ( err ) ) ;
 else if ( verbose ) print_sexp ( "Result of gcry_pubkey_get_sexp (GET_PUBKEY):\n" , sexp ) ;
 gcry_sexp_release ( sexp ) ;
 err = gcry_mpi_ec_set_mpi ( "d" , NULL , ctx ) ;
 if ( err ) die ( "gcry_mpi_ec_set_mpi(d=NULL) failed\n" ) ;
 err = gcry_pubkey_get_sexp ( & sexp , 0 , ctx ) ;
 if ( err ) fail ( "gcry_pubkey_get_sexp(0 w/o d) failed: %s\n" , gpg_strerror ( err ) ) ;
 else if ( verbose ) print_sexp ( "Result of gcry_pubkey_get_sexp (0 w/o d):\n" , sexp ) ;
 gcry_sexp_release ( sexp ) ;
 err = gcry_pubkey_get_sexp ( & sexp , GCRY_PK_GET_SECKEY , ctx ) ;
 if ( gpg_err_code ( err ) != GPG_ERR_NO_SECKEY ) fail ( "gcry_pubkey_get_sexp(GET_SECKEY) returned wrong error: %s\n" , gpg_strerror ( err ) ) ;
 gcry_sexp_release ( sexp ) ;
 err = gcry_mpi_ec_set_point ( "q" , NULL , ctx ) ;
 if ( err ) die ( "gcry_mpi_ec_set_point(q=NULL) failed\n" ) ;
 err = gcry_pubkey_get_sexp ( & sexp , 0 , ctx ) ;
 if ( gpg_err_code ( err ) != GPG_ERR_BAD_CRYPT_CTX ) fail ( "gcry_pubkey_get_sexp(0 w/o Q,d) returned wrong error: %s\n" , gpg_strerror ( err ) ) ;
 gcry_sexp_release ( sexp ) ;
 gcry_mpi_point_release ( Q ) ;
 gcry_mpi_release ( d ) ;
 gcry_mpi_point_release ( G ) ;
 gcry_ctx_release ( ctx ) ;
 }