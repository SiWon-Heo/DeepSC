int dtls1_query_mtu ( SSL * s ) {
 if ( s -> d1 -> link_mtu ) {
 s -> d1 -> mtu = s -> d1 -> link_mtu - BIO_dgram_get_mtu_overhead ( SSL_get_wbio ( s ) ) ;
 s -> d1 -> link_mtu = 0 ;
 }
 if ( s -> d1 -> mtu < dtls1_min_mtu ( s ) ) {
 if ( ! ( SSL_get_options ( s ) & SSL_OP_NO_QUERY_MTU ) ) {
 s -> d1 -> mtu = BIO_ctrl ( SSL_get_wbio ( s ) , BIO_CTRL_DGRAM_QUERY_MTU , 0 , NULL ) ;
 if ( s -> d1 -> mtu < dtls1_min_mtu ( s ) ) {
 s -> d1 -> mtu = dtls1_min_mtu ( s ) ;
 BIO_ctrl ( SSL_get_wbio ( s ) , BIO_CTRL_DGRAM_SET_MTU , s -> d1 -> mtu , NULL ) ;
 }
 }
 else return 0 ;
 }
 return 1 ;
 }