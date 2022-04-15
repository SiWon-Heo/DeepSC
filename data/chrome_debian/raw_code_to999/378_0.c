int ssl_set_client_hello_version ( SSL * s ) {
 int ver_min , ver_max , ret ;
 ret = ssl_get_client_min_max_version ( s , & ver_min , & ver_max ) ;
 if ( ret != 0 ) return ret ;
 s -> client_version = s -> version = ver_max ;
 return 0 ;
 }