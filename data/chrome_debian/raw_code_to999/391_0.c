static void dtls1_clear_queues ( SSL * s ) {
 dtls1_clear_received_buffer ( s ) ;
 dtls1_clear_sent_buffer ( s ) ;
 }