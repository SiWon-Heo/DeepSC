int ssl_choose_client_version ( SSL * s , int version ) {
 const version_info * vent ;
 const version_info * table ;
 switch ( s -> method -> version ) {
 default : if ( version != s -> version ) return SSL_R_WRONG_SSL_VERSION ;
 return 0 ;
 case TLS_ANY_VERSION : table = tls_version_table ;
 break ;
 case DTLS_ANY_VERSION : table = dtls_version_table ;
 break ;
 }
 for ( vent = table ;
 vent -> version != 0 ;
 ++ vent ) {
 const SSL_METHOD * method ;
 int err ;
 if ( version != vent -> version ) continue ;
 if ( vent -> cmeth == NULL ) break ;
 method = vent -> cmeth ( ) ;
 err = ssl_method_error ( s , method ) ;
 if ( err != 0 ) return err ;
 s -> method = method ;
 s -> version = version ;
 return 0 ;
 }
 return SSL_R_UNSUPPORTED_PROTOCOL ;
 }