static void dissect_udvm_bytecode ( tvbuff_t * udvm_tvb , packet_info * pinfo , proto_tree * sigcomp_udvm_tree , guint start_address ) {
 guint instruction ;
 gint offset = 0 ;
 gint start_offset = 0 ;
 gint len ;
 gint n ;
 guint instruction_no = 0 ;
 guint16 value = 0 ;
 proto_item * item , * item2 ;
 guint UDVM_address = start_address ;
 gboolean is_memory_address ;
 guint16 msg_length = tvb_reported_length_remaining ( udvm_tvb , offset ) ;
 while ( msg_length > offset ) {
 instruction = tvb_get_guint8 ( udvm_tvb , offset ) ;
 instruction_no ++ ;
 UDVM_address = start_address + offset ;
 item = proto_tree_add_uint_format ( sigcomp_udvm_tree , hf_sigcomp_udvm_instruction , udvm_tvb , offset , 1 , instruction_no , "######### UDVM instruction %u at UDVM-address %u (0x%x) #########" , instruction_no , UDVM_address , UDVM_address ) ;
 PROTO_ITEM_SET_GENERATED ( item ) ;
 proto_tree_add_item ( sigcomp_udvm_tree , hf_sigcomp_udvm_instr , udvm_tvb , offset , 1 , ENC_BIG_ENDIAN ) ;
 offset ++ ;
 switch ( instruction ) {
 case SIGCOMP_INSTR_AND : offset = dissect_udvm_reference_operand ( udvm_tvb , sigcomp_udvm_tree , offset , & start_offset , & value ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_operand_1 , udvm_tvb , start_offset , len , value ) ;
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , FALSE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 if ( is_memory_address ) {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_operand_2_addr , udvm_tvb , start_offset , len , value ) ;
 }
 else {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_operand_2 , udvm_tvb , start_offset , len , value ) ;
 }
 break ;
 case SIGCOMP_INSTR_OR : offset = dissect_udvm_reference_operand ( udvm_tvb , sigcomp_udvm_tree , offset , & start_offset , & value ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_operand_1 , udvm_tvb , start_offset , len , value ) ;
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , FALSE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 if ( is_memory_address ) {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_operand_2_addr , udvm_tvb , start_offset , len , value ) ;
 }
 else {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_operand_2 , udvm_tvb , start_offset , len , value ) ;
 }
 break ;
 case SIGCOMP_INSTR_NOT : offset = dissect_udvm_reference_operand ( udvm_tvb , sigcomp_udvm_tree , offset , & start_offset , & value ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_operand_1 , udvm_tvb , start_offset , len , value ) ;
 break ;
 case SIGCOMP_INSTR_LSHIFT : offset = dissect_udvm_reference_operand ( udvm_tvb , sigcomp_udvm_tree , offset , & start_offset , & value ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_operand_1 , udvm_tvb , start_offset , len , value ) ;
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , FALSE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 if ( is_memory_address ) {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_operand_2_addr , udvm_tvb , start_offset , len , value ) ;
 }
 else {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_operand_2 , udvm_tvb , start_offset , len , value ) ;
 }
 break ;
 case SIGCOMP_INSTR_RSHIFT : offset = dissect_udvm_reference_operand ( udvm_tvb , sigcomp_udvm_tree , offset , & start_offset , & value ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_operand_1 , udvm_tvb , start_offset , len , value ) ;
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , FALSE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 if ( is_memory_address ) {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_operand_2_addr , udvm_tvb , start_offset , len , value ) ;
 }
 else {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_operand_2 , udvm_tvb , start_offset , len , value ) ;
 }
 break ;
 case SIGCOMP_INSTR_ADD : offset = dissect_udvm_reference_operand ( udvm_tvb , sigcomp_udvm_tree , offset , & start_offset , & value ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_operand_1 , udvm_tvb , start_offset , len , value ) ;
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , FALSE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 if ( is_memory_address ) {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_operand_2_addr , udvm_tvb , start_offset , len , value ) ;
 }
 else {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_operand_2 , udvm_tvb , start_offset , len , value ) ;
 }
 break ;
 case SIGCOMP_INSTR_SUBTRACT : offset = dissect_udvm_reference_operand ( udvm_tvb , sigcomp_udvm_tree , offset , & start_offset , & value ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_operand_1 , udvm_tvb , start_offset , len , value ) ;
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , FALSE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 if ( is_memory_address ) {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_operand_2_addr , udvm_tvb , start_offset , len , value ) ;
 }
 else {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_operand_2 , udvm_tvb , start_offset , len , value ) ;
 }
 break ;
 case SIGCOMP_INSTR_MULTIPLY : offset = dissect_udvm_reference_operand ( udvm_tvb , sigcomp_udvm_tree , offset , & start_offset , & value ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_operand_1 , udvm_tvb , start_offset , len , value ) ;
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , FALSE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 if ( is_memory_address ) {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_operand_2_addr , udvm_tvb , start_offset , len , value ) ;
 }
 else {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_operand_2 , udvm_tvb , start_offset , len , value ) ;
 }
 break ;
 case SIGCOMP_INSTR_DIVIDE : offset = dissect_udvm_reference_operand ( udvm_tvb , sigcomp_udvm_tree , offset , & start_offset , & value ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_operand_1 , udvm_tvb , start_offset , len , value ) ;
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , FALSE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 if ( is_memory_address ) {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_operand_2_addr , udvm_tvb , start_offset , len , value ) ;
 }
 else {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_operand_2 , udvm_tvb , start_offset , len , value ) ;
 }
 break ;
 case SIGCOMP_INSTR_REMAINDER : offset = dissect_udvm_reference_operand ( udvm_tvb , sigcomp_udvm_tree , offset , & start_offset , & value ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_operand_1 , udvm_tvb , start_offset , len , value ) ;
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , FALSE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 if ( is_memory_address ) {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_operand_2_addr , udvm_tvb , start_offset , len , value ) ;
 }
 else {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_operand_2 , udvm_tvb , start_offset , len , value ) ;
 }
 break ;
 case SIGCOMP_INSTR_SORT_ASCENDING : offset = offset + tvb_reported_length_remaining ( udvm_tvb , offset ) ;
 break ;
 case SIGCOMP_INSTR_SORT_DESCENDING : offset = offset + tvb_reported_length_remaining ( udvm_tvb , offset ) ;
 break ;
 case SIGCOMP_INSTR_SHA_1 : offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , FALSE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_position , udvm_tvb , start_offset , len , value ) ;
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , FALSE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 if ( is_memory_address ) {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_addr_length , udvm_tvb , start_offset , len , value ) ;
 }
 else {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_length , udvm_tvb , start_offset , len , value ) ;
 }
 offset = dissect_udvm_reference_operand ( udvm_tvb , sigcomp_udvm_tree , offset , & start_offset , & value ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_ref_dest , udvm_tvb , start_offset , len , value ) ;
 break ;
 case SIGCOMP_INSTR_LOAD : offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , TRUE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_address , udvm_tvb , start_offset , len , value ) ;
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , FALSE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 if ( is_memory_address ) {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_addr_value , udvm_tvb , start_offset , len , value ) ;
 }
 else {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_value , udvm_tvb , start_offset , len , value ) ;
 }
 break ;
 case SIGCOMP_INSTR_MULTILOAD : offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , TRUE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_address , udvm_tvb , start_offset , len , value ) ;
 offset = dissect_udvm_literal_operand ( udvm_tvb , sigcomp_udvm_tree , offset , & start_offset , & value ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_literal_num , udvm_tvb , start_offset , len , value ) ;
 n = value ;
 while ( n > 0 ) {
 n = n - 1 ;
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , FALSE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 if ( is_memory_address ) {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_addr_value , udvm_tvb , start_offset , len , value ) ;
 }
 else {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_value , udvm_tvb , start_offset , len , value ) ;
 }
 }
 break ;
 case SIGCOMP_INSTR_PUSH : offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , FALSE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 if ( is_memory_address ) {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_addr_value , udvm_tvb , start_offset , len , value ) ;
 }
 else {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_value , udvm_tvb , start_offset , len , value ) ;
 }
 break ;
 case SIGCOMP_INSTR_POP : offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , TRUE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_address , udvm_tvb , start_offset , len , value ) ;
 break ;
 case SIGCOMP_INSTR_COPY : offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , FALSE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_position , udvm_tvb , start_offset , len , value ) ;
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , FALSE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 if ( is_memory_address ) {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_addr_length , udvm_tvb , start_offset , len , value ) ;
 }
 else {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_length , udvm_tvb , start_offset , len , value ) ;
 }
 offset = dissect_udvm_reference_operand ( udvm_tvb , sigcomp_udvm_tree , offset , & start_offset , & value ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_ref_dest , udvm_tvb , start_offset , len , value ) ;
 break ;
 case SIGCOMP_INSTR_COPY_LITERAL : offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , FALSE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_position , udvm_tvb , start_offset , len , value ) ;
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , FALSE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 if ( is_memory_address ) {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_addr_length , udvm_tvb , start_offset , len , value ) ;
 }
 else {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_length , udvm_tvb , start_offset , len , value ) ;
 }
 offset = dissect_udvm_reference_operand ( udvm_tvb , sigcomp_udvm_tree , offset , & start_offset , & value ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_ref_dest , udvm_tvb , start_offset , len , value ) ;
 break ;
 case SIGCOMP_INSTR_COPY_OFFSET : offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , FALSE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 if ( is_memory_address ) {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_addr_offset , udvm_tvb , start_offset , len , value ) ;
 }
 else {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_offset , udvm_tvb , start_offset , len , value ) ;
 }
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , FALSE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 if ( is_memory_address ) {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_addr_length , udvm_tvb , start_offset , len , value ) ;
 }
 else {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_length , udvm_tvb , start_offset , len , value ) ;
 }
 offset = dissect_udvm_reference_operand ( udvm_tvb , sigcomp_udvm_tree , offset , & start_offset , & value ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_ref_dest , udvm_tvb , start_offset , len , value ) ;
 break ;
 case SIGCOMP_INSTR_MEMSET : offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , TRUE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_address , udvm_tvb , start_offset , len , value ) ;
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , FALSE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 if ( is_memory_address ) {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_addr_length , udvm_tvb , start_offset , len , value ) ;
 }
 else {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_length , udvm_tvb , start_offset , len , value ) ;
 }
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , FALSE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_start_value , udvm_tvb , start_offset , len , value ) ;
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , FALSE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_offset , udvm_tvb , start_offset , len , value ) ;
 break ;
 case SIGCOMP_INSTR_JUMP : offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , TRUE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 value = ( value + UDVM_address ) & 0xffff ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_at_address , udvm_tvb , start_offset , len , value ) ;
 break ;
 case SIGCOMP_INSTR_COMPARE : offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , FALSE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 if ( is_memory_address ) {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_addr_value , udvm_tvb , start_offset , len , value ) ;
 }
 else {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_value , udvm_tvb , start_offset , len , value ) ;
 }
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , FALSE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 if ( is_memory_address ) {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_addr_value , udvm_tvb , start_offset , len , value ) ;
 }
 else {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_value , udvm_tvb , start_offset , len , value ) ;
 }
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , TRUE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 value = ( value + UDVM_address ) & 0xffff ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_at_address , udvm_tvb , start_offset , len , value ) ;
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , TRUE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 value = ( value + UDVM_address ) & 0xffff ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_at_address , udvm_tvb , start_offset , len , value ) ;
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , TRUE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 value = ( value + UDVM_address ) & 0xffff ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_at_address , udvm_tvb , start_offset , len , value ) ;
 break ;
 case SIGCOMP_INSTR_CALL : offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , TRUE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 value = ( value + UDVM_address ) & 0xffff ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_at_address , udvm_tvb , start_offset , len , value ) ;
 break ;
 case SIGCOMP_INSTR_RETURN : break ;
 case SIGCOMP_INSTR_SWITCH : offset = dissect_udvm_literal_operand ( udvm_tvb , sigcomp_udvm_tree , offset , & start_offset , & value ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_literal_num , udvm_tvb , start_offset , len , value ) ;
 n = value ;
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , FALSE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 if ( is_memory_address ) {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_addr_j , udvm_tvb , start_offset , len , value ) ;
 }
 else {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_j , udvm_tvb , start_offset , len , value ) ;
 }
 while ( n > 0 ) {
 n = n - 1 ;
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , TRUE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 value = ( value + UDVM_address ) & 0xffff ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_at_address , udvm_tvb , start_offset , len , value ) ;
 }
 break ;
 case SIGCOMP_INSTR_CRC : offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , FALSE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 if ( is_memory_address ) {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_addr_value , udvm_tvb , start_offset , len , value ) ;
 }
 else {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_value , udvm_tvb , start_offset , len , value ) ;
 }
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , FALSE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_position , udvm_tvb , start_offset , len , value ) ;
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , FALSE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 if ( is_memory_address ) {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_addr_length , udvm_tvb , start_offset , len , value ) ;
 }
 else {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_length , udvm_tvb , start_offset , len , value ) ;
 }
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , TRUE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 value = ( value + UDVM_address ) & 0xffff ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_at_address , udvm_tvb , start_offset , len , value ) ;
 break ;
 case SIGCOMP_INSTR_INPUT_BYTES : offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , FALSE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 if ( is_memory_address ) {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_addr_length , udvm_tvb , start_offset , len , value ) ;
 }
 else {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_length , udvm_tvb , start_offset , len , value ) ;
 }
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , FALSE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 if ( is_memory_address ) {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_addr_destination , udvm_tvb , start_offset , len , value ) ;
 }
 else {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_destination , udvm_tvb , start_offset , len , value ) ;
 }
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , TRUE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 value = ( value + UDVM_address ) & 0xffff ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_at_address , udvm_tvb , start_offset , len , value ) ;
 break ;
 case SIGCOMP_INSTR_INPUT_BITS : offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , FALSE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 if ( is_memory_address ) {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_addr_length , udvm_tvb , start_offset , len , value ) ;
 }
 else {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_length , udvm_tvb , start_offset , len , value ) ;
 }
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , FALSE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 if ( is_memory_address ) {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_addr_destination , udvm_tvb , start_offset , len , value ) ;
 }
 else {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_destination , udvm_tvb , start_offset , len , value ) ;
 }
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , TRUE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 value = ( value + UDVM_address ) & 0xffff ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_at_address , udvm_tvb , start_offset , len , value ) ;
 break ;
 case SIGCOMP_INSTR_INPUT_HUFFMAN : offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , FALSE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 if ( is_memory_address ) {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_addr_destination , udvm_tvb , start_offset , len , value ) ;
 }
 else {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_destination , udvm_tvb , start_offset , len , value ) ;
 }
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , TRUE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 value = ( value + UDVM_address ) & 0xffff ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_at_address , udvm_tvb , start_offset , len , value ) ;
 offset = dissect_udvm_literal_operand ( udvm_tvb , sigcomp_udvm_tree , offset , & start_offset , & value ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_literal_num , udvm_tvb , start_offset , len , value ) ;
 n = value ;
 while ( n > 0 ) {
 n = n - 1 ;
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , FALSE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_bits , udvm_tvb , start_offset , len , value ) ;
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , FALSE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_lower_bound , udvm_tvb , start_offset , len , value ) ;
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , FALSE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_upper_bound , udvm_tvb , start_offset , len , value ) ;
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , FALSE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_uncompressed , udvm_tvb , start_offset , len , value ) ;
 }
 break ;
 case SIGCOMP_INSTR_STATE_ACCESS : offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , TRUE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_partial_identifier_start , udvm_tvb , start_offset , len , value ) ;
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , TRUE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_partial_identifier_length , udvm_tvb , start_offset , len , value ) ;
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , TRUE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_state_begin , udvm_tvb , start_offset , len , value ) ;
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , TRUE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 if ( is_memory_address ) {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_state_length_addr , udvm_tvb , start_offset , len , value ) ;
 }
 else {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_state_length , udvm_tvb , start_offset , len , value ) ;
 }
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , TRUE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 if ( is_memory_address ) {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_state_address_addr , udvm_tvb , start_offset , len , value ) ;
 }
 else {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_state_address , udvm_tvb , start_offset , len , value ) ;
 }
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , TRUE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_state_instr , udvm_tvb , start_offset , len , value ) ;
 break ;
 case SIGCOMP_INSTR_STATE_CREATE : offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , TRUE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 if ( is_memory_address ) {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_state_length_addr , udvm_tvb , start_offset , len , value ) ;
 }
 else {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_state_length , udvm_tvb , start_offset , len , value ) ;
 }
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , TRUE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 if ( is_memory_address ) {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_state_address_addr , udvm_tvb , start_offset , len , value ) ;
 }
 else {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_state_address , udvm_tvb , start_offset , len , value ) ;
 }
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , TRUE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_state_instr , udvm_tvb , start_offset , len , value ) ;
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , TRUE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_min_acc_len , udvm_tvb , start_offset , len , value ) ;
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , TRUE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_state_ret_pri , udvm_tvb , start_offset , len , value ) ;
 break ;
 case SIGCOMP_INSTR_STATE_FREE : offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , TRUE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_partial_identifier_start , udvm_tvb , start_offset , len , value ) ;
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , TRUE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_partial_identifier_length , udvm_tvb , start_offset , len , value ) ;
 break ;
 case SIGCOMP_INSTR_OUTPUT : offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , TRUE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 if ( is_memory_address ) {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_addr_output_start , udvm_tvb , start_offset , len , value ) ;
 }
 else {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_output_start , udvm_tvb , start_offset , len , value ) ;
 }
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , TRUE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 if ( is_memory_address ) {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_output_length_addr , udvm_tvb , start_offset , len , value ) ;
 }
 else {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_output_length , udvm_tvb , start_offset , len , value ) ;
 }
 break ;
 case SIGCOMP_INSTR_END_MESSAGE : if ( ( msg_length - 1 ) < offset ) {
 proto_tree_add_expert ( sigcomp_udvm_tree , pinfo , & ei_sigcomp_all_remaining_parameters_zero , udvm_tvb , 0 , - 1 ) ;
 return ;
 }
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , TRUE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_req_feedback_loc , udvm_tvb , start_offset , len , value ) ;
 if ( ( msg_length - 1 ) < offset ) {
 proto_tree_add_expert ( sigcomp_udvm_tree , pinfo , & ei_sigcomp_all_remaining_parameters_zero , udvm_tvb , offset - 1 , - 1 ) ;
 return ;
 }
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , TRUE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_ret_param_loc , udvm_tvb , start_offset , len , value ) ;
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , TRUE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 if ( is_memory_address ) {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_state_length_addr , udvm_tvb , start_offset , len , value ) ;
 }
 else {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_state_length , udvm_tvb , start_offset , len , value ) ;
 }
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , TRUE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 if ( is_memory_address ) {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_state_address_addr , udvm_tvb , start_offset , len , value ) ;
 }
 else {
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_state_address , udvm_tvb , start_offset , len , value ) ;
 }
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , TRUE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_state_instr , udvm_tvb , start_offset , len , value ) ;
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , TRUE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_min_acc_len , udvm_tvb , start_offset , len , value ) ;
 if ( tvb_reported_length_remaining ( udvm_tvb , offset ) != 0 ) {
 offset = dissect_udvm_multitype_operand ( udvm_tvb , sigcomp_udvm_tree , offset , TRUE , & start_offset , & value , & is_memory_address ) ;
 len = offset - start_offset ;
 proto_tree_add_uint ( sigcomp_udvm_tree , hf_udvm_state_ret_pri , udvm_tvb , start_offset , len , value ) ;
 }
 else {
 item2 = proto_tree_add_uint_format_value ( sigcomp_udvm_tree , hf_udvm_state_ret_pri , udvm_tvb , offset , 1 , 0 , "0 (Not in the uploaded code as UDVM buffer initialized to Zero" ) ;
 PROTO_ITEM_SET_GENERATED ( item2 ) ;
 }
 if ( tvb_reported_length_remaining ( udvm_tvb , offset ) != 0 ) {
 len = tvb_reported_length_remaining ( udvm_tvb , offset ) ;
 UDVM_address = start_address + offset ;
 proto_tree_add_bytes_format ( sigcomp_udvm_tree , hf_sigcomp_remaining_bytes , udvm_tvb , offset , len , NULL , "Remaining %u bytes starting at UDVM addr %u (0x%x)- State information ?" , len , UDVM_address , UDVM_address ) ;
 }
 offset = offset + tvb_reported_length_remaining ( udvm_tvb , offset ) ;
 break ;
 default : offset = offset + tvb_reported_length_remaining ( udvm_tvb , offset ) ;
 break ;
 }
 }
 return ;
 }