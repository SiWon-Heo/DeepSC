int tls_construct_cert_status ( SSL * s ) {
 unsigned char * p ;
 if ( ! BUF_MEM_grow ( s -> init_buf , 8 + s -> tlsext_ocsp_resplen ) ) {
 ossl_statem_set_error ( s ) ;
 return 0 ;
 }
 p = ( unsigned char * ) s -> init_buf -> data ;
 * ( p ++ ) = SSL3_MT_CERTIFICATE_STATUS ;
 l2n3 ( s -> tlsext_ocsp_resplen + 4 , p ) ;
 * ( p ++ ) = s -> tlsext_status_type ;
 l2n3 ( s -> tlsext_ocsp_resplen , p ) ;
 memcpy ( p , s -> tlsext_ocsp_resp , s -> tlsext_ocsp_resplen ) ;
 s -> init_num = 8 + s -> tlsext_ocsp_resplen ;
 s -> init_off = 0 ;
 return 1 ;
 }