static int send_certificate_request ( SSL * s ) {
 if ( s -> verify_mode & SSL_VERIFY_PEER && ( ( s -> session -> peer == NULL ) || ! ( s -> verify_mode & SSL_VERIFY_CLIENT_ONCE ) ) && ( ! ( s -> s3 -> tmp . new_cipher -> algorithm_auth & SSL_aNULL ) || ( s -> verify_mode & SSL_VERIFY_FAIL_IF_NO_PEER_CERT ) ) && ! ( s -> s3 -> tmp . new_cipher -> algorithm_auth & SSL_aSRP ) && ! ( s -> s3 -> tmp . new_cipher -> algorithm_auth & SSL_aPSK ) ) {
 return 1 ;
 }
 return 0 ;
 }