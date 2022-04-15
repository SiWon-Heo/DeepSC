static unsigned long dtls1_max_handshake_message_len ( const SSL * s ) {
 unsigned long max_len = DTLS1_HM_HEADER_LENGTH + SSL3_RT_MAX_ENCRYPTED_LENGTH ;
 if ( max_len < ( unsigned long ) s -> max_cert_list ) return s -> max_cert_list ;
 return max_len ;
 }