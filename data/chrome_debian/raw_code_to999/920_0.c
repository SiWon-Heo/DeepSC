static int dissect_sigcomp_tcp ( tvbuff_t * tvb , packet_info * pinfo , proto_tree * tree , void * _data _U_ ) {
 proto_item * ti ;
 proto_tree * sigcomp_tree ;
 tvbuff_t * unescaped_tvb ;
 guint8 * buff ;
 int offset = 0 ;
 int length ;
 guint8 octet ;
 guint16 data ;
 int i ;
 int n ;
 gboolean end_off_message ;
 top_tree = tree ;
 data = tvb_get_ntohs ( tvb , offset ) ;
 if ( data == 0xffff ) {
 offset = offset + 2 ;
 octet = tvb_get_guint8 ( tvb , offset ) ;
 }
 else {
 octet = tvb_get_guint8 ( tvb , offset ) ;
 }
 if ( ( octet & 0xf8 ) != 0xf8 ) return offset ;
 length = tvb_reported_length_remaining ( tvb , offset ) ;
 for ( i = 0 ;
 i < ( length - 1 ) ;
 ++ i ) {
 data = tvb_get_ntohs ( tvb , offset + i ) ;
 if ( 0xffff == data ) break ;
 }
 if ( i >= ( length - 1 ) ) {
 if ( pinfo -> can_desegment || pinfo -> saved_can_desegment ) {
 pinfo -> desegment_offset = offset ;
 pinfo -> desegment_len = DESEGMENT_ONE_MORE_SEGMENT ;
 return - 1 ;
 }
 }
 col_set_str ( pinfo -> cinfo , COL_PROTOCOL , "SIGCOMP" ) ;
 col_clear ( pinfo -> cinfo , COL_INFO ) ;
 length = tvb_reported_length ( tvb ) ;
 try_again : ti = proto_tree_add_item ( tree , proto_sigcomp , tvb , 0 , - 1 , ENC_NA ) ;
 sigcomp_tree = proto_item_add_subtree ( ti , ett_sigcomp ) ;
 i = 0 ;
 end_off_message = FALSE ;
 buff = ( guint8 * ) wmem_alloc ( pinfo -> pool , length - offset ) ;
 if ( udvm_print_detail_level > 2 ) proto_tree_add_item ( sigcomp_tree , hf_sigcomp_starting_to_remove_escape_digits , tvb , offset , - 1 , ENC_NA ) ;
 while ( ( offset < length ) && ( end_off_message == FALSE ) ) {
 octet = tvb_get_guint8 ( tvb , offset ) ;
 if ( octet == 0xff ) {
 if ( offset + 1 >= length ) {
 offset ++ ;
 continue ;
 }
 if ( udvm_print_detail_level > 2 ) proto_tree_add_none_format ( sigcomp_tree , hf_sigcomp_escape_digit_found , tvb , offset , 2 , " Escape digit found (0xFF)" ) ;
 octet = tvb_get_guint8 ( tvb , offset + 1 ) ;
 if ( octet == 0 ) {
 buff [ i ] = 0xff ;
 offset = offset + 2 ;
 i ++ ;
 continue ;
 }
 if ( ( octet > 0x7f ) && ( octet < 0xff ) ) {
 if ( udvm_print_detail_level > 2 ) proto_tree_add_none_format ( sigcomp_tree , hf_sigcomp_illegal_escape_code , tvb , offset , 2 , " Illegal escape code" ) ;
 offset += tvb_captured_length_remaining ( tvb , offset ) ;
 return offset ;
 }
 if ( octet == 0xff ) {
 if ( udvm_print_detail_level > 2 ) proto_tree_add_none_format ( sigcomp_tree , hf_sigcomp_end_of_sigcomp_message_indication_found , tvb , offset , 2 , " End of SigComp message indication found (0xFFFF)" ) ;
 end_off_message = TRUE ;
 offset = offset + 2 ;
 continue ;
 }
 buff [ i ] = 0xff ;
 if ( udvm_print_detail_level > 2 ) proto_tree_add_uint_format ( sigcomp_tree , hf_sigcomp_addr_value , tvb , offset , 1 , buff [ i ] , " Addr: %u tvb value(0x%0x) " , i , buff [ i ] ) ;
 i ++ ;
 offset = offset + 2 ;
 if ( udvm_print_detail_level > 2 ) proto_tree_add_bytes_format ( sigcomp_tree , hf_sigcomp_copying_bytes_literally , tvb , offset , octet , NULL , " Copying %u bytes literally" , octet ) ;
 if ( offset + octet >= length ) octet = length - offset ;
 for ( n = 0 ;
 n < octet ;
 n ++ ) {
 buff [ i ] = tvb_get_guint8 ( tvb , offset ) ;
 if ( udvm_print_detail_level > 2 ) proto_tree_add_uint_format ( sigcomp_tree , hf_sigcomp_addr_value , tvb , offset , 1 , buff [ i ] , " Addr: %u tvb value(0x%0x) " , i , buff [ i ] ) ;
 i ++ ;
 offset ++ ;
 }
 continue ;
 }
 buff [ i ] = octet ;
 if ( udvm_print_detail_level > 2 ) proto_tree_add_uint_format ( sigcomp_tree , hf_sigcomp_addr_value , tvb , offset , 1 , buff [ i ] , " Addr: %u tvb value(0x%0x) " , i , buff [ i ] ) ;
 i ++ ;
 offset ++ ;
 }
 unescaped_tvb = tvb_new_child_real_data ( tvb , buff , i , i ) ;
 add_new_data_source ( pinfo , unescaped_tvb , "Unescaped Data handed to the SigComp dissector" ) ;
 proto_tree_add_item ( sigcomp_tree , hf_sigcomp_data_for_sigcomp_dissector , unescaped_tvb , 0 , - 1 , ENC_NA ) ;
 if ( end_off_message == TRUE ) {
 dissect_sigcomp_common ( unescaped_tvb , pinfo , sigcomp_tree ) ;
 }
 else {
 proto_tree_add_expert ( sigcomp_tree , pinfo , & ei_sigcomp_tcp_fragment , unescaped_tvb , 0 , - 1 ) ;
 }
 if ( offset < length ) {
 goto try_again ;
 }
 return offset ;
 }