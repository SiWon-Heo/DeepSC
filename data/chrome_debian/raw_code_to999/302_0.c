WRITE_TRAN ossl_statem_server_write_transition ( SSL * s ) {
 OSSL_STATEM * st = & s -> statem ;
 switch ( st -> hand_state ) {
 case TLS_ST_BEFORE : return WRITE_TRAN_FINISHED ;
 case TLS_ST_OK : st -> hand_state = TLS_ST_SW_HELLO_REQ ;
 return WRITE_TRAN_CONTINUE ;
 case TLS_ST_SW_HELLO_REQ : st -> hand_state = TLS_ST_OK ;
 ossl_statem_set_in_init ( s , 0 ) ;
 return WRITE_TRAN_CONTINUE ;
 case TLS_ST_SR_CLNT_HELLO : if ( SSL_IS_DTLS ( s ) && ! s -> d1 -> cookie_verified && ( SSL_get_options ( s ) & SSL_OP_COOKIE_EXCHANGE ) ) st -> hand_state = DTLS_ST_SW_HELLO_VERIFY_REQUEST ;
 else st -> hand_state = TLS_ST_SW_SRVR_HELLO ;
 return WRITE_TRAN_CONTINUE ;
 case DTLS_ST_SW_HELLO_VERIFY_REQUEST : return WRITE_TRAN_FINISHED ;
 case TLS_ST_SW_SRVR_HELLO : if ( s -> hit ) {
 if ( s -> tlsext_ticket_expected ) st -> hand_state = TLS_ST_SW_SESSION_TICKET ;
 else st -> hand_state = TLS_ST_SW_CHANGE ;
 }
 else {
 if ( ! ( s -> s3 -> tmp . new_cipher -> algorithm_auth & ( SSL_aNULL | SSL_aSRP | SSL_aPSK ) ) ) {
 st -> hand_state = TLS_ST_SW_CERT ;
 }
 else if ( send_server_key_exchange ( s ) ) {
 st -> hand_state = TLS_ST_SW_KEY_EXCH ;
 }
 else if ( send_certificate_request ( s ) ) {
 st -> hand_state = TLS_ST_SW_CERT_REQ ;
 }
 else {
 st -> hand_state = TLS_ST_SW_SRVR_DONE ;
 }
 }
 return WRITE_TRAN_CONTINUE ;
 case TLS_ST_SW_CERT : if ( s -> tlsext_status_expected ) {
 st -> hand_state = TLS_ST_SW_CERT_STATUS ;
 return WRITE_TRAN_CONTINUE ;
 }
 case TLS_ST_SW_CERT_STATUS : if ( send_server_key_exchange ( s ) ) {
 st -> hand_state = TLS_ST_SW_KEY_EXCH ;
 return WRITE_TRAN_CONTINUE ;
 }
 case TLS_ST_SW_KEY_EXCH : if ( send_certificate_request ( s ) ) {
 st -> hand_state = TLS_ST_SW_CERT_REQ ;
 return WRITE_TRAN_CONTINUE ;
 }
 case TLS_ST_SW_CERT_REQ : st -> hand_state = TLS_ST_SW_SRVR_DONE ;
 return WRITE_TRAN_CONTINUE ;
 case TLS_ST_SW_SRVR_DONE : return WRITE_TRAN_FINISHED ;
 case TLS_ST_SR_FINISHED : if ( s -> hit ) {
 st -> hand_state = TLS_ST_OK ;
 ossl_statem_set_in_init ( s , 0 ) ;
 return WRITE_TRAN_CONTINUE ;
 }
 else if ( s -> tlsext_ticket_expected ) {
 st -> hand_state = TLS_ST_SW_SESSION_TICKET ;
 }
 else {
 st -> hand_state = TLS_ST_SW_CHANGE ;
 }
 return WRITE_TRAN_CONTINUE ;
 case TLS_ST_SW_SESSION_TICKET : st -> hand_state = TLS_ST_SW_CHANGE ;
 return WRITE_TRAN_CONTINUE ;
 case TLS_ST_SW_CHANGE : st -> hand_state = TLS_ST_SW_FINISHED ;
 return WRITE_TRAN_CONTINUE ;
 case TLS_ST_SW_FINISHED : if ( s -> hit ) {
 return WRITE_TRAN_FINISHED ;
 }
 st -> hand_state = TLS_ST_OK ;
 ossl_statem_set_in_init ( s , 0 ) ;
 return WRITE_TRAN_CONTINUE ;
 default : return WRITE_TRAN_ERROR ;
 }
 }