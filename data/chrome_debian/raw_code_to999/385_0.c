long dtls1_ctrl ( SSL * s , int cmd , long larg , void * parg ) {
 int ret = 0 ;
 switch ( cmd ) {
 case DTLS_CTRL_GET_TIMEOUT : if ( dtls1_get_timeout ( s , ( struct timeval * ) parg ) != NULL ) {
 ret = 1 ;
 }
 break ;
 case DTLS_CTRL_HANDLE_TIMEOUT : ret = dtls1_handle_timeout ( s ) ;
 break ;
 case DTLS_CTRL_SET_LINK_MTU : if ( larg < ( long ) dtls1_link_min_mtu ( ) ) return 0 ;
 s -> d1 -> link_mtu = larg ;
 return 1 ;
 case DTLS_CTRL_GET_LINK_MIN_MTU : return ( long ) dtls1_link_min_mtu ( ) ;
 case SSL_CTRL_SET_MTU : if ( larg < ( long ) dtls1_link_min_mtu ( ) - DTLS1_MAX_MTU_OVERHEAD ) return 0 ;
 s -> d1 -> mtu = larg ;
 return larg ;
 default : ret = ssl3_ctrl ( s , cmd , larg , parg ) ;
 break ;
 }
 return ( ret ) ;
 }