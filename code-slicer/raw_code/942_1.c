int dtls1_get_record ( SSL * s ) {
 int ssl_major , ssl_minor ;
 int i , n ;
 SSL3_RECORD * rr ;
 unsigned char * p = NULL ;
 unsigned short version ;
 DTLS1_BITMAP * bitmap ;
 unsigned int is_next_epoch ;
 rr = RECORD_LAYER_get_rrec ( & s -> rlayer ) ;
 again : if ( dtls1_process_buffered_records ( s ) < 0 ) return - 1 ;
 if ( dtls1_get_processed_record ( s ) ) return 1 ;
 if ( ( RECORD_LAYER_get_rstate ( & s -> rlayer ) != SSL_ST_READ_BODY ) || ( RECORD_LAYER_get_packet_length ( & s -> rlayer ) < DTLS1_RT_HEADER_LENGTH ) ) {
 n = ssl3_read_n ( s , DTLS1_RT_HEADER_LENGTH , SSL3_BUFFER_get_len ( & s -> rlayer . rbuf ) , 0 , 1 ) ;
 if ( n <= 0 ) return ( n ) ;
 if ( RECORD_LAYER_get_packet_length ( & s -> rlayer ) != DTLS1_RT_HEADER_LENGTH ) {
 RECORD_LAYER_reset_packet_length ( & s -> rlayer ) ;
 goto again ;
 }
 RECORD_LAYER_set_rstate ( & s -> rlayer , SSL_ST_READ_BODY ) ;
 p = RECORD_LAYER_get_packet ( & s -> rlayer ) ;
 if ( s -> msg_callback ) s -> msg_callback ( 0 , 0 , SSL3_RT_HEADER , p , DTLS1_RT_HEADER_LENGTH , s , s -> msg_callback_arg ) ;
 rr -> type = * ( p ++ ) ;
 ssl_major = * ( p ++ ) ;
 ssl_minor = * ( p ++ ) ;
 version = ( ssl_major << 8 ) | ssl_minor ;
 n2s ( p , rr -> epoch ) ;
 memcpy ( & ( RECORD_LAYER_get_read_sequence ( & s -> rlayer ) [ 2 ] ) , p , 6 ) ;
 p += 6 ;
 n2s ( p , rr -> length ) ;
 if ( ! s -> first_packet ) {
 if ( version != s -> version ) {
 rr -> length = 0 ;
 RECORD_LAYER_reset_packet_length ( & s -> rlayer ) ;
 goto again ;
 }
 }
 if ( ( version & 0xff00 ) != ( s -> version & 0xff00 ) ) {
 rr -> length = 0 ;
 RECORD_LAYER_reset_packet_length ( & s -> rlayer ) ;
 goto again ;
 }
 if ( rr -> length > SSL3_RT_MAX_ENCRYPTED_LENGTH ) {
 rr -> length = 0 ;
 RECORD_LAYER_reset_packet_length ( & s -> rlayer ) ;
 goto again ;
 }
 }
 if ( rr -> length > RECORD_LAYER_get_packet_length ( & s -> rlayer ) - DTLS1_RT_HEADER_LENGTH ) {
 i = rr -> length ;
 n = ssl3_read_n ( s , i , i , 1 , 1 ) ;
 if ( n != i ) {
 rr -> length = 0 ;
 RECORD_LAYER_reset_packet_length ( & s -> rlayer ) ;
 goto again ;
 }
 }
 RECORD_LAYER_set_rstate ( & s -> rlayer , SSL_ST_READ_HEADER ) ;
 bitmap = dtls1_get_bitmap ( s , rr , & is_next_epoch ) ;
 if ( bitmap == NULL ) {
 rr -> length = 0 ;
 RECORD_LAYER_reset_packet_length ( & s -> rlayer ) ;
 goto again ;
 }
 # ifndef OPENSSL_NO_SCTP if ( ! BIO_dgram_is_sctp ( SSL_get_rbio ( s ) ) ) {
 # endif if ( ! dtls1_record_replay_check ( s , bitmap ) ) {
 rr -> length = 0 ;
 RECORD_LAYER_reset_packet_length ( & s -> rlayer ) ;
 goto again ;
 }
 # ifndef OPENSSL_NO_SCTP }
 # endif if ( rr -> length == 0 ) goto again ;
 if ( is_next_epoch ) {
 if ( ( SSL_in_init ( s ) || ossl_statem_get_in_handshake ( s ) ) ) {
 if ( dtls1_buffer_record ( s , & ( DTLS_RECORD_LAYER_get_unprocessed_rcds ( & s -> rlayer ) ) , rr -> seq_num ) < 0 ) return - 1 ;
 dtls1_record_bitmap_update ( s , bitmap ) ;
 }
 rr -> length = 0 ;
 RECORD_LAYER_reset_packet_length ( & s -> rlayer ) ;
 goto again ;
 }
 if ( ! dtls1_process_record ( s ) ) {
 rr -> length = 0 ;
 RECORD_LAYER_reset_packet_length ( & s -> rlayer ) ;
 goto again ;
 }
 dtls1_record_bitmap_update ( s , bitmap ) ;
 return ( 1 ) ;
 }