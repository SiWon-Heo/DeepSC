void dtls1_clear ( SSL * s ) {
 pqueue * buffered_messages ;
 pqueue * sent_messages ;
 unsigned int mtu ;
 unsigned int link_mtu ;
 DTLS_RECORD_LAYER_clear ( & s -> rlayer ) ;
 if ( s -> d1 ) {
 buffered_messages = s -> d1 -> buffered_messages ;
 sent_messages = s -> d1 -> sent_messages ;
 mtu = s -> d1 -> mtu ;
 link_mtu = s -> d1 -> link_mtu ;
 dtls1_clear_queues ( s ) ;
 memset ( s -> d1 , 0 , sizeof ( * s -> d1 ) ) ;
 if ( s -> server ) {
 s -> d1 -> cookie_len = sizeof ( s -> d1 -> cookie ) ;
 }
 if ( SSL_get_options ( s ) & SSL_OP_NO_QUERY_MTU ) {
 s -> d1 -> mtu = mtu ;
 s -> d1 -> link_mtu = link_mtu ;
 }
 s -> d1 -> buffered_messages = buffered_messages ;
 s -> d1 -> sent_messages = sent_messages ;
 }
 ssl3_clear ( s ) ;
 if ( s -> method -> version == DTLS_ANY_VERSION ) s -> version = DTLS_MAX_VERSION ;
 # ifndef OPENSSL_NO_DTLS1_METHOD else if ( s -> options & SSL_OP_CISCO_ANYCONNECT ) s -> client_version = s -> version = DTLS1_BAD_VER ;
 # endif else s -> version = s -> method -> version ;
 }