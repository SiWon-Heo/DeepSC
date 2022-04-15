static int dissect_udvm_multitype_operand ( tvbuff_t * udvm_tvb , proto_tree * sigcomp_udvm_tree , gint offset , gboolean is_addr _U_ , gint * start_offset , guint16 * value , gboolean * is_memory_address ) {
 guint bytecode ;
 guint display_bytecode ;
 guint16 operand ;
 guint32 result ;
 guint test_bits ;
 * is_memory_address = FALSE ;
 bytecode = tvb_get_guint8 ( udvm_tvb , offset ) ;
 test_bits = ( bytecode & 0xc0 ) >> 6 ;
 switch ( test_bits ) {
 case 0 : display_bytecode = bytecode & 0xc0 ;
 if ( display_udvm_bytecode ) proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_multitype_bytecode , udvm_tvb , offset , 1 , display_bytecode ) ;
 operand = ( bytecode & 0x3f ) ;
 * value = operand ;
 * start_offset = offset ;
 offset ++ ;
 break ;
 case 1 : display_bytecode = bytecode & 0xc0 ;
 if ( display_udvm_bytecode ) proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_multitype_bytecode , udvm_tvb , offset , 1 , display_bytecode ) ;
 operand = ( bytecode & 0x3f ) * 2 ;
 * is_memory_address = TRUE ;
 * value = operand ;
 * start_offset = offset ;
 offset ++ ;
 break ;
 case 2 : test_bits = ( bytecode & 0xe0 ) >> 5 ;
 if ( test_bits == 5 ) {
 display_bytecode = bytecode & 0xe0 ;
 if ( display_udvm_bytecode ) proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_multitype_bytecode , udvm_tvb , offset , 1 , display_bytecode ) ;
 operand = tvb_get_ntohs ( udvm_tvb , offset ) & 0x1fff ;
 * value = operand ;
 * start_offset = offset ;
 offset = offset + 2 ;
 }
 else {
 test_bits = ( bytecode & 0xf0 ) >> 4 ;
 if ( test_bits == 9 ) {
 display_bytecode = bytecode & 0xf0 ;
 if ( display_udvm_bytecode ) proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_multitype_bytecode , udvm_tvb , offset , 1 , display_bytecode ) ;
 operand = ( tvb_get_ntohs ( udvm_tvb , offset ) & 0x0fff ) + 61440 ;
 * start_offset = offset ;
 * value = operand ;
 offset = offset + 2 ;
 }
 else {
 test_bits = ( bytecode & 0x08 ) >> 3 ;
 if ( test_bits == 1 ) {
 display_bytecode = bytecode & 0xf8 ;
 if ( display_udvm_bytecode ) proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_multitype_bytecode , udvm_tvb , offset , 1 , display_bytecode ) ;
 result = ( guint32 ) pow ( 2 , ( bytecode & 0x07 ) + 8 ) ;
 operand = result & 0xffff ;
 * start_offset = offset ;
 * value = operand ;
 offset ++ ;
 }
 else {
 test_bits = ( bytecode & 0x0e ) >> 1 ;
 if ( test_bits == 3 ) {
 display_bytecode = bytecode & 0xfe ;
 if ( display_udvm_bytecode ) proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_multitype_bytecode , udvm_tvb , offset , 1 , display_bytecode ) ;
 result = ( guint32 ) pow ( 2 , ( bytecode & 0x01 ) + 6 ) ;
 operand = result & 0xffff ;
 * start_offset = offset ;
 * value = operand ;
 offset ++ ;
 }
 else {
 display_bytecode = bytecode ;
 if ( display_udvm_bytecode ) proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_multitype_bytecode , udvm_tvb , offset , 1 , display_bytecode ) ;
 if ( ( bytecode & 0x01 ) == 1 ) * is_memory_address = TRUE ;
 offset ++ ;
 operand = tvb_get_ntohs ( udvm_tvb , offset ) ;
 * value = operand ;
 * start_offset = offset ;
 offset = offset + 2 ;
 }
 }
 }
 }
 break ;
 case 3 : test_bits = ( bytecode & 0x20 ) >> 5 ;
 if ( test_bits == 1 ) {
 display_bytecode = bytecode & 0xe0 ;
 if ( display_udvm_bytecode ) proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_multitype_bytecode , udvm_tvb , offset , 1 , display_bytecode ) ;
 operand = ( bytecode & 0x1f ) + 65504 ;
 * start_offset = offset ;
 * value = operand ;
 offset ++ ;
 }
 else {
 display_bytecode = bytecode & 0xe0 ;
 if ( display_udvm_bytecode ) proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_multitype_bytecode , udvm_tvb , offset , 1 , display_bytecode ) ;
 operand = ( tvb_get_ntohs ( udvm_tvb , offset ) & 0x1fff ) ;
 * is_memory_address = TRUE ;
 * start_offset = offset ;
 * value = operand ;
 offset = offset + 2 ;
 }
 default : break ;
 }
 return offset ;
 }