int ssl_choose_server_version ( SSL * s ) {
 int server_version = s -> method -> version ;
 int client_version = s -> client_version ;
 const version_info * vent ;
 const version_info * table ;
 int disabled = 0 ;
 switch ( server_version ) {
 default : if ( version_cmp ( s , client_version , s -> version ) < 0 ) return SSL_R_WRONG_SSL_VERSION ;
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
 if ( vent -> smeth == NULL || version_cmp ( s , client_version , vent -> version ) < 0 ) continue ;
 method = vent -> smeth ( ) ;
 if ( ssl_method_error ( s , method ) == 0 ) {
 s -> version = vent -> version ;
 s -> method = method ;
 return 0 ;
 }
 disabled = 1 ;
 }
 return disabled ? SSL_R_UNSUPPORTED_PROTOCOL : SSL_R_VERSION_TOO_LOW ;
 }