static void dtls1_fix_message_header ( SSL * s , unsigned long frag_off , unsigned long frag_len ) {
 struct hm_header_st * msg_hdr = & s -> d1 -> w_msg_hdr ;
 msg_hdr -> frag_off = frag_off ;
 msg_hdr -> frag_len = frag_len ;
 }