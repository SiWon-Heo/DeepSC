int dtls_get_message ( SSL * s , int * mt , unsigned long * len ) {
 struct hm_header_st * msg_hdr ;
 unsigned char * p ;
 unsigned long msg_len ;
 int ok ;
 long tmplen ;
 msg_hdr = & s -> d1 -> r_msg_hdr ;
 memset ( msg_hdr , 0 , sizeof ( * msg_hdr ) ) ;
 again : ok = dtls_get_reassembled_message ( s , & tmplen ) ;
 if ( tmplen == DTLS1_HM_BAD_FRAGMENT || tmplen == DTLS1_HM_FRAGMENT_RETRY ) {
 goto again ;
 }
 else if ( tmplen <= 0 && ! ok ) {
 return 0 ;
 }
 * mt = s -> s3 -> tmp . message_type ;
 p = ( unsigned char * ) s -> init_buf -> data ;
 if ( * mt == SSL3_MT_CHANGE_CIPHER_SPEC ) {
 if ( s -> msg_callback ) {
 s -> msg_callback ( 0 , s -> version , SSL3_RT_CHANGE_CIPHER_SPEC , p , 1 , s , s -> msg_callback_arg ) ;
 }
 * len = ( unsigned long ) tmplen ;
 return 1 ;
 }
 msg_len = msg_hdr -> msg_len ;
 * ( p ++ ) = msg_hdr -> type ;
 l2n3 ( msg_len , p ) ;
 s2n ( msg_hdr -> seq , p ) ;
 l2n3 ( 0 , p ) ;
 l2n3 ( msg_len , p ) ;
 if ( s -> version != DTLS1_BAD_VER ) {
 p -= DTLS1_HM_HEADER_LENGTH ;
 msg_len += DTLS1_HM_HEADER_LENGTH ;
 }
 if ( ! ssl3_finish_mac ( s , p , msg_len ) ) return 0 ;
 if ( s -> msg_callback ) s -> msg_callback ( 0 , s -> version , SSL3_RT_HANDSHAKE , p , msg_len , s , s -> msg_callback_arg ) ;
 memset ( msg_hdr , 0 , sizeof ( * msg_hdr ) ) ;
 s -> d1 -> handshake_read_seq ++ ;
 s -> init_msg = s -> init_buf -> data + DTLS1_HM_HEADER_LENGTH ;
 * len = s -> init_num ;
 return 1 ;
 }