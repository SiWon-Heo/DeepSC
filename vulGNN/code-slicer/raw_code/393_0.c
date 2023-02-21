unsigned int dtls1_min_mtu ( SSL * s ) {
 return dtls1_link_min_mtu ( ) - BIO_dgram_get_mtu_overhead ( SSL_get_wbio ( s ) ) ;
 }