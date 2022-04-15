static int dissect_sigcomp ( tvbuff_t * tvb , packet_info * pinfo , proto_tree * tree , void * data _U_ ) {
 proto_item * ti ;
 proto_tree * sigcomp_tree ;
 gint offset = 0 ;
 gint8 octet ;
 if ( pinfo -> ptype == PT_TCP ) return dissect_sigcomp_tcp ( tvb , pinfo , tree , NULL ) ;
 octet = tvb_get_guint8 ( tvb , offset ) ;
 if ( ( octet & 0xf8 ) != 0xf8 ) return 0 ;
 col_set_str ( pinfo -> cinfo , COL_PROTOCOL , "SIGCOMP" ) ;
 col_clear ( pinfo -> cinfo , COL_INFO ) ;
 top_tree = tree ;
 ti = proto_tree_add_item ( tree , proto_sigcomp , tvb , 0 , - 1 , ENC_NA ) ;
 sigcomp_tree = proto_item_add_subtree ( ti , ett_sigcomp ) ;
 return dissect_sigcomp_common ( tvb , pinfo , sigcomp_tree ) ;
 }