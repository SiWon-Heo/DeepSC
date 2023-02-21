int dtls1_shutdown ( SSL * s ) {
 int ret ;
 # ifndef OPENSSL_NO_SCTP BIO * wbio ;
 wbio = SSL_get_wbio ( s ) ;
 if ( wbio != NULL && BIO_dgram_is_sctp ( wbio ) && ! ( s -> shutdown & SSL_SENT_SHUTDOWN ) ) {
 ret = BIO_dgram_sctp_wait_for_dry ( wbio ) ;
 if ( ret < 0 ) return - 1 ;
 if ( ret == 0 ) BIO_ctrl ( SSL_get_wbio ( s ) , BIO_CTRL_DGRAM_SCTP_SAVE_SHUTDOWN , 1 , NULL ) ;
 }
 # endif ret = ssl3_shutdown ( s ) ;
 # ifndef OPENSSL_NO_SCTP BIO_ctrl ( SSL_get_wbio ( s ) , BIO_CTRL_DGRAM_SCTP_SAVE_SHUTDOWN , 0 , NULL ) ;
 # endif return ret ;
 }