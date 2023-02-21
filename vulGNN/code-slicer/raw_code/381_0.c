int dtls1_new ( SSL * s ) {
 DTLS1_STATE * d1 ;
 if ( ! DTLS_RECORD_LAYER_new ( & s -> rlayer ) ) {
 return 0 ;
 }
 if ( ! ssl3_new ( s ) ) return ( 0 ) ;
 if ( ( d1 = OPENSSL_zalloc ( sizeof ( * d1 ) ) ) == NULL ) {
 ssl3_free ( s ) ;
 return ( 0 ) ;
 }
 d1 -> buffered_messages = pqueue_new ( ) ;
 d1 -> sent_messages = pqueue_new ( ) ;
 if ( s -> server ) {
 d1 -> cookie_len = sizeof ( s -> d1 -> cookie ) ;
 }
 d1 -> link_mtu = 0 ;
 d1 -> mtu = 0 ;
 if ( d1 -> buffered_messages == NULL || d1 -> sent_messages == NULL ) {
 pqueue_free ( d1 -> buffered_messages ) ;
 pqueue_free ( d1 -> sent_messages ) ;
 OPENSSL_free ( d1 ) ;
 ssl3_free ( s ) ;
 return ( 0 ) ;
 }
 s -> d1 = d1 ;
 s -> method -> ssl_clear ( s ) ;
 return ( 1 ) ;
 }