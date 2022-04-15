void dtls1_free ( SSL * s ) {
 DTLS_RECORD_LAYER_free ( & s -> rlayer ) ;
 ssl3_free ( s ) ;
 dtls1_clear_queues ( s ) ;
 pqueue_free ( s -> d1 -> buffered_messages ) ;
 pqueue_free ( s -> d1 -> sent_messages ) ;
 OPENSSL_free ( s -> d1 ) ;
 s -> d1 = NULL ;
 }