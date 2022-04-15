int ossl_statem_server_read_transition ( SSL * s , int mt ) {
 OSSL_STATEM * st = & s -> statem ;
 switch ( st -> hand_state ) {
 case TLS_ST_BEFORE : case DTLS_ST_SW_HELLO_VERIFY_REQUEST : if ( mt == SSL3_MT_CLIENT_HELLO ) {
 st -> hand_state = TLS_ST_SR_CLNT_HELLO ;
 return 1 ;
 }
 break ;
 case TLS_ST_SW_SRVR_DONE : if ( mt == SSL3_MT_CLIENT_KEY_EXCHANGE ) {
 if ( s -> s3 -> tmp . cert_request ) {
 if ( s -> version == SSL3_VERSION ) {
 if ( ( s -> verify_mode & SSL_VERIFY_PEER ) && ( s -> verify_mode & SSL_VERIFY_FAIL_IF_NO_PEER_CERT ) ) {
 ssl3_send_alert ( s , SSL3_AL_FATAL , SSL3_AD_HANDSHAKE_FAILURE ) ;
 SSLerr ( SSL_F_OSSL_STATEM_SERVER_READ_TRANSITION , SSL_R_PEER_DID_NOT_RETURN_A_CERTIFICATE ) ;
 return 0 ;
 }
 st -> hand_state = TLS_ST_SR_KEY_EXCH ;
 return 1 ;
 }
 }
 else {
 st -> hand_state = TLS_ST_SR_KEY_EXCH ;
 return 1 ;
 }
 }
 else if ( s -> s3 -> tmp . cert_request ) {
 if ( mt == SSL3_MT_CERTIFICATE ) {
 st -> hand_state = TLS_ST_SR_CERT ;
 return 1 ;
 }
 }
 break ;
 case TLS_ST_SR_CERT : if ( mt == SSL3_MT_CLIENT_KEY_EXCHANGE ) {
 st -> hand_state = TLS_ST_SR_KEY_EXCH ;
 return 1 ;
 }
 break ;
 case TLS_ST_SR_KEY_EXCH : if ( s -> session -> peer == NULL || st -> no_cert_verify ) {
 if ( mt == SSL3_MT_CHANGE_CIPHER_SPEC ) {
 st -> hand_state = TLS_ST_SR_CHANGE ;
 return 1 ;
 }
 }
 else {
 if ( mt == SSL3_MT_CERTIFICATE_VERIFY ) {
 st -> hand_state = TLS_ST_SR_CERT_VRFY ;
 return 1 ;
 }
 }
 break ;
 case TLS_ST_SR_CERT_VRFY : if ( mt == SSL3_MT_CHANGE_CIPHER_SPEC ) {
 st -> hand_state = TLS_ST_SR_CHANGE ;
 return 1 ;
 }
 break ;
 case TLS_ST_SR_CHANGE : # ifndef OPENSSL_NO_NEXTPROTONEG if ( s -> s3 -> next_proto_neg_seen ) {
 if ( mt == SSL3_MT_NEXT_PROTO ) {
 st -> hand_state = TLS_ST_SR_NEXT_PROTO ;
 return 1 ;
 }
 }
 else {
 # endif if ( mt == SSL3_MT_FINISHED ) {
 st -> hand_state = TLS_ST_SR_FINISHED ;
 return 1 ;
 }
 # ifndef OPENSSL_NO_NEXTPROTONEG }
 # endif break ;
 # ifndef OPENSSL_NO_NEXTPROTONEG case TLS_ST_SR_NEXT_PROTO : if ( mt == SSL3_MT_FINISHED ) {
 st -> hand_state = TLS_ST_SR_FINISHED ;
 return 1 ;
 }
 break ;
 # endif case TLS_ST_SW_FINISHED : if ( mt == SSL3_MT_CHANGE_CIPHER_SPEC ) {
 st -> hand_state = TLS_ST_SR_CHANGE ;
 return 1 ;
 }
 break ;
 default : break ;
 }
 ssl3_send_alert ( s , SSL3_AL_FATAL , SSL3_AD_UNEXPECTED_MESSAGE ) ;
 SSLerr ( SSL_F_OSSL_STATEM_SERVER_READ_TRANSITION , SSL_R_UNEXPECTED_MESSAGE ) ;
 return 0 ;
 }