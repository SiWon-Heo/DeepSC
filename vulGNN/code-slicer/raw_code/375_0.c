static int ssl_method_error ( const SSL * s , const SSL_METHOD * method ) {
 int version = method -> version ;
 if ( ( s -> min_proto_version != 0 && version_cmp ( s , version , s -> min_proto_version ) < 0 ) || ssl_security ( s , SSL_SECOP_VERSION , 0 , version , NULL ) == 0 ) return SSL_R_VERSION_TOO_LOW ;
 if ( s -> max_proto_version != 0 && version_cmp ( s , version , s -> max_proto_version ) > 0 ) return SSL_R_VERSION_TOO_HIGH ;
 if ( ( s -> options & method -> mask ) != 0 ) return SSL_R_UNSUPPORTED_PROTOCOL ;
 if ( ( method -> flags & SSL_METHOD_NO_SUITEB ) != 0 && tls1_suiteb ( s ) ) return SSL_R_AT_LEAST_TLS_1_2_NEEDED_IN_SUITEB_MODE ;
 else if ( ( method -> flags & SSL_METHOD_NO_FIPS ) != 0 && FIPS_mode ( ) ) return SSL_R_AT_LEAST_TLS_1_0_NEEDED_IN_FIPS_MODE ;
 return 0 ;
 }