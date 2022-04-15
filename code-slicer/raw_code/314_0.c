MSG_PROCESS_RETURN tls_process_next_proto ( SSL * s , PACKET * pkt ) {
 PACKET next_proto , padding ;
 size_t next_proto_len ;
 if ( ! PACKET_get_length_prefixed_1 ( pkt , & next_proto ) || ! PACKET_get_length_prefixed_1 ( pkt , & padding ) || PACKET_remaining ( pkt ) > 0 ) {
 SSLerr ( SSL_F_TLS_PROCESS_NEXT_PROTO , SSL_R_LENGTH_MISMATCH ) ;
 goto err ;
 }
 if ( ! PACKET_memdup ( & next_proto , & s -> next_proto_negotiated , & next_proto_len ) ) {
 s -> next_proto_negotiated_len = 0 ;
 goto err ;
 }
 s -> next_proto_negotiated_len = ( unsigned char ) next_proto_len ;
 return MSG_PROCESS_CONTINUE_READING ;
 err : ossl_statem_set_error ( s ) ;
 return MSG_PROCESS_ERROR ;
 }