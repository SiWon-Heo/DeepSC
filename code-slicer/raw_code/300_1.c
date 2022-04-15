bool ans_key_h ( connection_t * c ) {
 char from_name [ MAX_STRING_SIZE ] ;
 char to_name [ MAX_STRING_SIZE ] ;
 char key [ MAX_STRING_SIZE ] ;
 char address [ MAX_STRING_SIZE ] = "" ;
 char port [ MAX_STRING_SIZE ] = "" ;
 int cipher , digest , maclength , compression ;
 node_t * from , * to ;
 if ( sscanf ( c -> buffer , "%*d " MAX_STRING " " MAX_STRING " " MAX_STRING " %d %d %d %d " MAX_STRING " " MAX_STRING , from_name , to_name , key , & cipher , & digest , & maclength , & compression , address , port ) < 7 ) {
 logger ( LOG_ERR , "Got bad %s from %s (%s)" , "ANS_KEY" , c -> name , c -> hostname ) ;
 return false ;
 }
 if ( ! check_id ( from_name ) || ! check_id ( to_name ) ) {
 logger ( LOG_ERR , "Got bad %s from %s (%s): %s" , "ANS_KEY" , c -> name , c -> hostname , "invalid name" ) ;
 return false ;
 }
 from = lookup_node ( from_name ) ;
 if ( ! from ) {
 logger ( LOG_ERR , "Got %s from %s (%s) origin %s which does not exist in our connection list" , "ANS_KEY" , c -> name , c -> hostname , from_name ) ;
 return true ;
 }
 to = lookup_node ( to_name ) ;
 if ( ! to ) {
 logger ( LOG_ERR , "Got %s from %s (%s) destination %s which does not exist in our connection list" , "ANS_KEY" , c -> name , c -> hostname , to_name ) ;
 return true ;
 }
 if ( to != myself ) {
 if ( tunnelserver ) {
 return true ;
 }
 if ( ! to -> status . reachable ) {
 logger ( LOG_WARNING , "Got %s from %s (%s) destination %s which is not reachable" , "ANS_KEY" , c -> name , c -> hostname , to_name ) ;
 return true ;
 }
 if ( ! * address && from -> address . sa . sa_family != AF_UNSPEC && to -> minmtu ) {
 char * address , * port ;
 ifdebug ( PROTOCOL ) logger ( LOG_DEBUG , "Appending reflexive UDP address to ANS_KEY from %s to %s" , from -> name , to -> name ) ;
 sockaddr2str ( & from -> address , & address , & port ) ;
 send_request ( to -> nexthop -> connection , "%s %s %s" , c -> buffer , address , port ) ;
 free ( address ) ;
 free ( port ) ;
 return true ;
 }
 return send_request ( to -> nexthop -> connection , "%s" , c -> buffer ) ;
 }
 from -> status . validkey = false ;
 from -> outkey = xrealloc ( from -> outkey , strlen ( key ) / 2 ) ;
 from -> outkeylength = strlen ( key ) / 2 ;
 if ( ! hex2bin ( key , from -> outkey , from -> outkeylength ) ) {
 logger ( LOG_ERR , "Got bad %s from %s(%s): %s" , "ANS_KEY" , from -> name , from -> hostname , "invalid key" ) ;
 return true ;
 }
 if ( cipher ) {
 from -> outcipher = EVP_get_cipherbynid ( cipher ) ;
 if ( ! from -> outcipher ) {
 logger ( LOG_ERR , "Node %s (%s) uses unknown cipher!" , from -> name , from -> hostname ) ;
 return true ;
 }
 if ( from -> outkeylength != EVP_CIPHER_key_length ( from -> outcipher ) + EVP_CIPHER_iv_length ( from -> outcipher ) ) {
 logger ( LOG_ERR , "Node %s (%s) uses wrong keylength!" , from -> name , from -> hostname ) ;
 return true ;
 }
 }
 else {
 from -> outcipher = NULL ;
 }
 from -> outmaclength = maclength ;
 if ( digest ) {
 from -> outdigest = EVP_get_digestbynid ( digest ) ;
 if ( ! from -> outdigest ) {
 logger ( LOG_ERR , "Node %s (%s) uses unknown digest!" , from -> name , from -> hostname ) ;
 return true ;
 }
 if ( from -> outmaclength > EVP_MD_size ( from -> outdigest ) || from -> outmaclength < 0 ) {
 logger ( LOG_ERR , "Node %s (%s) uses bogus MAC length!" , from -> name , from -> hostname ) ;
 return true ;
 }
 }
 else {
 from -> outdigest = NULL ;
 }
 if ( compression < 0 || compression > 11 ) {
 logger ( LOG_ERR , "Node %s (%s) uses bogus compression level!" , from -> name , from -> hostname ) ;
 return true ;
 }
 from -> outcompression = compression ;
 if ( from -> outcipher ) if ( ! EVP_EncryptInit_ex ( from -> outctx , from -> outcipher , NULL , ( unsigned char * ) from -> outkey , ( unsigned char * ) from -> outkey + EVP_CIPHER_key_length ( from -> outcipher ) ) ) {
 logger ( LOG_ERR , "Error during initialisation of key from %s (%s): %s" , from -> name , from -> hostname , ERR_error_string ( ERR_get_error ( ) , NULL ) ) ;
 return true ;
 }
 from -> status . validkey = true ;
 from -> sent_seqno = 0 ;
 if ( * address && * port ) {
 ifdebug ( PROTOCOL ) logger ( LOG_DEBUG , "Using reflexive UDP address from %s: %s port %s" , from -> name , address , port ) ;
 sockaddr_t sa = str2sockaddr ( address , port ) ;
 update_node_udp ( from , & sa ) ;
 }
 if ( from -> options & OPTION_PMTU_DISCOVERY && ! from -> mtuevent ) {
 send_mtu_probe ( from ) ;
 }
 return true ;
 }