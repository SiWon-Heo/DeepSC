int tls_construct_change_cipher_spec ( SSL * s ) {
 unsigned char * p ;
 p = ( unsigned char * ) s -> init_buf -> data ;
 * p = SSL3_MT_CCS ;
 s -> init_num = 1 ;
 s -> init_off = 0 ;
 return 1 ;
 }