int ssl_set_version_bound ( int method_version , int version , int * bound ) {
 if ( version == 0 ) {
 * bound = version ;
 return 1 ;
 }
 switch ( method_version ) {
 default : return 0 ;
 case TLS_ANY_VERSION : if ( version < SSL3_VERSION || version > TLS_MAX_VERSION ) return 0 ;
 break ;
 case DTLS_ANY_VERSION : if ( DTLS_VERSION_GT ( version , DTLS_MAX_VERSION ) || DTLS_VERSION_LT ( version , DTLS1_BAD_VER ) ) return 0 ;
 break ;
 }
 * bound = version ;
 return 1 ;
 }