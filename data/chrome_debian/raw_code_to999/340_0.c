int dtls_construct_change_cipher_spec ( SSL * s ) {
 unsigned char * p ;
 p = ( unsigned char * ) s -> init_buf -> data ;
 * p ++ = SSL3_MT_CCS ;
 s -> d1 -> handshake_write_seq = s -> d1 -> next_handshake_write_seq ;
 s -> init_num = DTLS1_CCS_HEADER_LENGTH ;
 if ( s -> version == DTLS1_BAD_VER ) {
 s -> d1 -> next_handshake_write_seq ++ ;
 s2n ( s -> d1 -> handshake_write_seq , p ) ;
 s -> init_num += 2 ;
 }
 s -> init_off = 0 ;
 dtls1_set_message_header_int ( s , SSL3_MT_CCS , 0 , s -> d1 -> handshake_write_seq , 0 , 0 ) ;
 if ( ! dtls1_buffer_message ( s , 1 ) ) {
 SSLerr ( SSL_F_DTLS_CONSTRUCT_CHANGE_CIPHER_SPEC , ERR_R_INTERNAL_ERROR ) ;
 return 0 ;
 }
 return 1 ;
 }