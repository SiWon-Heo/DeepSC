int dtls1_do_write ( SSL * s , int type ) {
 int ret ;
 unsigned int curr_mtu ;
 int retry = 1 ;
 unsigned int len , frag_off , mac_size , blocksize , used_len ;
 if ( ! dtls1_query_mtu ( s ) ) return - 1 ;
 if ( s -> d1 -> mtu < dtls1_min_mtu ( s ) ) return - 1 ;
 if ( s -> init_off == 0 && type == SSL3_RT_HANDSHAKE ) OPENSSL_assert ( s -> init_num == ( int ) s -> d1 -> w_msg_hdr . msg_len + DTLS1_HM_HEADER_LENGTH ) ;
 if ( s -> write_hash ) {
 if ( s -> enc_write_ctx && ( EVP_CIPHER_flags ( EVP_CIPHER_CTX_cipher ( s -> enc_write_ctx ) ) & EVP_CIPH_FLAG_AEAD_CIPHER ) != 0 ) mac_size = 0 ;
 else mac_size = EVP_MD_CTX_size ( s -> write_hash ) ;
 }
 else mac_size = 0 ;
 if ( s -> enc_write_ctx && ( EVP_CIPHER_CTX_mode ( s -> enc_write_ctx ) == EVP_CIPH_CBC_MODE ) ) blocksize = 2 * EVP_CIPHER_CTX_block_size ( s -> enc_write_ctx ) ;
 else blocksize = 0 ;
 frag_off = 0 ;
 s -> rwstate = SSL_NOTHING ;
 while ( s -> init_num > 0 ) {
 if ( type == SSL3_RT_HANDSHAKE && s -> init_off != 0 ) {
 if ( frag_off > 0 ) {
 if ( s -> init_off <= DTLS1_HM_HEADER_LENGTH ) {
 return - 1 ;
 }
 s -> init_off -= DTLS1_HM_HEADER_LENGTH ;
 s -> init_num += DTLS1_HM_HEADER_LENGTH ;
 }
 else {
 frag_off = s -> d1 -> w_msg_hdr . frag_off ;
 }
 }
 used_len = BIO_wpending ( s -> wbio ) + DTLS1_RT_HEADER_LENGTH + mac_size + blocksize ;
 if ( s -> d1 -> mtu > used_len ) curr_mtu = s -> d1 -> mtu - used_len ;
 else curr_mtu = 0 ;
 if ( curr_mtu <= DTLS1_HM_HEADER_LENGTH ) {
 ret = BIO_flush ( s -> wbio ) ;
 if ( ret <= 0 ) {
 s -> rwstate = SSL_WRITING ;
 return ret ;
 }
 used_len = DTLS1_RT_HEADER_LENGTH + mac_size + blocksize ;
 if ( s -> d1 -> mtu > used_len + DTLS1_HM_HEADER_LENGTH ) {
 curr_mtu = s -> d1 -> mtu - used_len ;
 }
 else {
 return - 1 ;
 }
 }
 if ( ( ( unsigned int ) s -> init_num ) > curr_mtu ) len = curr_mtu ;
 else len = s -> init_num ;
 if ( len > INT_MAX ) len = INT_MAX ;
 if ( type == SSL3_RT_HANDSHAKE ) {
 if ( len < DTLS1_HM_HEADER_LENGTH ) {
 return - 1 ;
 }
 dtls1_fix_message_header ( s , frag_off , len - DTLS1_HM_HEADER_LENGTH ) ;
 dtls1_write_message_header ( s , ( unsigned char * ) & s -> init_buf -> data [ s -> init_off ] ) ;
 }
 ret = dtls1_write_bytes ( s , type , & s -> init_buf -> data [ s -> init_off ] , len ) ;
 if ( ret < 0 ) {
 if ( retry && BIO_ctrl ( SSL_get_wbio ( s ) , BIO_CTRL_DGRAM_MTU_EXCEEDED , 0 , NULL ) > 0 ) {
 if ( ! ( SSL_get_options ( s ) & SSL_OP_NO_QUERY_MTU ) ) {
 if ( ! dtls1_query_mtu ( s ) ) return - 1 ;
 retry = 0 ;
 }
 else return - 1 ;
 }
 else {
 return ( - 1 ) ;
 }
 }
 else {
 OPENSSL_assert ( len == ( unsigned int ) ret ) ;
 if ( type == SSL3_RT_HANDSHAKE && ! s -> d1 -> retransmitting ) {
 unsigned char * p = ( unsigned char * ) & s -> init_buf -> data [ s -> init_off ] ;
 const struct hm_header_st * msg_hdr = & s -> d1 -> w_msg_hdr ;
 int xlen ;
 if ( frag_off == 0 && s -> version != DTLS1_BAD_VER ) {
 * p ++ = msg_hdr -> type ;
 l2n3 ( msg_hdr -> msg_len , p ) ;
 s2n ( msg_hdr -> seq , p ) ;
 l2n3 ( 0 , p ) ;
 l2n3 ( msg_hdr -> msg_len , p ) ;
 p -= DTLS1_HM_HEADER_LENGTH ;
 xlen = ret ;
 }
 else {
 p += DTLS1_HM_HEADER_LENGTH ;
 xlen = ret - DTLS1_HM_HEADER_LENGTH ;
 }
 if ( ! ssl3_finish_mac ( s , p , xlen ) ) return - 1 ;
 }
 if ( ret == s -> init_num ) {
 if ( s -> msg_callback ) s -> msg_callback ( 1 , s -> version , type , s -> init_buf -> data , ( size_t ) ( s -> init_off + s -> init_num ) , s , s -> msg_callback_arg ) ;
 s -> init_off = 0 ;
 s -> init_num = 0 ;
 return ( 1 ) ;
 }
 s -> init_off += ret ;
 s -> init_num -= ret ;
 ret -= DTLS1_HM_HEADER_LENGTH ;
 frag_off += ret ;
 dtls1_fix_message_header ( s , frag_off , 0 ) ;
 }
 }
 return ( 0 ) ;
 }