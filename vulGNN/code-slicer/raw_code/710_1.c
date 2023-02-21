int ssl3_read_bytes ( SSL * s , int type , int * recvd_type , unsigned char * buf , int len , int peek ) {
 int al , i , j , ret ;
 unsigned int n , curr_rec , num_recs , read_bytes ;
 SSL3_RECORD * rr ;
 SSL3_BUFFER * rbuf ;
 void ( * cb ) ( const SSL * ssl , int type2 , int val ) = NULL ;
 rbuf = & s -> rlayer . rbuf ;
 if ( ! SSL3_BUFFER_is_initialised ( rbuf ) ) {
 if ( ! ssl3_setup_read_buffer ( s ) ) return ( - 1 ) ;
 }
 if ( ( type && ( type != SSL3_RT_APPLICATION_DATA ) && ( type != SSL3_RT_HANDSHAKE ) ) || ( peek && ( type != SSL3_RT_APPLICATION_DATA ) ) ) {
 SSLerr ( SSL_F_SSL3_READ_BYTES , ERR_R_INTERNAL_ERROR ) ;
 return - 1 ;
 }
 if ( ( type == SSL3_RT_HANDSHAKE ) && ( s -> rlayer . handshake_fragment_len > 0 ) ) {
 unsigned char * src = s -> rlayer . handshake_fragment ;
 unsigned char * dst = buf ;
 unsigned int k ;
 n = 0 ;
 while ( ( len > 0 ) && ( s -> rlayer . handshake_fragment_len > 0 ) ) {
 * dst ++ = * src ++ ;
 len -- ;
 s -> rlayer . handshake_fragment_len -- ;
 n ++ ;
 }
 for ( k = 0 ;
 k < s -> rlayer . handshake_fragment_len ;
 k ++ ) s -> rlayer . handshake_fragment [ k ] = * src ++ ;
 if ( recvd_type != NULL ) * recvd_type = SSL3_RT_HANDSHAKE ;
 return n ;
 }
 if ( ! ossl_statem_get_in_handshake ( s ) && SSL_in_init ( s ) ) {
 i = s -> handshake_func ( s ) ;
 if ( i < 0 ) return ( i ) ;
 if ( i == 0 ) {
 SSLerr ( SSL_F_SSL3_READ_BYTES , SSL_R_SSL_HANDSHAKE_FAILURE ) ;
 return ( - 1 ) ;
 }
 }
 start : s -> rwstate = SSL_NOTHING ;
 rr = s -> rlayer . rrec ;
 num_recs = RECORD_LAYER_get_numrpipes ( & s -> rlayer ) ;
 do {
 if ( num_recs == 0 ) {
 ret = ssl3_get_record ( s ) ;
 if ( ret <= 0 ) return ( ret ) ;
 num_recs = RECORD_LAYER_get_numrpipes ( & s -> rlayer ) ;
 if ( num_recs == 0 ) {
 al = SSL_AD_INTERNAL_ERROR ;
 SSLerr ( SSL_F_SSL3_READ_BYTES , ERR_R_INTERNAL_ERROR ) ;
 goto f_err ;
 }
 }
 for ( curr_rec = 0 ;
 curr_rec < num_recs && SSL3_RECORD_is_read ( & rr [ curr_rec ] ) ;
 curr_rec ++ ) ;
 if ( curr_rec == num_recs ) {
 RECORD_LAYER_set_numrpipes ( & s -> rlayer , 0 ) ;
 num_recs = 0 ;
 curr_rec = 0 ;
 }
 }
 while ( num_recs == 0 ) ;
 rr = & rr [ curr_rec ] ;
 if ( s -> s3 -> change_cipher_spec && ( SSL3_RECORD_get_type ( rr ) != SSL3_RT_HANDSHAKE ) ) {
 al = SSL_AD_UNEXPECTED_MESSAGE ;
 SSLerr ( SSL_F_SSL3_READ_BYTES , SSL_R_DATA_BETWEEN_CCS_AND_FINISHED ) ;
 goto f_err ;
 }
 if ( s -> shutdown & SSL_RECEIVED_SHUTDOWN ) {
 SSL3_RECORD_set_length ( rr , 0 ) ;
 s -> rwstate = SSL_NOTHING ;
 return ( 0 ) ;
 }
 if ( type == SSL3_RECORD_get_type ( rr ) || ( SSL3_RECORD_get_type ( rr ) == SSL3_RT_CHANGE_CIPHER_SPEC && type == SSL3_RT_HANDSHAKE && recvd_type != NULL ) ) {
 if ( SSL_in_init ( s ) && ( type == SSL3_RT_APPLICATION_DATA ) && ( s -> enc_read_ctx == NULL ) ) {
 al = SSL_AD_UNEXPECTED_MESSAGE ;
 SSLerr ( SSL_F_SSL3_READ_BYTES , SSL_R_APP_DATA_IN_HANDSHAKE ) ;
 goto f_err ;
 }
 if ( type == SSL3_RT_HANDSHAKE && SSL3_RECORD_get_type ( rr ) == SSL3_RT_CHANGE_CIPHER_SPEC && s -> rlayer . handshake_fragment_len > 0 ) {
 al = SSL_AD_UNEXPECTED_MESSAGE ;
 SSLerr ( SSL_F_SSL3_READ_BYTES , SSL_R_CCS_RECEIVED_EARLY ) ;
 goto f_err ;
 }
 if ( recvd_type != NULL ) * recvd_type = SSL3_RECORD_get_type ( rr ) ;
 if ( len <= 0 ) return ( len ) ;
 read_bytes = 0 ;
 do {
 if ( ( unsigned int ) len - read_bytes > SSL3_RECORD_get_length ( rr ) ) n = SSL3_RECORD_get_length ( rr ) ;
 else n = ( unsigned int ) len - read_bytes ;
 memcpy ( buf , & ( rr -> data [ rr -> off ] ) , n ) ;
 buf += n ;
 if ( ! peek ) {
 SSL3_RECORD_sub_length ( rr , n ) ;
 SSL3_RECORD_add_off ( rr , n ) ;
 if ( SSL3_RECORD_get_length ( rr ) == 0 ) {
 s -> rlayer . rstate = SSL_ST_READ_HEADER ;
 SSL3_RECORD_set_off ( rr , 0 ) ;
 SSL3_RECORD_set_read ( rr ) ;
 }
 }
 if ( SSL3_RECORD_get_length ( rr ) == 0 || ( peek && n == SSL3_RECORD_get_length ( rr ) ) ) {
 curr_rec ++ ;
 rr ++ ;
 }
 read_bytes += n ;
 }
 while ( type == SSL3_RT_APPLICATION_DATA && curr_rec < num_recs && read_bytes < ( unsigned int ) len ) ;
 if ( read_bytes == 0 ) {
 goto start ;
 }
 if ( ! peek && curr_rec == num_recs && ( s -> mode & SSL_MODE_RELEASE_BUFFERS ) && SSL3_BUFFER_get_left ( rbuf ) == 0 ) ssl3_release_read_buffer ( s ) ;
 return read_bytes ;
 }
 if ( rr -> rec_version == SSL2_VERSION ) {
 al = SSL_AD_INTERNAL_ERROR ;
 SSLerr ( SSL_F_SSL3_READ_BYTES , ERR_R_INTERNAL_ERROR ) ;
 goto f_err ;
 }
 if ( s -> method -> version == TLS_ANY_VERSION && ( s -> server || rr -> type != SSL3_RT_ALERT ) ) {
 s -> version = rr -> rec_version ;
 al = SSL_AD_UNEXPECTED_MESSAGE ;
 SSLerr ( SSL_F_SSL3_READ_BYTES , SSL_R_UNEXPECTED_MESSAGE ) ;
 goto f_err ;
 }
 {
 unsigned int dest_maxlen = 0 ;
 unsigned char * dest = NULL ;
 unsigned int * dest_len = NULL ;
 if ( SSL3_RECORD_get_type ( rr ) == SSL3_RT_HANDSHAKE ) {
 dest_maxlen = sizeof s -> rlayer . handshake_fragment ;
 dest = s -> rlayer . handshake_fragment ;
 dest_len = & s -> rlayer . handshake_fragment_len ;
 }
 else if ( SSL3_RECORD_get_type ( rr ) == SSL3_RT_ALERT ) {
 dest_maxlen = sizeof s -> rlayer . alert_fragment ;
 dest = s -> rlayer . alert_fragment ;
 dest_len = & s -> rlayer . alert_fragment_len ;
 }
 if ( dest_maxlen > 0 ) {
 n = dest_maxlen - * dest_len ;
 if ( SSL3_RECORD_get_length ( rr ) < n ) n = SSL3_RECORD_get_length ( rr ) ;
 while ( n -- > 0 ) {
 dest [ ( * dest_len ) ++ ] = SSL3_RECORD_get_data ( rr ) [ SSL3_RECORD_get_off ( rr ) ] ;
 SSL3_RECORD_add_off ( rr , 1 ) ;
 SSL3_RECORD_add_length ( rr , - 1 ) ;
 }
 if ( * dest_len < dest_maxlen ) {
 SSL3_RECORD_set_read ( rr ) ;
 goto start ;
 }
 }
 }
 if ( ( ! s -> server ) && ( s -> rlayer . handshake_fragment_len >= 4 ) && ( s -> rlayer . handshake_fragment [ 0 ] == SSL3_MT_HELLO_REQUEST ) && ( s -> session != NULL ) && ( s -> session -> cipher != NULL ) ) {
 s -> rlayer . handshake_fragment_len = 0 ;
 if ( ( s -> rlayer . handshake_fragment [ 1 ] != 0 ) || ( s -> rlayer . handshake_fragment [ 2 ] != 0 ) || ( s -> rlayer . handshake_fragment [ 3 ] != 0 ) ) {
 al = SSL_AD_DECODE_ERROR ;
 SSLerr ( SSL_F_SSL3_READ_BYTES , SSL_R_BAD_HELLO_REQUEST ) ;
 goto f_err ;
 }
 if ( s -> msg_callback ) s -> msg_callback ( 0 , s -> version , SSL3_RT_HANDSHAKE , s -> rlayer . handshake_fragment , 4 , s , s -> msg_callback_arg ) ;
 if ( SSL_is_init_finished ( s ) && ! ( s -> s3 -> flags & SSL3_FLAGS_NO_RENEGOTIATE_CIPHERS ) && ! s -> s3 -> renegotiate ) {
 ssl3_renegotiate ( s ) ;
 if ( ssl3_renegotiate_check ( s ) ) {
 i = s -> handshake_func ( s ) ;
 if ( i < 0 ) return ( i ) ;
 if ( i == 0 ) {
 SSLerr ( SSL_F_SSL3_READ_BYTES , SSL_R_SSL_HANDSHAKE_FAILURE ) ;
 return ( - 1 ) ;
 }
 if ( ! ( s -> mode & SSL_MODE_AUTO_RETRY ) ) {
 if ( SSL3_BUFFER_get_left ( rbuf ) == 0 ) {
 BIO * bio ;
 s -> rwstate = SSL_READING ;
 bio = SSL_get_rbio ( s ) ;
 BIO_clear_retry_flags ( bio ) ;
 BIO_set_retry_read ( bio ) ;
 return ( - 1 ) ;
 }
 }
 }
 }
 goto start ;
 }
 if ( s -> server && SSL_is_init_finished ( s ) && ! s -> s3 -> send_connection_binding && ( s -> version > SSL3_VERSION ) && ( s -> rlayer . handshake_fragment_len >= 4 ) && ( s -> rlayer . handshake_fragment [ 0 ] == SSL3_MT_CLIENT_HELLO ) && ( s -> session != NULL ) && ( s -> session -> cipher != NULL ) && ! ( s -> ctx -> options & SSL_OP_ALLOW_UNSAFE_LEGACY_RENEGOTIATION ) ) {
 SSL3_RECORD_set_length ( rr , 0 ) ;
 SSL3_RECORD_set_read ( rr ) ;
 ssl3_send_alert ( s , SSL3_AL_WARNING , SSL_AD_NO_RENEGOTIATION ) ;
 goto start ;
 }
 if ( s -> rlayer . alert_fragment_len >= 2 ) {
 int alert_level = s -> rlayer . alert_fragment [ 0 ] ;
 int alert_descr = s -> rlayer . alert_fragment [ 1 ] ;
 s -> rlayer . alert_fragment_len = 0 ;
 if ( s -> msg_callback ) s -> msg_callback ( 0 , s -> version , SSL3_RT_ALERT , s -> rlayer . alert_fragment , 2 , s , s -> msg_callback_arg ) ;
 if ( s -> info_callback != NULL ) cb = s -> info_callback ;
 else if ( s -> ctx -> info_callback != NULL ) cb = s -> ctx -> info_callback ;
 if ( cb != NULL ) {
 j = ( alert_level << 8 ) | alert_descr ;
 cb ( s , SSL_CB_READ_ALERT , j ) ;
 }
 if ( alert_level == SSL3_AL_WARNING ) {
 s -> s3 -> warn_alert = alert_descr ;
 SSL3_RECORD_set_read ( rr ) ;
 if ( alert_descr == SSL_AD_CLOSE_NOTIFY ) {
 s -> shutdown |= SSL_RECEIVED_SHUTDOWN ;
 return ( 0 ) ;
 }
 else if ( alert_descr == SSL_AD_NO_RENEGOTIATION ) {
 al = SSL_AD_HANDSHAKE_FAILURE ;
 SSLerr ( SSL_F_SSL3_READ_BYTES , SSL_R_NO_RENEGOTIATION ) ;
 goto f_err ;
 }
 # ifdef SSL_AD_MISSING_SRP_USERNAME else if ( alert_descr == SSL_AD_MISSING_SRP_USERNAME ) return ( 0 ) ;
 # endif }
 else if ( alert_level == SSL3_AL_FATAL ) {
 char tmp [ 16 ] ;
 s -> rwstate = SSL_NOTHING ;
 s -> s3 -> fatal_alert = alert_descr ;
 SSLerr ( SSL_F_SSL3_READ_BYTES , SSL_AD_REASON_OFFSET + alert_descr ) ;
 BIO_snprintf ( tmp , sizeof tmp , "%d" , alert_descr ) ;
 ERR_add_error_data ( 2 , "SSL alert number " , tmp ) ;
 s -> shutdown |= SSL_RECEIVED_SHUTDOWN ;
 SSL3_RECORD_set_read ( rr ) ;
 SSL_CTX_remove_session ( s -> session_ctx , s -> session ) ;
 return ( 0 ) ;
 }
 else {
 al = SSL_AD_ILLEGAL_PARAMETER ;
 SSLerr ( SSL_F_SSL3_READ_BYTES , SSL_R_UNKNOWN_ALERT_TYPE ) ;
 goto f_err ;
 }
 goto start ;
 }
 if ( s -> shutdown & SSL_SENT_SHUTDOWN ) {
 s -> rwstate = SSL_NOTHING ;
 SSL3_RECORD_set_length ( rr , 0 ) ;
 SSL3_RECORD_set_read ( rr ) ;
 return ( 0 ) ;
 }
 if ( SSL3_RECORD_get_type ( rr ) == SSL3_RT_CHANGE_CIPHER_SPEC ) {
 al = SSL_AD_UNEXPECTED_MESSAGE ;
 SSLerr ( SSL_F_SSL3_READ_BYTES , SSL_R_CCS_RECEIVED_EARLY ) ;
 goto f_err ;
 }
 if ( ( s -> rlayer . handshake_fragment_len >= 4 ) && ! ossl_statem_get_in_handshake ( s ) ) {
 if ( SSL_is_init_finished ( s ) && ! ( s -> s3 -> flags & SSL3_FLAGS_NO_RENEGOTIATE_CIPHERS ) ) {
 ossl_statem_set_in_init ( s , 1 ) ;
 s -> renegotiate = 1 ;
 s -> new_session = 1 ;
 }
 i = s -> handshake_func ( s ) ;
 if ( i < 0 ) return ( i ) ;
 if ( i == 0 ) {
 SSLerr ( SSL_F_SSL3_READ_BYTES , SSL_R_SSL_HANDSHAKE_FAILURE ) ;
 return ( - 1 ) ;
 }
 if ( ! ( s -> mode & SSL_MODE_AUTO_RETRY ) ) {
 if ( SSL3_BUFFER_get_left ( rbuf ) == 0 ) {
 BIO * bio ;
 s -> rwstate = SSL_READING ;
 bio = SSL_get_rbio ( s ) ;
 BIO_clear_retry_flags ( bio ) ;
 BIO_set_retry_read ( bio ) ;
 return ( - 1 ) ;
 }
 }
 goto start ;
 }
 switch ( SSL3_RECORD_get_type ( rr ) ) {
 default : if ( s -> version >= TLS1_VERSION && s -> version <= TLS1_1_VERSION ) {
 SSL3_RECORD_set_length ( rr , 0 ) ;
 SSL3_RECORD_set_read ( rr ) ;
 goto start ;
 }
 al = SSL_AD_UNEXPECTED_MESSAGE ;
 SSLerr ( SSL_F_SSL3_READ_BYTES , SSL_R_UNEXPECTED_RECORD ) ;
 goto f_err ;
 case SSL3_RT_CHANGE_CIPHER_SPEC : case SSL3_RT_ALERT : case SSL3_RT_HANDSHAKE : al = SSL_AD_UNEXPECTED_MESSAGE ;
 SSLerr ( SSL_F_SSL3_READ_BYTES , ERR_R_INTERNAL_ERROR ) ;
 goto f_err ;
 case SSL3_RT_APPLICATION_DATA : if ( ossl_statem_app_data_allowed ( s ) ) {
 s -> s3 -> in_read_app_data = 2 ;
 return ( - 1 ) ;
 }
 else {
 al = SSL_AD_UNEXPECTED_MESSAGE ;
 SSLerr ( SSL_F_SSL3_READ_BYTES , SSL_R_UNEXPECTED_RECORD ) ;
 goto f_err ;
 }
 }
 f_err : ssl3_send_alert ( s , SSL3_AL_FATAL , al ) ;
 return ( - 1 ) ;
 }