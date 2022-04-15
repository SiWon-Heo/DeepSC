int ssl_check_version_downgrade ( SSL * s ) {
 const version_info * vent ;
 const version_info * table ;
 if ( s -> version == s -> ctx -> method -> version ) return 1 ;
 if ( s -> ctx -> method -> version == TLS_method ( ) -> version ) table = tls_version_table ;
 else if ( s -> ctx -> method -> version == DTLS_method ( ) -> version ) table = dtls_version_table ;
 else {
 return 0 ;
 }
 for ( vent = table ;
 vent -> version != 0 ;
 ++ vent ) {
 if ( vent -> smeth != NULL && ssl_method_error ( s , vent -> smeth ( ) ) == 0 ) return s -> version == vent -> version ;
 }
 return 0 ;
 }