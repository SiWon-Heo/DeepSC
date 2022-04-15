int dtls1_buffer_message ( SSL * s , int is_ccs ) {
 pitem * item ;
 hm_fragment * frag ;
 unsigned char seq64be [ 8 ] ;
 OPENSSL_assert ( s -> init_off == 0 ) ;
 frag = dtls1_hm_fragment_new ( s -> init_num , 0 ) ;
 if ( frag == NULL ) return 0 ;
 memcpy ( frag -> fragment , s -> init_buf -> data , s -> init_num ) ;
 if ( is_ccs ) {
 OPENSSL_assert ( s -> d1 -> w_msg_hdr . msg_len + ( ( s -> version == DTLS1_BAD_VER ) ? 3 : DTLS1_CCS_HEADER_LENGTH ) == ( unsigned int ) s -> init_num ) ;
 }
 else {
 OPENSSL_assert ( s -> d1 -> w_msg_hdr . msg_len + DTLS1_HM_HEADER_LENGTH == ( unsigned int ) s -> init_num ) ;
 }
 frag -> msg_header . msg_len = s -> d1 -> w_msg_hdr . msg_len ;
 frag -> msg_header . seq = s -> d1 -> w_msg_hdr . seq ;
 frag -> msg_header . type = s -> d1 -> w_msg_hdr . type ;
 frag -> msg_header . frag_off = 0 ;
 frag -> msg_header . frag_len = s -> d1 -> w_msg_hdr . msg_len ;
 frag -> msg_header . is_ccs = is_ccs ;
 frag -> msg_header . saved_retransmit_state . enc_write_ctx = s -> enc_write_ctx ;
 frag -> msg_header . saved_retransmit_state . write_hash = s -> write_hash ;
 frag -> msg_header . saved_retransmit_state . compress = s -> compress ;
 frag -> msg_header . saved_retransmit_state . session = s -> session ;
 frag -> msg_header . saved_retransmit_state . epoch = DTLS_RECORD_LAYER_get_w_epoch ( & s -> rlayer ) ;
 memset ( seq64be , 0 , sizeof ( seq64be ) ) ;
 seq64be [ 6 ] = ( unsigned char ) ( dtls1_get_queue_priority ( frag -> msg_header . seq , frag -> msg_header . is_ccs ) >> 8 ) ;
 seq64be [ 7 ] = ( unsigned char ) ( dtls1_get_queue_priority ( frag -> msg_header . seq , frag -> msg_header . is_ccs ) ) ;
 item = pitem_new ( seq64be , frag ) ;
 if ( item == NULL ) {
 dtls1_hm_fragment_free ( frag ) ;
 return 0 ;
 }
 pqueue_insert ( s -> d1 -> sent_messages , item ) ;
 return 1 ;
 }