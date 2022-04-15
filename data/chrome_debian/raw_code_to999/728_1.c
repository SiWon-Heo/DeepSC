static gcry_err_code_t sexp_data_to_mpi ( gcry_sexp_t input , gcry_mpi_t * ret_mpi , struct pk_encoding_ctx * ctx ) {
 gcry_err_code_t rc = 0 ;
 gcry_sexp_t ldata , lhash , lvalue ;
 int i ;
 size_t n ;
 const char * s ;
 int unknown_flag = 0 ;
 int parsed_flags = 0 ;
 int explicit_raw = 0 ;
 * ret_mpi = NULL ;
 ldata = gcry_sexp_find_token ( input , "data" , 0 ) ;
 if ( ! ldata ) {
 * ret_mpi = gcry_sexp_nth_mpi ( input , 0 , 0 ) ;
 return * ret_mpi ? GPG_ERR_NO_ERROR : GPG_ERR_INV_OBJ ;
 }
 {
 gcry_sexp_t lflags = gcry_sexp_find_token ( ldata , "flags" , 0 ) ;
 if ( lflags ) {
 for ( i = gcry_sexp_length ( lflags ) - 1 ;
 i > 0 ;
 i -- ) {
 s = gcry_sexp_nth_data ( lflags , i , & n ) ;
 if ( ! s ) ;
 else if ( n == 7 && ! memcmp ( s , "rfc6979" , 7 ) ) parsed_flags |= PUBKEY_FLAG_RFC6979 ;
 else if ( n == 5 && ! memcmp ( s , "eddsa" , 5 ) ) {
 ctx -> encoding = PUBKEY_ENC_RAW ;
 parsed_flags |= PUBKEY_FLAG_EDDSA ;
 }
 else if ( n == 3 && ! memcmp ( s , "raw" , 3 ) && ctx -> encoding == PUBKEY_ENC_UNKNOWN ) {
 ctx -> encoding = PUBKEY_ENC_RAW ;
 explicit_raw = 1 ;
 }
 else if ( n == 5 && ! memcmp ( s , "pkcs1" , 5 ) && ctx -> encoding == PUBKEY_ENC_UNKNOWN ) ctx -> encoding = PUBKEY_ENC_PKCS1 ;
 else if ( n == 4 && ! memcmp ( s , "oaep" , 4 ) && ctx -> encoding == PUBKEY_ENC_UNKNOWN ) ctx -> encoding = PUBKEY_ENC_OAEP ;
 else if ( n == 3 && ! memcmp ( s , "pss" , 3 ) && ctx -> encoding == PUBKEY_ENC_UNKNOWN ) ctx -> encoding = PUBKEY_ENC_PSS ;
 else if ( n == 11 && ! memcmp ( s , "no-blinding" , 11 ) ) parsed_flags |= PUBKEY_FLAG_NO_BLINDING ;
 else unknown_flag = 1 ;
 }
 gcry_sexp_release ( lflags ) ;
 }
 }
 if ( ctx -> encoding == PUBKEY_ENC_UNKNOWN ) ctx -> encoding = PUBKEY_ENC_RAW ;
 lhash = gcry_sexp_find_token ( ldata , "hash" , 0 ) ;
 lvalue = lhash ? NULL : gcry_sexp_find_token ( ldata , "value" , 0 ) ;
 if ( ! ( ! lhash ^ ! lvalue ) ) rc = GPG_ERR_INV_OBJ ;
 else if ( unknown_flag ) rc = GPG_ERR_INV_FLAG ;
 else if ( ctx -> encoding == PUBKEY_ENC_RAW && ( parsed_flags & PUBKEY_FLAG_EDDSA ) ) {
 gcry_sexp_t list ;
 void * value ;
 size_t valuelen ;
 if ( ! lvalue ) {
 rc = GPG_ERR_INV_OBJ ;
 goto leave ;
 }
 list = gcry_sexp_find_token ( ldata , "hash-algo" , 0 ) ;
 if ( list ) {
 s = gcry_sexp_nth_data ( list , 1 , & n ) ;
 if ( ! s ) rc = GPG_ERR_NO_OBJ ;
 else {
 ctx -> hash_algo = get_hash_algo ( s , n ) ;
 if ( ! ctx -> hash_algo ) rc = GPG_ERR_DIGEST_ALGO ;
 }
 gcry_sexp_release ( list ) ;
 }
 else rc = GPG_ERR_INV_OBJ ;
 if ( rc ) goto leave ;
 value = gcry_sexp_nth_buffer ( lvalue , 1 , & valuelen ) ;
 if ( ! value ) rc = GPG_ERR_INV_OBJ ;
 else if ( ( valuelen * 8 ) < valuelen ) {
 gcry_free ( value ) ;
 rc = GPG_ERR_TOO_LARGE ;
 }
 if ( rc ) goto leave ;
 * ret_mpi = gcry_mpi_set_opaque ( NULL , value , valuelen * 8 ) ;
 }
 else if ( ctx -> encoding == PUBKEY_ENC_RAW && lhash && ( explicit_raw || ( parsed_flags & PUBKEY_FLAG_RFC6979 ) ) ) {
 if ( gcry_sexp_length ( lhash ) != 3 ) rc = GPG_ERR_INV_OBJ ;
 else if ( ! ( s = gcry_sexp_nth_data ( lhash , 1 , & n ) ) || ! n ) rc = GPG_ERR_INV_OBJ ;
 else {
 void * value ;
 size_t valuelen ;
 ctx -> hash_algo = get_hash_algo ( s , n ) ;
 if ( ! ctx -> hash_algo ) rc = GPG_ERR_DIGEST_ALGO ;
 else if ( ! ( value = gcry_sexp_nth_buffer ( lhash , 2 , & valuelen ) ) ) rc = GPG_ERR_INV_OBJ ;
 else if ( ( valuelen * 8 ) < valuelen ) {
 gcry_free ( value ) ;
 rc = GPG_ERR_TOO_LARGE ;
 }
 else * ret_mpi = gcry_mpi_set_opaque ( NULL , value , valuelen * 8 ) ;
 }
 }
 else if ( ctx -> encoding == PUBKEY_ENC_RAW && lvalue ) {
 if ( parsed_flags & PUBKEY_FLAG_RFC6979 ) {
 rc = GPG_ERR_CONFLICT ;
 goto leave ;
 }
 * ret_mpi = gcry_sexp_nth_mpi ( lvalue , 1 , GCRYMPI_FMT_USG ) ;
 if ( ! * ret_mpi ) rc = GPG_ERR_INV_OBJ ;
 }
 else if ( ctx -> encoding == PUBKEY_ENC_PKCS1 && lvalue && ctx -> op == PUBKEY_OP_ENCRYPT ) {
 const void * value ;
 size_t valuelen ;
 gcry_sexp_t list ;
 void * random_override = NULL ;
 size_t random_override_len = 0 ;
 if ( ! ( value = gcry_sexp_nth_data ( lvalue , 1 , & valuelen ) ) || ! valuelen ) rc = GPG_ERR_INV_OBJ ;
 else {
 list = gcry_sexp_find_token ( ldata , "random-override" , 0 ) ;
 if ( list ) {
 s = gcry_sexp_nth_data ( list , 1 , & n ) ;
 if ( ! s ) rc = GPG_ERR_NO_OBJ ;
 else if ( n > 0 ) {
 random_override = gcry_malloc ( n ) ;
 if ( ! random_override ) rc = gpg_err_code_from_syserror ( ) ;
 else {
 memcpy ( random_override , s , n ) ;
 random_override_len = n ;
 }
 }
 gcry_sexp_release ( list ) ;
 if ( rc ) goto leave ;
 }
 rc = pkcs1_encode_for_encryption ( ret_mpi , ctx -> nbits , value , valuelen , random_override , random_override_len ) ;
 gcry_free ( random_override ) ;
 }
 }
 else if ( ctx -> encoding == PUBKEY_ENC_PKCS1 && lhash && ( ctx -> op == PUBKEY_OP_SIGN || ctx -> op == PUBKEY_OP_VERIFY ) ) {
 if ( gcry_sexp_length ( lhash ) != 3 ) rc = GPG_ERR_INV_OBJ ;
 else if ( ! ( s = gcry_sexp_nth_data ( lhash , 1 , & n ) ) || ! n ) rc = GPG_ERR_INV_OBJ ;
 else {
 const void * value ;
 size_t valuelen ;
 ctx -> hash_algo = get_hash_algo ( s , n ) ;
 if ( ! ctx -> hash_algo ) rc = GPG_ERR_DIGEST_ALGO ;
 else if ( ! ( value = gcry_sexp_nth_data ( lhash , 2 , & valuelen ) ) || ! valuelen ) rc = GPG_ERR_INV_OBJ ;
 else rc = pkcs1_encode_for_signature ( ret_mpi , ctx -> nbits , value , valuelen , ctx -> hash_algo ) ;
 }
 }
 else if ( ctx -> encoding == PUBKEY_ENC_OAEP && lvalue && ctx -> op == PUBKEY_OP_ENCRYPT ) {
 const void * value ;
 size_t valuelen ;
 if ( ! ( value = gcry_sexp_nth_data ( lvalue , 1 , & valuelen ) ) || ! valuelen ) rc = GPG_ERR_INV_OBJ ;
 else {
 gcry_sexp_t list ;
 void * random_override = NULL ;
 size_t random_override_len = 0 ;
 list = gcry_sexp_find_token ( ldata , "hash-algo" , 0 ) ;
 if ( list ) {
 s = gcry_sexp_nth_data ( list , 1 , & n ) ;
 if ( ! s ) rc = GPG_ERR_NO_OBJ ;
 else {
 ctx -> hash_algo = get_hash_algo ( s , n ) ;
 if ( ! ctx -> hash_algo ) rc = GPG_ERR_DIGEST_ALGO ;
 }
 gcry_sexp_release ( list ) ;
 if ( rc ) goto leave ;
 }
 list = gcry_sexp_find_token ( ldata , "label" , 0 ) ;
 if ( list ) {
 s = gcry_sexp_nth_data ( list , 1 , & n ) ;
 if ( ! s ) rc = GPG_ERR_NO_OBJ ;
 else if ( n > 0 ) {
 ctx -> label = gcry_malloc ( n ) ;
 if ( ! ctx -> label ) rc = gpg_err_code_from_syserror ( ) ;
 else {
 memcpy ( ctx -> label , s , n ) ;
 ctx -> labellen = n ;
 }
 }
 gcry_sexp_release ( list ) ;
 if ( rc ) goto leave ;
 }
 list = gcry_sexp_find_token ( ldata , "random-override" , 0 ) ;
 if ( list ) {
 s = gcry_sexp_nth_data ( list , 1 , & n ) ;
 if ( ! s ) rc = GPG_ERR_NO_OBJ ;
 else if ( n > 0 ) {
 random_override = gcry_malloc ( n ) ;
 if ( ! random_override ) rc = gpg_err_code_from_syserror ( ) ;
 else {
 memcpy ( random_override , s , n ) ;
 random_override_len = n ;
 }
 }
 gcry_sexp_release ( list ) ;
 if ( rc ) goto leave ;
 }
 rc = oaep_encode ( ret_mpi , ctx -> nbits , ctx -> hash_algo , value , valuelen , ctx -> label , ctx -> labellen , random_override , random_override_len ) ;
 gcry_free ( random_override ) ;
 }
 }
 else if ( ctx -> encoding == PUBKEY_ENC_PSS && lhash && ctx -> op == PUBKEY_OP_SIGN ) {
 if ( gcry_sexp_length ( lhash ) != 3 ) rc = GPG_ERR_INV_OBJ ;
 else if ( ! ( s = gcry_sexp_nth_data ( lhash , 1 , & n ) ) || ! n ) rc = GPG_ERR_INV_OBJ ;
 else {
 const void * value ;
 size_t valuelen ;
 void * random_override = NULL ;
 size_t random_override_len = 0 ;
 ctx -> hash_algo = get_hash_algo ( s , n ) ;
 if ( ! ctx -> hash_algo ) rc = GPG_ERR_DIGEST_ALGO ;
 else if ( ! ( value = gcry_sexp_nth_data ( lhash , 2 , & valuelen ) ) || ! valuelen ) rc = GPG_ERR_INV_OBJ ;
 else {
 gcry_sexp_t list ;
 list = gcry_sexp_find_token ( ldata , "salt-length" , 0 ) ;
 if ( list ) {
 s = gcry_sexp_nth_data ( list , 1 , & n ) ;
 if ( ! s ) {
 rc = GPG_ERR_NO_OBJ ;
 goto leave ;
 }
 ctx -> saltlen = ( unsigned int ) strtoul ( s , NULL , 10 ) ;
 gcry_sexp_release ( list ) ;
 }
 list = gcry_sexp_find_token ( ldata , "random-override" , 0 ) ;
 if ( list ) {
 s = gcry_sexp_nth_data ( list , 1 , & n ) ;
 if ( ! s ) rc = GPG_ERR_NO_OBJ ;
 else if ( n > 0 ) {
 random_override = gcry_malloc ( n ) ;
 if ( ! random_override ) rc = gpg_err_code_from_syserror ( ) ;
 else {
 memcpy ( random_override , s , n ) ;
 random_override_len = n ;
 }
 }
 gcry_sexp_release ( list ) ;
 if ( rc ) goto leave ;
 }
 rc = pss_encode ( ret_mpi , ctx -> nbits - 1 , ctx -> hash_algo , value , valuelen , ctx -> saltlen , random_override , random_override_len ) ;
 gcry_free ( random_override ) ;
 }
 }
 }
 else if ( ctx -> encoding == PUBKEY_ENC_PSS && lhash && ctx -> op == PUBKEY_OP_VERIFY ) {
 if ( gcry_sexp_length ( lhash ) != 3 ) rc = GPG_ERR_INV_OBJ ;
 else if ( ! ( s = gcry_sexp_nth_data ( lhash , 1 , & n ) ) || ! n ) rc = GPG_ERR_INV_OBJ ;
 else {
 ctx -> hash_algo = get_hash_algo ( s , n ) ;
 if ( ! ctx -> hash_algo ) rc = GPG_ERR_DIGEST_ALGO ;
 else {
 * ret_mpi = gcry_sexp_nth_mpi ( lhash , 2 , GCRYMPI_FMT_USG ) ;
 if ( ! * ret_mpi ) rc = GPG_ERR_INV_OBJ ;
 ctx -> verify_cmp = pss_verify_cmp ;
 ctx -> verify_arg = * ret_mpi ;
 }
 }
 }
 else rc = GPG_ERR_CONFLICT ;
 leave : gcry_sexp_release ( ldata ) ;
 gcry_sexp_release ( lhash ) ;
 gcry_sexp_release ( lvalue ) ;
 if ( ! rc ) ctx -> flags = parsed_flags ;
 else {
 gcry_free ( ctx -> label ) ;
 ctx -> label = NULL ;
 }
 return rc ;
 }