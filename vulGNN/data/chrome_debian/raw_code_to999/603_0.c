inline static HTTPKeepAlive is_header_keep_alive ( const HTTPVersion & http_version , const MIMEField * con_hdr ) {
 enum {
 CON_TOKEN_NONE = 0 , CON_TOKEN_KEEP_ALIVE , CON_TOKEN_CLOSE , }
 ;
 int con_token = CON_TOKEN_NONE ;
 HTTPKeepAlive keep_alive = HTTP_NO_KEEPALIVE ;
 if ( con_hdr ) {
 if ( con_hdr -> value_get_index ( "keep-alive" , 10 ) >= 0 ) con_token = CON_TOKEN_KEEP_ALIVE ;
 else if ( con_hdr -> value_get_index ( "close" , 5 ) >= 0 ) con_token = CON_TOKEN_CLOSE ;
 }
 if ( HTTPVersion ( 1 , 0 ) == http_version ) {
 keep_alive = ( con_token == CON_TOKEN_KEEP_ALIVE ) ? ( HTTP_KEEPALIVE ) : ( HTTP_NO_KEEPALIVE ) ;
 }
 else if ( HTTPVersion ( 1 , 1 ) == http_version ) {
 keep_alive = ( ( con_token == CON_TOKEN_KEEP_ALIVE ) || ( con_token == CON_TOKEN_NONE && HTTPVersion ( 1 , 1 ) == http_version ) ) ? ( HTTP_KEEPALIVE ) : ( HTTP_NO_KEEPALIVE ) ;
 }
 else {
 keep_alive = HTTP_NO_KEEPALIVE ;
 }
 return ( keep_alive ) ;
 }