void dtls1_set_message_header ( SSL * s , unsigned char mt , unsigned long len , unsigned long frag_off , unsigned long frag_len ) {
 if ( frag_off == 0 ) {
 s -> d1 -> handshake_write_seq = s -> d1 -> next_handshake_write_seq ;
 s -> d1 -> next_handshake_write_seq ++ ;
 }
 dtls1_set_message_header_int ( s , mt , len , s -> d1 -> handshake_write_seq , frag_off , frag_len ) ;
 }