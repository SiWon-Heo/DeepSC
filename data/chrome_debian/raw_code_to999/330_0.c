static int send_server_key_exchange ( SSL * s ) {
 unsigned long alg_k = s -> s3 -> tmp . new_cipher -> algorithm_mkey ;
 if ( alg_k & ( SSL_kDHE | SSL_kECDHE ) # ifndef OPENSSL_NO_PSK || ( ( alg_k & ( SSL_kPSK | SSL_kRSAPSK ) ) && s -> cert -> psk_identity_hint ) || ( alg_k & ( SSL_PSK & ( SSL_kDHEPSK | SSL_kECDHEPSK ) ) ) # endif # ifndef OPENSSL_NO_SRP || ( alg_k & SSL_kSRP ) # endif ) {
 return 1 ;
 }
 return 0 ;
 }