int ssl_version_supported ( const SSL * s , int version ) {
 const version_info * vent ;
 const version_info * table ;
 switch ( s -> method -> version ) {
 default : return version_cmp ( s , version , s -> version ) == 0 ;
 case TLS_ANY_VERSION : table = tls_version_table ;
 break ;
 case DTLS_ANY_VERSION : table = dtls_version_table ;
 break ;
 }
 for ( vent = table ;
 vent -> version != 0 && version_cmp ( s , version , vent -> version ) <= 0 ;
 ++ vent ) {
 if ( vent -> cmeth != NULL && version_cmp ( s , version , vent -> version ) == 0 && ssl_method_error ( s , vent -> cmeth ( ) ) == 0 ) {
 return 1 ;
 }
 }
 return 0 ;
 }