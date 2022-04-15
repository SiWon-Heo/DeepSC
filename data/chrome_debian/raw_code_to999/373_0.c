int ssl_get_client_min_max_version ( const SSL * s , int * min_version , int * max_version ) {
 int version ;
 int hole ;
 const SSL_METHOD * single = NULL ;
 const SSL_METHOD * method ;
 const version_info * table ;
 const version_info * vent ;
 switch ( s -> method -> version ) {
 default : * min_version = * max_version = s -> version ;
 return 0 ;
 case TLS_ANY_VERSION : table = tls_version_table ;
 break ;
 case DTLS_ANY_VERSION : table = dtls_version_table ;
 break ;
 }
 * min_version = version = 0 ;
 hole = 1 ;
 for ( vent = table ;
 vent -> version != 0 ;
 ++ vent ) {
 if ( vent -> cmeth == NULL ) {
 hole = 1 ;
 continue ;
 }
 method = vent -> cmeth ( ) ;
 if ( ssl_method_error ( s , method ) != 0 ) {
 hole = 1 ;
 }
 else if ( ! hole ) {
 single = NULL ;
 * min_version = method -> version ;
 }
 else {
 version = ( single = method ) -> version ;
 * min_version = version ;
 hole = 0 ;
 }
 }
 * max_version = version ;
 if ( version == 0 ) return SSL_R_NO_PROTOCOLS_AVAILABLE ;
 return 0 ;
 }