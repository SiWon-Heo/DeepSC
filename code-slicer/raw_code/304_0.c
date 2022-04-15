MSG_PROCESS_RETURN ossl_statem_server_process_message ( SSL * s , PACKET * pkt ) {
 OSSL_STATEM * st = & s -> statem ;
 switch ( st -> hand_state ) {
 case TLS_ST_SR_CLNT_HELLO : return tls_process_client_hello ( s , pkt ) ;
 case TLS_ST_SR_CERT : return tls_process_client_certificate ( s , pkt ) ;
 case TLS_ST_SR_KEY_EXCH : return tls_process_client_key_exchange ( s , pkt ) ;
 case TLS_ST_SR_CERT_VRFY : return tls_process_cert_verify ( s , pkt ) ;
 # ifndef OPENSSL_NO_NEXTPROTONEG case TLS_ST_SR_NEXT_PROTO : return tls_process_next_proto ( s , pkt ) ;
 # endif case TLS_ST_SR_CHANGE : return tls_process_change_cipher_spec ( s , pkt ) ;
 case TLS_ST_SR_FINISHED : return tls_process_finished ( s , pkt ) ;
 default : break ;
 }
 return MSG_PROCESS_ERROR ;
 }