static int dissect_udvm_literal_operand ( tvbuff_t * udvm_tvb , proto_tree * sigcomp_udvm_tree , gint offset , gint * start_offset , guint16 * value ) {
 guint bytecode ;
 guint16 operand ;
 guint test_bits ;
 guint display_bytecode ;
 bytecode = tvb_get_guint8 ( udvm_tvb , offset ) ;
 test_bits = bytecode >> 7 ;
 if ( test_bits == 1 ) {
 test_bits = bytecode >> 6 ;
 if ( test_bits == 2 ) {
 display_bytecode = bytecode & 0xc0 ;
 if ( display_udvm_bytecode ) proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_literal_bytecode , udvm_tvb , offset , 1 , display_bytecode ) ;
 operand = tvb_get_ntohs ( udvm_tvb , offset ) & 0x3fff ;
 * value = operand ;
 * start_offset = offset ;
 offset = offset + 2 ;
 }
 else {
 display_bytecode = bytecode & 0xc0 ;
 if ( display_udvm_bytecode ) proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_literal_bytecode , udvm_tvb , offset , 1 , display_bytecode ) ;
 offset ++ ;
 operand = tvb_get_ntohs ( udvm_tvb , offset ) ;
 * value = operand ;
 * start_offset = offset ;
 offset = offset + 2 ;
 }
 }
 else {
 display_bytecode = bytecode & 0xc0 ;
 if ( display_udvm_bytecode ) proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_literal_bytecode , udvm_tvb , offset , 1 , display_bytecode ) ;
 operand = ( bytecode & 0x7f ) ;
 * value = operand ;
 * start_offset = offset ;
 offset ++ ;
 }
 return offset ;
 }