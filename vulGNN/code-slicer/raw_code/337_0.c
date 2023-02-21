static hm_fragment * dtls1_hm_fragment_new ( unsigned long frag_len , int reassembly ) {
 hm_fragment * frag = NULL ;
 unsigned char * buf = NULL ;
 unsigned char * bitmask = NULL ;
 frag = OPENSSL_malloc ( sizeof ( * frag ) ) ;
 if ( frag == NULL ) return NULL ;
 if ( frag_len ) {
 buf = OPENSSL_malloc ( frag_len ) ;
 if ( buf == NULL ) {
 OPENSSL_free ( frag ) ;
 return NULL ;
 }
 }
 frag -> fragment = buf ;
 if ( reassembly ) {
 bitmask = OPENSSL_zalloc ( RSMBLY_BITMASK_SIZE ( frag_len ) ) ;
 if ( bitmask == NULL ) {
 OPENSSL_free ( buf ) ;
 OPENSSL_free ( frag ) ;
 return NULL ;
 }
 }
 frag -> reassembly = bitmask ;
 return frag ;
 }