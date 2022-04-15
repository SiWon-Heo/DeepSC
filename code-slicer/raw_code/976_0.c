static struct row_pairs * gtkui_connections_add ( struct conn_object * co , void * conn , struct row_pairs * * list ) {
 GtkTreeIter iter ;
 char flags [ 2 ] , src [ MAX_ASCII_ADDR_LEN ] , dst [ MAX_ASCII_ADDR_LEN ] ;
 char proto [ 4 ] , status [ 8 ] , ccodes [ 8 ] ;
 unsigned int src_port = 0 , dst_port = 0 , tx = 0 , rx = 0 ;
 struct row_pairs * row = NULL ;
 if ( ! list ) return ( NULL ) ;
 memset ( & flags , 0 , sizeof ( flags ) ) ;
 memset ( & proto , 0 , sizeof ( proto ) ) ;
 memset ( & src , 0 , sizeof ( src ) ) ;
 memset ( & dst , 0 , sizeof ( dst ) ) ;
 memset ( & status , 0 , sizeof ( status ) ) ;
 memset ( & ccodes , 0 , sizeof ( ccodes ) ) ;
 conntrack_flagstr ( co , flags , sizeof ( flags ) ) ;
 conntrack_statusstr ( co , status , sizeof ( status ) ) ;
 conntrack_protostr ( co , proto , sizeof ( proto ) ) ;
 conntrack_countrystr ( co , ccodes , sizeof ( ccodes ) ) ;
 ip_addr_ntoa ( & co -> L3_addr1 , src ) ;
 ip_addr_ntoa ( & co -> L3_addr2 , dst ) ;
 src_port = ntohs ( co -> L4_addr1 ) ;
 dst_port = ntohs ( co -> L4_addr2 ) ;
 tx = co -> tx ;
 rx = co -> rx ;
 gtk_list_store_append ( ls_conns , & iter ) ;
 gtk_list_store_set ( ls_conns , & iter , 0 , flags , 1 , src , 2 , src_port , 3 , "-" , 4 , dst , 5 , dst_port , 6 , proto , 7 , status , 8 , tx , 9 , rx , 10 , ccodes , 11 , conn , - 1 ) ;
 if ( ! * list ) {
 row = malloc ( sizeof ( struct row_pairs ) ) ;
 if ( row == NULL ) {
 USER_MSG ( "Failed create new connection row\n" ) ;
 DEBUG_MSG ( "gktui_connections_add: failed to allocate memory for a new row" ) ;
 }
 row -> prev = NULL ;
 }
 else {
 for ( row = * list ;
 row && row -> next ;
 row = row -> next ) ;
 row -> next = malloc ( sizeof ( struct row_pairs ) ) ;
 if ( row -> next == NULL ) {
 USER_MSG ( "Failed create new connection row\n" ) ;
 DEBUG_MSG ( "gktui_connections_add: failed to allocate memory for a new row" ) ;
 }
 row -> next -> prev = row ;
 row = row -> next ;
 }
 row -> conn = conn ;
 row -> iter = iter ;
 row -> next = NULL ;
 if ( ! * list ) * list = row ;
 return ( row ) ;
 }