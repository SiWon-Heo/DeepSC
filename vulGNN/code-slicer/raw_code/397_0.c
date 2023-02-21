static int dtls1_set_handshake_header ( SSL * s , int htype , unsigned long len ) {
 dtls1_set_message_header ( s , htype , len , 0 , len ) ;
 s -> init_num = ( int ) len + DTLS1_HM_HEADER_LENGTH ;
 s -> init_off = 0 ;
 if ( ! dtls1_buffer_message ( s , 0 ) ) return 0 ;
 return 1 ;
 }