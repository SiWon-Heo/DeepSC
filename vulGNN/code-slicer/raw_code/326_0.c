int ossl_statem_server_construct_message ( SSL * s ) {
 OSSL_STATEM * st = & s -> statem ;
 switch ( st -> hand_state ) {
 case DTLS_ST_SW_HELLO_VERIFY_REQUEST : return dtls_construct_hello_verify_request ( s ) ;
 case TLS_ST_SW_HELLO_REQ : return tls_construct_hello_request ( s ) ;
 case TLS_ST_SW_SRVR_HELLO : return tls_construct_server_hello ( s ) ;
 case TLS_ST_SW_CERT : return tls_construct_server_certificate ( s ) ;
 case TLS_ST_SW_KEY_EXCH : return tls_construct_server_key_exchange ( s ) ;
 case TLS_ST_SW_CERT_REQ : return tls_construct_certificate_request ( s ) ;
 case TLS_ST_SW_SRVR_DONE : return tls_construct_server_done ( s ) ;
 case TLS_ST_SW_SESSION_TICKET : return tls_construct_new_session_ticket ( s ) ;
 case TLS_ST_SW_CERT_STATUS : return tls_construct_cert_status ( s ) ;
 case TLS_ST_SW_CHANGE : if ( SSL_IS_DTLS ( s ) ) return dtls_construct_change_cipher_spec ( s ) ;
 else return tls_construct_change_cipher_spec ( s ) ;
 case TLS_ST_SW_FINISHED : return tls_construct_finished ( s , s -> method -> ssl3_enc -> server_finished_label , s -> method -> ssl3_enc -> server_finished_label_len ) ;
 default : break ;
 }
 return 0 ;
 }