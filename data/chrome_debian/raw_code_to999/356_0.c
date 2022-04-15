static int dtls_get_reassembled_message ( SSL * s , long * len ) {
 unsigned char wire [ DTLS1_HM_HEADER_LENGTH ] ;
 unsigned long mlen , frag_off , frag_len ;
 int i , al , recvd_type ;
 struct hm_header_st msg_hdr ;
 int ok ;
 redo : if ( ( frag_len = dtls1_retrieve_buffered_fragment ( s , & ok ) ) || ok ) {
 if ( ok ) s -> init_num = frag_len ;
 * len = frag_len ;
 return ok ;
 }
 i = s -> method -> ssl_read_bytes ( s , SSL3_RT_HANDSHAKE , & recvd_type , wire , DTLS1_HM_HEADER_LENGTH , 0 ) ;
 if ( i <= 0 ) {
 s -> rwstate = SSL_READING ;
 * len = i ;
 return 0 ;
 }
 if ( recvd_type == SSL3_RT_CHANGE_CIPHER_SPEC ) {
 if ( wire [ 0 ] != SSL3_MT_CCS ) {
 al = SSL_AD_UNEXPECTED_MESSAGE ;
 SSLerr ( SSL_F_DTLS_GET_REASSEMBLED_MESSAGE , SSL_R_BAD_CHANGE_CIPHER_SPEC ) ;
 goto f_err ;
 }
 memcpy ( s -> init_buf -> data , wire , i ) ;
 s -> init_num = i - 1 ;
 s -> init_msg = s -> init_buf -> data + 1 ;
 s -> s3 -> tmp . message_type = SSL3_MT_CHANGE_CIPHER_SPEC ;
 s -> s3 -> tmp . message_size = i - 1 ;
 * len = i - 1 ;
 return 1 ;
 }
 if ( i != DTLS1_HM_HEADER_LENGTH ) {
 al = SSL_AD_UNEXPECTED_MESSAGE ;
 SSLerr ( SSL_F_DTLS_GET_REASSEMBLED_MESSAGE , SSL_R_UNEXPECTED_MESSAGE ) ;
 goto f_err ;
 }
 dtls1_get_message_header ( wire , & msg_hdr ) ;
 mlen = msg_hdr . msg_len ;
 frag_off = msg_hdr . frag_off ;
 frag_len = msg_hdr . frag_len ;
 if ( frag_len > RECORD_LAYER_get_rrec_length ( & s -> rlayer ) ) {
 al = SSL3_AD_ILLEGAL_PARAMETER ;
 SSLerr ( SSL_F_DTLS_GET_REASSEMBLED_MESSAGE , SSL_R_BAD_LENGTH ) ;
 goto f_err ;
 }
 if ( msg_hdr . seq != s -> d1 -> handshake_read_seq ) {
 * len = dtls1_process_out_of_seq_message ( s , & msg_hdr , & ok ) ;
 return ok ;
 }
 if ( frag_len && frag_len < mlen ) {
 * len = dtls1_reassemble_fragment ( s , & msg_hdr , & ok ) ;
 return ok ;
 }
 if ( ! s -> server && s -> d1 -> r_msg_hdr . frag_off == 0 && wire [ 0 ] == SSL3_MT_HELLO_REQUEST ) {
 if ( wire [ 1 ] == 0 && wire [ 2 ] == 0 && wire [ 3 ] == 0 ) {
 if ( s -> msg_callback ) s -> msg_callback ( 0 , s -> version , SSL3_RT_HANDSHAKE , wire , DTLS1_HM_HEADER_LENGTH , s , s -> msg_callback_arg ) ;
 s -> init_num = 0 ;
 goto redo ;
 }
 else {
 al = SSL_AD_UNEXPECTED_MESSAGE ;
 SSLerr ( SSL_F_DTLS_GET_REASSEMBLED_MESSAGE , SSL_R_UNEXPECTED_MESSAGE ) ;
 goto f_err ;
 }
 }
 if ( ( al = dtls1_preprocess_fragment ( s , & msg_hdr ) ) ) goto f_err ;
 if ( frag_len > 0 ) {
 unsigned char * p = ( unsigned char * ) s -> init_buf -> data + DTLS1_HM_HEADER_LENGTH ;
 i = s -> method -> ssl_read_bytes ( s , SSL3_RT_HANDSHAKE , NULL , & p [ frag_off ] , frag_len , 0 ) ;
 if ( i <= 0 ) {
 s -> rwstate = SSL_READING ;
 * len = i ;
 return 0 ;
 }
 }
 else i = 0 ;
 if ( i != ( int ) frag_len ) {
 al = SSL3_AD_ILLEGAL_PARAMETER ;
 SSLerr ( SSL_F_DTLS_GET_REASSEMBLED_MESSAGE , SSL3_AD_ILLEGAL_PARAMETER ) ;
 goto f_err ;
 }
 * len = s -> init_num = frag_len ;
 return 1 ;
 f_err : ssl3_send_alert ( s , SSL3_AL_FATAL , al ) ;
 s -> init_num = 0 ;
 * len = - 1 ;
 return 0 ;
 }