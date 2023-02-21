unsigned long ossl_statem_server_max_message_size ( SSL * s ) {
 OSSL_STATEM * st = & s -> statem ;
 switch ( st -> hand_state ) {
 case TLS_ST_SR_CLNT_HELLO : return CLIENT_HELLO_MAX_LENGTH ;
 case TLS_ST_SR_CERT : return s -> max_cert_list ;
 case TLS_ST_SR_KEY_EXCH : return CLIENT_KEY_EXCH_MAX_LENGTH ;
 case TLS_ST_SR_CERT_VRFY : return SSL3_RT_MAX_PLAIN_LENGTH ;
 # ifndef OPENSSL_NO_NEXTPROTONEG case TLS_ST_SR_NEXT_PROTO : return NEXT_PROTO_MAX_LENGTH ;
 # endif case TLS_ST_SR_CHANGE : return CCS_MAX_LENGTH ;
 case TLS_ST_SR_FINISHED : return FINISHED_MAX_LENGTH ;
 default : break ;
 }
 return 0 ;
 }