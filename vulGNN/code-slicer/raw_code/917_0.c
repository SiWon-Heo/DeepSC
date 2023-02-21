void proto_reg_handoff_sigcomp ( void ) {
 static dissector_handle_t sigcomp_handle ;
 static dissector_handle_t sigcomp_tcp_handle ;
 static gboolean Initialized = FALSE ;
 static guint udp_port1 ;
 static guint udp_port2 ;
 static guint tcp_port1 ;
 static guint tcp_port2 ;
 if ( ! Initialized ) {
 sigcomp_handle = find_dissector ( "sigcomp" ) ;
 sigcomp_tcp_handle = create_dissector_handle ( dissect_sigcomp_tcp , proto_sigcomp ) ;
 sip_handle = find_dissector_add_dependency ( "sip" , proto_sigcomp ) ;
 Initialized = TRUE ;
 }
 else {
 dissector_delete_uint ( "udp.port" , udp_port1 , sigcomp_handle ) ;
 dissector_delete_uint ( "udp.port" , udp_port2 , sigcomp_handle ) ;
 dissector_delete_uint ( "tcp.port" , tcp_port1 , sigcomp_tcp_handle ) ;
 dissector_delete_uint ( "tcp.port" , tcp_port2 , sigcomp_tcp_handle ) ;
 }
 udp_port1 = SigCompUDPPort1 ;
 udp_port2 = SigCompUDPPort2 ;
 tcp_port1 = SigCompTCPPort1 ;
 tcp_port2 = SigCompTCPPort2 ;
 dissector_add_uint ( "udp.port" , SigCompUDPPort1 , sigcomp_handle ) ;
 dissector_add_uint ( "udp.port" , SigCompUDPPort2 , sigcomp_handle ) ;
 dissector_add_uint ( "tcp.port" , SigCompTCPPort1 , sigcomp_tcp_handle ) ;
 dissector_add_uint ( "tcp.port" , SigCompTCPPort2 , sigcomp_tcp_handle ) ;
 }