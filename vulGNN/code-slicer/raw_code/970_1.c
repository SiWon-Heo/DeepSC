int main ( int argc , char * * argv ) {
 gpg_error_t err ;
 if ( argc ) {
 argc -- ;
 argv ++ ;
 }
 if ( argc ) {
 unsigned char * buffer ;
 size_t n , buflen ;
 for ( ;
 argc ;
 argc -- , argv ++ ) {
 err = ksba_oid_from_str ( * argv , & buffer , & buflen ) ;
 if ( err ) {
 fprintf ( stderr , "can't convert `%s': %s\n" , * argv , gpg_strerror ( err ) ) ;
 return 1 ;
 }
 printf ( "%s ->" , * argv ) ;
 for ( n = 0 ;
 n < buflen ;
 n ++ ) printf ( " %02X" , buffer [ n ] ) ;
 putchar ( '\n' ) ;
 free ( buffer ) ;
 }
 }
 else {
 char * buffer ;
 size_t buflen ;
 char * result ;
 buffer = read_into_buffer ( stdin , & buflen ) ;
 result = ksba_oid_to_str ( buffer , buflen ) ;
 free ( buffer ) ;
 printf ( "%s\n" , result ? result : "[malloc failed]" ) ;
 free ( result ) ;
 }
 return 0 ;
 }