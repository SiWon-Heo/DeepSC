MSG_PROCESS_RETURN tls_process_client_hello ( SSL * s , PACKET * pkt ) {
 int i , al = SSL_AD_INTERNAL_ERROR ;
 unsigned int j , complen = 0 ;
 unsigned long id ;
 const SSL_CIPHER * c ;
 # ifndef OPENSSL_NO_COMP SSL_COMP * comp = NULL ;
 # endif STACK_OF ( SSL_CIPHER ) * ciphers = NULL ;
 int protverr ;
 PACKET session_id , cipher_suites , compression , extensions , cookie ;
 int is_v2_record ;
 static const unsigned char null_compression = 0 ;
 is_v2_record = RECORD_LAYER_is_sslv2_record ( & s -> rlayer ) ;
 PACKET_null_init ( & cookie ) ;
 if ( is_v2_record ) {
 unsigned int version ;
 unsigned int mt ;
 if ( ! PACKET_get_1 ( pkt , & mt ) || mt != SSL2_MT_CLIENT_HELLO ) {
 SSLerr ( SSL_F_TLS_PROCESS_CLIENT_HELLO , ERR_R_INTERNAL_ERROR ) ;
 goto err ;
 }
 if ( ! PACKET_get_net_2 ( pkt , & version ) ) {
 SSLerr ( SSL_F_TLS_PROCESS_CLIENT_HELLO , SSL_R_UNKNOWN_PROTOCOL ) ;
 goto err ;
 }
 if ( version == 0x0002 ) {
 SSLerr ( SSL_F_TLS_PROCESS_CLIENT_HELLO , SSL_R_UNKNOWN_PROTOCOL ) ;
 goto err ;
 }
 else if ( ( version & 0xff00 ) == ( SSL3_VERSION_MAJOR << 8 ) ) {
 s -> client_version = version ;
 }
 else {
 SSLerr ( SSL_F_TLS_PROCESS_CLIENT_HELLO , SSL_R_UNKNOWN_PROTOCOL ) ;
 goto err ;
 }
 }
 else {
 if ( ! PACKET_get_net_2 ( pkt , ( unsigned int * ) & s -> client_version ) ) {
 al = SSL_AD_DECODE_ERROR ;
 SSLerr ( SSL_F_TLS_PROCESS_CLIENT_HELLO , SSL_R_LENGTH_TOO_SHORT ) ;
 goto f_err ;
 }
 }
 if ( ! SSL_IS_DTLS ( s ) ) {
 protverr = ssl_choose_server_version ( s ) ;
 }
 else if ( s -> method -> version != DTLS_ANY_VERSION && DTLS_VERSION_LT ( s -> client_version , s -> version ) ) {
 protverr = SSL_R_VERSION_TOO_LOW ;
 }
 else {
 protverr = 0 ;
 }
 if ( protverr ) {
 SSLerr ( SSL_F_TLS_PROCESS_CLIENT_HELLO , protverr ) ;
 if ( ( ! s -> enc_write_ctx && ! s -> write_hash ) ) {
 s -> version = s -> client_version ;
 }
 al = SSL_AD_PROTOCOL_VERSION ;
 goto f_err ;
 }
 if ( is_v2_record ) {
 unsigned int cipher_len , session_id_len , challenge_len ;
 PACKET challenge ;
 if ( ! PACKET_get_net_2 ( pkt , & cipher_len ) || ! PACKET_get_net_2 ( pkt , & session_id_len ) || ! PACKET_get_net_2 ( pkt , & challenge_len ) ) {
 SSLerr ( SSL_F_TLS_PROCESS_CLIENT_HELLO , SSL_R_RECORD_LENGTH_MISMATCH ) ;
 al = SSL_AD_DECODE_ERROR ;
 goto f_err ;
 }
 if ( session_id_len > SSL_MAX_SSL_SESSION_ID_LENGTH ) {
 al = SSL_AD_DECODE_ERROR ;
 SSLerr ( SSL_F_TLS_PROCESS_CLIENT_HELLO , SSL_R_LENGTH_MISMATCH ) ;
 goto f_err ;
 }
 if ( ! PACKET_get_sub_packet ( pkt , & cipher_suites , cipher_len ) || ! PACKET_get_sub_packet ( pkt , & session_id , session_id_len ) || ! PACKET_get_sub_packet ( pkt , & challenge , challenge_len ) || PACKET_remaining ( pkt ) != 0 ) {
 SSLerr ( SSL_F_TLS_PROCESS_CLIENT_HELLO , SSL_R_RECORD_LENGTH_MISMATCH ) ;
 al = SSL_AD_DECODE_ERROR ;
 goto f_err ;
 }
 challenge_len = challenge_len > SSL3_RANDOM_SIZE ? SSL3_RANDOM_SIZE : challenge_len ;
 memset ( s -> s3 -> client_random , 0 , SSL3_RANDOM_SIZE ) ;
 if ( ! PACKET_copy_bytes ( & challenge , s -> s3 -> client_random + SSL3_RANDOM_SIZE - challenge_len , challenge_len ) || ! PACKET_buf_init ( & compression , & null_compression , 1 ) ) {
 SSLerr ( SSL_F_TLS_PROCESS_CLIENT_HELLO , ERR_R_INTERNAL_ERROR ) ;
 al = SSL_AD_INTERNAL_ERROR ;
 goto f_err ;
 }
 PACKET_null_init ( & extensions ) ;
 }
 else {
 if ( ! PACKET_copy_bytes ( pkt , s -> s3 -> client_random , SSL3_RANDOM_SIZE ) || ! PACKET_get_length_prefixed_1 ( pkt , & session_id ) ) {
 al = SSL_AD_DECODE_ERROR ;
 SSLerr ( SSL_F_TLS_PROCESS_CLIENT_HELLO , SSL_R_LENGTH_MISMATCH ) ;
 goto f_err ;
 }
 if ( PACKET_remaining ( & session_id ) > SSL_MAX_SSL_SESSION_ID_LENGTH ) {
 al = SSL_AD_DECODE_ERROR ;
 SSLerr ( SSL_F_TLS_PROCESS_CLIENT_HELLO , SSL_R_LENGTH_MISMATCH ) ;
 goto f_err ;
 }
 if ( SSL_IS_DTLS ( s ) ) {
 if ( ! PACKET_get_length_prefixed_1 ( pkt , & cookie ) ) {
 al = SSL_AD_DECODE_ERROR ;
 SSLerr ( SSL_F_TLS_PROCESS_CLIENT_HELLO , SSL_R_LENGTH_MISMATCH ) ;
 goto f_err ;
 }
 if ( SSL_get_options ( s ) & SSL_OP_COOKIE_EXCHANGE ) {
 if ( PACKET_remaining ( & cookie ) == 0 ) return 1 ;
 }
 }
 if ( ! PACKET_get_length_prefixed_2 ( pkt , & cipher_suites ) || ! PACKET_get_length_prefixed_1 ( pkt , & compression ) ) {
 al = SSL_AD_DECODE_ERROR ;
 SSLerr ( SSL_F_TLS_PROCESS_CLIENT_HELLO , SSL_R_LENGTH_MISMATCH ) ;
 goto f_err ;
 }
 extensions = * pkt ;
 }
 if ( SSL_IS_DTLS ( s ) ) {
 if ( SSL_get_options ( s ) & SSL_OP_COOKIE_EXCHANGE ) {
 if ( s -> ctx -> app_verify_cookie_cb != NULL ) {
 if ( s -> ctx -> app_verify_cookie_cb ( s , PACKET_data ( & cookie ) , PACKET_remaining ( & cookie ) ) == 0 ) {
 al = SSL_AD_HANDSHAKE_FAILURE ;
 SSLerr ( SSL_F_TLS_PROCESS_CLIENT_HELLO , SSL_R_COOKIE_MISMATCH ) ;
 goto f_err ;
 }
 }
 else if ( ! PACKET_equal ( & cookie , s -> d1 -> cookie , s -> d1 -> cookie_len ) ) {
 al = SSL_AD_HANDSHAKE_FAILURE ;
 SSLerr ( SSL_F_TLS_PROCESS_CLIENT_HELLO , SSL_R_COOKIE_MISMATCH ) ;
 goto f_err ;
 }
 s -> d1 -> cookie_verified = 1 ;
 }
 if ( s -> method -> version == DTLS_ANY_VERSION ) {
 protverr = ssl_choose_server_version ( s ) ;
 if ( protverr != 0 ) {
 SSLerr ( SSL_F_TLS_PROCESS_CLIENT_HELLO , protverr ) ;
 s -> version = s -> client_version ;
 al = SSL_AD_PROTOCOL_VERSION ;
 goto f_err ;
 }
 }
 }
 s -> hit = 0 ;
 if ( is_v2_record || ( s -> new_session && ( s -> options & SSL_OP_NO_SESSION_RESUMPTION_ON_RENEGOTIATION ) ) ) {
 if ( ! ssl_get_new_session ( s , 1 ) ) goto err ;
 }
 else {
 i = ssl_get_prev_session ( s , & extensions , & session_id ) ;
 if ( i == 1 && s -> version == s -> session -> ssl_version ) {
 s -> hit = 1 ;
 }
 else if ( i == - 1 ) {
 goto err ;
 }
 else {
 if ( ! ssl_get_new_session ( s , 1 ) ) goto err ;
 }
 }
 if ( ssl_bytes_to_cipher_list ( s , & cipher_suites , & ( ciphers ) , is_v2_record , & al ) == NULL ) {
 goto f_err ;
 }
 if ( s -> hit ) {
 j = 0 ;
 id = s -> session -> cipher -> id ;
 # ifdef CIPHER_DEBUG fprintf ( stderr , "client sent %d ciphers\n" , sk_SSL_CIPHER_num ( ciphers ) ) ;
 # endif for ( i = 0 ;
 i < sk_SSL_CIPHER_num ( ciphers ) ;
 i ++ ) {
 c = sk_SSL_CIPHER_value ( ciphers , i ) ;
 # ifdef CIPHER_DEBUG fprintf ( stderr , "client [%2d of %2d]:%s\n" , i , sk_SSL_CIPHER_num ( ciphers ) , SSL_CIPHER_get_name ( c ) ) ;
 # endif if ( c -> id == id ) {
 j = 1 ;
 break ;
 }
 }
 if ( j == 0 ) {
 al = SSL_AD_ILLEGAL_PARAMETER ;
 SSLerr ( SSL_F_TLS_PROCESS_CLIENT_HELLO , SSL_R_REQUIRED_CIPHER_MISSING ) ;
 goto f_err ;
 }
 }
 complen = PACKET_remaining ( & compression ) ;
 for ( j = 0 ;
 j < complen ;
 j ++ ) {
 if ( PACKET_data ( & compression ) [ j ] == 0 ) break ;
 }
 if ( j >= complen ) {
 al = SSL_AD_DECODE_ERROR ;
 SSLerr ( SSL_F_TLS_PROCESS_CLIENT_HELLO , SSL_R_NO_COMPRESSION_SPECIFIED ) ;
 goto f_err ;
 }
 if ( s -> version >= SSL3_VERSION ) {
 if ( ! ssl_parse_clienthello_tlsext ( s , & extensions ) ) {
 SSLerr ( SSL_F_TLS_PROCESS_CLIENT_HELLO , SSL_R_PARSE_TLSEXT ) ;
 goto err ;
 }
 }
 {
 unsigned char * pos ;
 pos = s -> s3 -> server_random ;
 if ( ssl_fill_hello_random ( s , 1 , pos , SSL3_RANDOM_SIZE ) <= 0 ) {
 goto f_err ;
 }
 }
 if ( ! s -> hit && s -> version >= TLS1_VERSION && s -> tls_session_secret_cb ) {
 const SSL_CIPHER * pref_cipher = NULL ;
 s -> session -> master_key_length = sizeof ( s -> session -> master_key ) ;
 if ( s -> tls_session_secret_cb ( s , s -> session -> master_key , & s -> session -> master_key_length , ciphers , & pref_cipher , s -> tls_session_secret_cb_arg ) ) {
 s -> hit = 1 ;
 s -> session -> ciphers = ciphers ;
 s -> session -> verify_result = X509_V_OK ;
 ciphers = NULL ;
 pref_cipher = pref_cipher ? pref_cipher : ssl3_choose_cipher ( s , s -> session -> ciphers , SSL_get_ciphers ( s ) ) ;
 if ( pref_cipher == NULL ) {
 al = SSL_AD_HANDSHAKE_FAILURE ;
 SSLerr ( SSL_F_TLS_PROCESS_CLIENT_HELLO , SSL_R_NO_SHARED_CIPHER ) ;
 goto f_err ;
 }
 s -> session -> cipher = pref_cipher ;
 sk_SSL_CIPHER_free ( s -> cipher_list ) ;
 s -> cipher_list = sk_SSL_CIPHER_dup ( s -> session -> ciphers ) ;
 sk_SSL_CIPHER_free ( s -> cipher_list_by_id ) ;
 s -> cipher_list_by_id = sk_SSL_CIPHER_dup ( s -> session -> ciphers ) ;
 }
 }
 s -> s3 -> tmp . new_compression = NULL ;
 # ifndef OPENSSL_NO_COMP if ( s -> session -> compress_meth != 0 ) {
 int m , comp_id = s -> session -> compress_meth ;
 unsigned int k ;
 if ( ! ssl_allow_compression ( s ) ) {
 SSLerr ( SSL_F_TLS_PROCESS_CLIENT_HELLO , SSL_R_INCONSISTENT_COMPRESSION ) ;
 goto f_err ;
 }
 for ( m = 0 ;
 m < sk_SSL_COMP_num ( s -> ctx -> comp_methods ) ;
 m ++ ) {
 comp = sk_SSL_COMP_value ( s -> ctx -> comp_methods , m ) ;
 if ( comp_id == comp -> id ) {
 s -> s3 -> tmp . new_compression = comp ;
 break ;
 }
 }
 if ( s -> s3 -> tmp . new_compression == NULL ) {
 SSLerr ( SSL_F_TLS_PROCESS_CLIENT_HELLO , SSL_R_INVALID_COMPRESSION_ALGORITHM ) ;
 goto f_err ;
 }
 for ( k = 0 ;
 k < complen ;
 k ++ ) {
 if ( PACKET_data ( & compression ) [ k ] == comp_id ) break ;
 }
 if ( k >= complen ) {
 al = SSL_AD_ILLEGAL_PARAMETER ;
 SSLerr ( SSL_F_TLS_PROCESS_CLIENT_HELLO , SSL_R_REQUIRED_COMPRESSION_ALGORITHM_MISSING ) ;
 goto f_err ;
 }
 }
 else if ( s -> hit ) comp = NULL ;
 else if ( ssl_allow_compression ( s ) && s -> ctx -> comp_methods ) {
 int m , nn , v , done = 0 ;
 unsigned int o ;
 nn = sk_SSL_COMP_num ( s -> ctx -> comp_methods ) ;
 for ( m = 0 ;
 m < nn ;
 m ++ ) {
 comp = sk_SSL_COMP_value ( s -> ctx -> comp_methods , m ) ;
 v = comp -> id ;
 for ( o = 0 ;
 o < complen ;
 o ++ ) {
 if ( v == PACKET_data ( & compression ) [ o ] ) {
 done = 1 ;
 break ;
 }
 }
 if ( done ) break ;
 }
 if ( done ) s -> s3 -> tmp . new_compression = comp ;
 else comp = NULL ;
 }
 # else if ( s -> session -> compress_meth != 0 ) {
 SSLerr ( SSL_F_TLS_PROCESS_CLIENT_HELLO , SSL_R_INCONSISTENT_COMPRESSION ) ;
 goto f_err ;
 }
 # endif if ( ! s -> hit ) {
 # ifdef OPENSSL_NO_COMP s -> session -> compress_meth = 0 ;
 # else s -> session -> compress_meth = ( comp == NULL ) ? 0 : comp -> id ;
 # endif sk_SSL_CIPHER_free ( s -> session -> ciphers ) ;
 s -> session -> ciphers = ciphers ;
 if ( ciphers == NULL ) {
 al = SSL_AD_INTERNAL_ERROR ;
 SSLerr ( SSL_F_TLS_PROCESS_CLIENT_HELLO , ERR_R_INTERNAL_ERROR ) ;
 goto f_err ;
 }
 ciphers = NULL ;
 if ( ! tls1_set_server_sigalgs ( s ) ) {
 SSLerr ( SSL_F_TLS_PROCESS_CLIENT_HELLO , SSL_R_CLIENTHELLO_TLSEXT ) ;
 goto err ;
 }
 }
 sk_SSL_CIPHER_free ( ciphers ) ;
 return MSG_PROCESS_CONTINUE_PROCESSING ;
 f_err : ssl3_send_alert ( s , SSL3_AL_FATAL , al ) ;
 err : ossl_statem_set_error ( s ) ;
 sk_SSL_CIPHER_free ( ciphers ) ;
 return MSG_PROCESS_ERROR ;
 }