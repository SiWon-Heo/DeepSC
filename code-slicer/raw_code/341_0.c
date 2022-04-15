void dtls1_hm_fragment_free ( hm_fragment * frag ) {
 if ( ! frag ) return ;
 if ( frag -> msg_header . is_ccs ) {
 EVP_CIPHER_CTX_free ( frag -> msg_header . saved_retransmit_state . enc_write_ctx ) ;
 EVP_MD_CTX_free ( frag -> msg_header . saved_retransmit_state . write_hash ) ;
 }
 OPENSSL_free ( frag -> fragment ) ;
 OPENSSL_free ( frag -> reassembly ) ;
 OPENSSL_free ( frag ) ;
 }