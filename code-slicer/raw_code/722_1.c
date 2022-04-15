gcry_error_t gcry_mpi_print ( enum gcry_mpi_format format , unsigned char * buffer , size_t buflen , size_t * nwritten , struct gcry_mpi * a ) {
 unsigned int nbits = mpi_get_nbits ( a ) ;
 size_t len ;
 size_t dummy_nwritten ;
 int negative ;
 if ( ! nwritten ) nwritten = & dummy_nwritten ;
 if ( a -> sign && _gcry_mpi_cmp_ui ( a , 0 ) ) negative = 1 ;
 else negative = 0 ;
 len = buflen ;
 * nwritten = 0 ;
 if ( format == GCRYMPI_FMT_STD ) {
 unsigned char * tmp ;
 int extra = 0 ;
 unsigned int n ;
 if ( negative ) return gcry_error ( GPG_ERR_INTERNAL ) ;
 tmp = _gcry_mpi_get_buffer ( a , & n , NULL ) ;
 if ( ! tmp ) return gpg_error_from_syserror ( ) ;
 if ( n && ( * tmp & 0x80 ) ) {
 n ++ ;
 extra = 1 ;
 }
 if ( buffer && n > len ) {
 gcry_free ( tmp ) ;
 return gcry_error ( GPG_ERR_TOO_SHORT ) ;
 }
 if ( buffer ) {
 unsigned char * s = buffer ;
 if ( extra ) * s ++ = 0 ;
 memcpy ( s , tmp , n - extra ) ;
 }
 gcry_free ( tmp ) ;
 * nwritten = n ;
 return 0 ;
 }
 else if ( format == GCRYMPI_FMT_USG ) {
 unsigned int n = ( nbits + 7 ) / 8 ;
 if ( buffer && n > len ) return gcry_error ( GPG_ERR_TOO_SHORT ) ;
 if ( buffer ) {
 unsigned char * tmp ;
 tmp = _gcry_mpi_get_buffer ( a , & n , NULL ) ;
 if ( ! tmp ) return gpg_error_from_syserror ( ) ;
 memcpy ( buffer , tmp , n ) ;
 gcry_free ( tmp ) ;
 }
 * nwritten = n ;
 return 0 ;
 }
 else if ( format == GCRYMPI_FMT_PGP ) {
 unsigned int n = ( nbits + 7 ) / 8 ;
 if ( negative ) return gcry_error ( GPG_ERR_INV_ARG ) ;
 if ( buffer && n + 2 > len ) return gcry_error ( GPG_ERR_TOO_SHORT ) ;
 if ( buffer ) {
 unsigned char * tmp ;
 unsigned char * s = buffer ;
 s [ 0 ] = nbits >> 8 ;
 s [ 1 ] = nbits ;
 tmp = _gcry_mpi_get_buffer ( a , & n , NULL ) ;
 if ( ! tmp ) return gpg_error_from_syserror ( ) ;
 memcpy ( s + 2 , tmp , n ) ;
 gcry_free ( tmp ) ;
 }
 * nwritten = n + 2 ;
 return 0 ;
 }
 else if ( format == GCRYMPI_FMT_SSH ) {
 unsigned char * tmp ;
 int extra = 0 ;
 unsigned int n ;
 if ( negative ) return gcry_error ( GPG_ERR_INTERNAL ) ;
 tmp = _gcry_mpi_get_buffer ( a , & n , NULL ) ;
 if ( ! tmp ) return gpg_error_from_syserror ( ) ;
 if ( n && ( * tmp & 0x80 ) ) {
 n ++ ;
 extra = 1 ;
 }
 if ( buffer && n + 4 > len ) {
 gcry_free ( tmp ) ;
 return gcry_error ( GPG_ERR_TOO_SHORT ) ;
 }
 if ( buffer ) {
 unsigned char * s = buffer ;
 * s ++ = n >> 24 ;
 * s ++ = n >> 16 ;
 * s ++ = n >> 8 ;
 * s ++ = n ;
 if ( extra ) * s ++ = 0 ;
 memcpy ( s , tmp , n - extra ) ;
 }
 gcry_free ( tmp ) ;
 * nwritten = 4 + n ;
 return 0 ;
 }
 else if ( format == GCRYMPI_FMT_HEX ) {
 unsigned char * tmp ;
 int i ;
 int extra = 0 ;
 unsigned int n = 0 ;
 tmp = _gcry_mpi_get_buffer ( a , & n , NULL ) ;
 if ( ! tmp ) return gpg_error_from_syserror ( ) ;
 if ( ! n || ( * tmp & 0x80 ) ) extra = 2 ;
 if ( buffer && 2 * n + extra + negative + 1 > len ) {
 gcry_free ( tmp ) ;
 return gcry_error ( GPG_ERR_TOO_SHORT ) ;
 }
 if ( buffer ) {
 unsigned char * s = buffer ;
 if ( negative ) * s ++ = '-' ;
 if ( extra ) {
 * s ++ = '0' ;
 * s ++ = '0' ;
 }
 for ( i = 0 ;
 i < n ;
 i ++ ) {
 unsigned int c = tmp [ i ] ;
 * s ++ = ( c >> 4 ) < 10 ? '0' + ( c >> 4 ) : 'A' + ( c >> 4 ) - 10 ;
 c &= 15 ;
 * s ++ = c < 10 ? '0' + c : 'A' + c - 10 ;
 }
 * s ++ = 0 ;
 * nwritten = s - buffer ;
 }
 else {
 * nwritten = 2 * n + extra + negative + 1 ;
 }
 gcry_free ( tmp ) ;
 return 0 ;
 }
 else return gcry_error ( GPG_ERR_INV_ARG ) ;
 }