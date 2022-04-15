int tls_construct_certificate_request ( SSL * s ) {
 unsigned char * p , * d ;
 int i , j , nl , off , n ;
 STACK_OF ( X509_NAME ) * sk = NULL ;
 X509_NAME * name ;
 BUF_MEM * buf ;
 buf = s -> init_buf ;
 d = p = ssl_handshake_start ( s ) ;
 p ++ ;
 n = ssl3_get_req_cert_type ( s , p ) ;
 d [ 0 ] = n ;
 p += n ;
 n ++ ;
 if ( SSL_USE_SIGALGS ( s ) ) {
 const unsigned char * psigs ;
 unsigned char * etmp = p ;
 nl = tls12_get_psigalgs ( s , & psigs ) ;
 p += 2 ;
 nl = tls12_copy_sigalgs ( s , p , psigs , nl ) ;
 s2n ( nl , etmp ) ;
 p += nl ;
 n += nl + 2 ;
 }
 off = n ;
 p += 2 ;
 n += 2 ;
 sk = SSL_get_client_CA_list ( s ) ;
 nl = 0 ;
 if ( sk != NULL ) {
 for ( i = 0 ;
 i < sk_X509_NAME_num ( sk ) ;
 i ++ ) {
 name = sk_X509_NAME_value ( sk , i ) ;
 j = i2d_X509_NAME ( name , NULL ) ;
 if ( ! BUF_MEM_grow_clean ( buf , SSL_HM_HEADER_LENGTH ( s ) + n + j + 2 ) ) {
 SSLerr ( SSL_F_TLS_CONSTRUCT_CERTIFICATE_REQUEST , ERR_R_BUF_LIB ) ;
 goto err ;
 }
 p = ssl_handshake_start ( s ) + n ;
 s2n ( j , p ) ;
 i2d_X509_NAME ( name , & p ) ;
 n += 2 + j ;
 nl += 2 + j ;
 }
 }
 p = ssl_handshake_start ( s ) + off ;
 s2n ( nl , p ) ;
 if ( ! ssl_set_handshake_header ( s , SSL3_MT_CERTIFICATE_REQUEST , n ) ) {
 SSLerr ( SSL_F_TLS_CONSTRUCT_CERTIFICATE_REQUEST , ERR_R_INTERNAL_ERROR ) ;
 goto err ;
 }
 s -> s3 -> tmp . cert_request = 1 ;
 return 1 ;
 err : ossl_statem_set_error ( s ) ;
 return 0 ;
 }