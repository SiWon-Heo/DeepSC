static tvbuff_t * decompress_sigcomp_message ( tvbuff_t * bytecode_tvb , tvbuff_t * message_tvb , packet_info * pinfo , proto_tree * udvm_tree , gint udvm_mem_dest , gint print_flags , gint hf_id , gint header_len , gint byte_code_state_len , gint byte_code_id_len , gint udvm_start_ip ) {
 tvbuff_t * decomp_tvb ;
 guint8 * buff = ( guint8 * ) wmem_alloc0 ( wmem_packet_scope ( ) , UDVM_MEMORY_SIZE ) ;
 char string [ 2 ] ;
 guint8 * out_buff ;
 guint32 i = 0 ;
 guint16 n = 0 ;
 guint16 m = 0 ;
 guint16 x ;
 guint k = 0 ;
 guint16 H ;
 guint16 oldH ;
 guint offset = 0 ;
 guint start_offset ;
 guint result_dest ;
 guint code_length = 0 ;
 guint8 current_instruction ;
 guint current_address ;
 guint operand_address ;
 guint input_address ;
 guint16 output_address = 0 ;
 guint next_operand_address ;
 guint8 octet ;
 guint8 msb ;
 guint8 lsb ;
 guint16 byte_copy_right ;
 guint16 byte_copy_left ;
 guint16 input_bit_order ;
 guint16 stack_location ;
 guint16 stack_fill ;
 guint16 result ;
 guint msg_end = tvb_reported_length_remaining ( message_tvb , 0 ) ;
 guint16 result_code = 0 ;
 guint16 old_input_bit_order = 0 ;
 guint16 remaining_bits = 0 ;
 guint16 input_bits = 0 ;
 guint8 bit_order = 0 ;
 gboolean outside_huffman_boundaries = TRUE ;
 gboolean print_in_loop = FALSE ;
 guint16 instruction_address ;
 guint8 no_of_state_create = 0 ;
 guint16 state_length_buff [ 5 ] ;
 guint16 state_address_buff [ 5 ] ;
 guint16 state_instruction_buff [ 5 ] ;
 guint16 state_minimum_access_length_buff [ 5 ] ;
 guint32 used_udvm_cycles = 0 ;
 guint cycles_per_bit ;
 guint maximum_UDVM_cycles ;
 guint8 * sha1buff ;
 unsigned char sha1_digest_buf [ STATE_BUFFER_SIZE ] ;
 sha1_context ctx ;
 proto_item * addr_item = NULL , * ti = NULL ;
 guint16 length ;
 guint16 at_address ;
 guint16 destination ;
 guint16 addr ;
 guint16 value ;
 guint16 p_id_start ;
 guint16 p_id_length ;
 guint16 state_begin ;
 guint16 state_length ;
 guint16 state_address ;
 guint16 state_instruction ;
 guint16 operand_1 ;
 guint16 operand_2 ;
 guint16 value_1 ;
 guint16 value_2 ;
 guint16 at_address_1 ;
 guint16 at_address_2 ;
 guint16 at_address_3 ;
 guint16 j ;
 guint16 bits_n ;
 guint16 lower_bound_n ;
 guint16 upper_bound_n ;
 guint16 uncompressed_n ;
 guint16 position ;
 guint16 ref_destination ;
 guint16 multy_offset ;
 guint16 output_start ;
 guint16 output_length ;
 guint16 minimum_access_length ;
 guint16 state_retention_priority ;
 guint16 requested_feedback_location ;
 guint16 returned_parameters_location ;
 guint16 start_value ;
 gboolean print_level_1 = FALSE ;
 gboolean print_level_2 = FALSE ;
 gboolean print_level_3 = FALSE ;
 gint show_instr_detail_level = 0 ;
 switch ( print_flags ) {
 case 0 : break ;
 case 1 : print_level_1 = TRUE ;
 show_instr_detail_level = 1 ;
 break ;
 case 2 : print_level_1 = TRUE ;
 print_level_2 = TRUE ;
 show_instr_detail_level = 1 ;
 break ;
 case 3 : print_level_1 = TRUE ;
 print_level_2 = TRUE ;
 print_level_3 = TRUE ;
 show_instr_detail_level = 2 ;
 break ;
 default : print_level_1 = TRUE ;
 show_instr_detail_level = 1 ;
 break ;
 }
 buff [ 0 ] = ( UDVM_MEMORY_SIZE >> 8 ) & 0x00FF ;
 buff [ 1 ] = UDVM_MEMORY_SIZE & 0x00FF ;
 buff [ 2 ] = 0 ;
 buff [ 3 ] = 16 ;
 buff [ 4 ] = 0 ;
 buff [ 5 ] = 1 ;
 buff [ 6 ] = ( byte_code_id_len >> 8 ) & 0x00FF ;
 buff [ 7 ] = byte_code_id_len & 0x00FF ;
 buff [ 8 ] = ( byte_code_state_len >> 8 ) & 0x00FF ;
 buff [ 9 ] = byte_code_state_len & 0x00FF ;
 code_length = tvb_reported_length_remaining ( bytecode_tvb , 0 ) ;
 cycles_per_bit = buff [ 2 ] << 8 ;
 cycles_per_bit = cycles_per_bit | buff [ 3 ] ;
 maximum_UDVM_cycles = ( ( 8 * ( header_len + msg_end ) ) + 1000 ) * cycles_per_bit ;
 proto_tree_add_uint ( udvm_tree , hf_sigcomp_message_length , bytecode_tvb , offset , 1 , msg_end ) ;
 proto_tree_add_uint ( udvm_tree , hf_sigcomp_byte_code_length , bytecode_tvb , offset , 1 , code_length ) ;
 proto_tree_add_uint ( udvm_tree , hf_sigcomp_max_udvm_cycles , bytecode_tvb , offset , 1 , maximum_UDVM_cycles ) ;
 i = udvm_mem_dest ;
 if ( print_level_3 ) proto_tree_add_uint ( udvm_tree , hf_sigcomp_load_bytecode_into_udvm_start , bytecode_tvb , offset , 1 , i ) ;
 while ( code_length > offset && i < UDVM_MEMORY_SIZE ) {
 buff [ i ] = tvb_get_guint8 ( bytecode_tvb , offset ) ;
 if ( print_level_3 ) proto_tree_add_uint_format ( udvm_tree , hf_sigcomp_instruction_code , bytecode_tvb , offset , 1 , buff [ i ] , " Addr: %u Instruction code(0x%02x) " , i , buff [ i ] ) ;
 i ++ ;
 offset ++ ;
 }
 current_address = udvm_start_ip ;
 input_address = 0 ;
 proto_tree_add_uint_format ( udvm_tree , hf_sigcomp_udvm_execution_stated , bytecode_tvb , offset , 1 , current_address , "UDVM EXECUTION STARTED at Address: %u Message size %u" , current_address , msg_end ) ;
 out_buff = ( guint8 * ) g_malloc ( UDVM_MEMORY_SIZE ) ;
 offset = 0 ;
 execute_next_instruction : if ( used_udvm_cycles > maximum_UDVM_cycles ) {
 result_code = 15 ;
 goto decompression_failure ;
 }
 used_udvm_cycles ++ ;
 current_instruction = buff [ current_address & 0xffff ] ;
 if ( show_instr_detail_level == 2 ) {
 addr_item = proto_tree_add_uint_format ( udvm_tree , hf_sigcomp_current_instruction , bytecode_tvb , offset , 1 , current_instruction , "Addr: %u ## %s(%d)" , current_address , val_to_str_ext_const ( current_instruction , & udvm_instruction_code_vals_ext , "INVALID INSTRUCTION" ) , current_instruction ) ;
 }
 offset ++ ;
 switch ( current_instruction ) {
 case SIGCOMP_INSTR_DECOMPRESSION_FAILURE : if ( result_code == 0 ) result_code = 9 ;
 proto_tree_add_uint_format ( udvm_tree , hf_sigcomp_decompression_failure , NULL , 0 , 0 , current_address , "Addr: %u ## DECOMPRESSION-FAILURE(0)" , current_address ) ;
 proto_tree_add_uint ( udvm_tree , hf_sigcomp_wireshark_udvm_diagnostic , NULL , 0 , 0 , result_code ) ;
 if ( output_address > 0 ) {
 decomp_tvb = tvb_new_child_real_data ( message_tvb , out_buff , output_address , output_address ) ;
 tvb_set_free_cb ( decomp_tvb , g_free ) ;
 add_new_data_source ( pinfo , decomp_tvb , "Decompressed SigComp message(Incomplete)" ) ;
 proto_tree_add_expert ( udvm_tree , pinfo , & ei_sigcomp_sigcomp_message_decompression_failure , decomp_tvb , 0 , - 1 ) ;
 return decomp_tvb ;
 }
 g_free ( out_buff ) ;
 return NULL ;
 break ;
 case SIGCOMP_INSTR_AND : if ( show_instr_detail_level == 2 ) {
 proto_item_append_text ( addr_item , " (operand_1, operand_2)" ) ;
 }
 start_offset = offset ;
 operand_address = current_address + 1 ;
 next_operand_address = dissect_udvm_reference_operand_memory ( buff , operand_address , & operand_1 , & result_dest ) ;
 if ( next_operand_address < operand_address ) goto decompression_failure ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_operand_1 , bytecode_tvb , offset , ( next_operand_address - operand_address ) , operand_1 , "Addr: %u operand_1 %u" , operand_address , operand_1 ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & operand_2 ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_operand_2 , bytecode_tvb , offset , ( next_operand_address - operand_address ) , operand_2 , "Addr: %u operand_2 %u" , operand_address , operand_2 ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 if ( show_instr_detail_level == 1 ) {
 proto_tree_add_none_format ( udvm_tree , hf_sigcomp_decompress_instruction , bytecode_tvb , start_offset , offset - start_offset , "Addr: %u ## AND (operand_1=%u, operand_2=%u)" , current_address , operand_1 , operand_2 ) ;
 }
 result = operand_1 & operand_2 ;
 lsb = result & 0xff ;
 msb = result >> 8 ;
 buff [ result_dest ] = msb ;
 buff [ ( result_dest + 1 ) & 0xffff ] = lsb ;
 if ( print_level_1 ) {
 proto_tree_add_none_format ( udvm_tree , hf_sigcomp_loading_result , bytecode_tvb , 0 , - 1 , " Loading result %u at %u" , result , result_dest ) ;
 }
 current_address = next_operand_address ;
 goto execute_next_instruction ;
 break ;
 case SIGCOMP_INSTR_OR : if ( show_instr_detail_level == 2 ) {
 proto_item_append_text ( addr_item , " (operand_1, operand_2)" ) ;
 }
 start_offset = offset ;
 operand_address = current_address + 1 ;
 next_operand_address = dissect_udvm_reference_operand_memory ( buff , operand_address , & operand_1 , & result_dest ) ;
 if ( next_operand_address < operand_address ) goto decompression_failure ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_operand_1 , bytecode_tvb , offset , ( next_operand_address - operand_address ) , operand_1 , "Addr: %u operand_1 %u" , operand_address , operand_1 ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & operand_2 ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_operand_2 , bytecode_tvb , offset , ( next_operand_address - operand_address ) , operand_2 , "Addr: %u operand_2 %u" , operand_address , operand_2 ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 if ( show_instr_detail_level == 1 ) {
 proto_tree_add_none_format ( udvm_tree , hf_sigcomp_decompress_instruction , bytecode_tvb , start_offset , offset - start_offset , "Addr: %u ## OR (operand_1=%u, operand_2=%u)" , current_address , operand_1 , operand_2 ) ;
 }
 result = operand_1 | operand_2 ;
 lsb = result & 0xff ;
 msb = result >> 8 ;
 buff [ result_dest ] = msb ;
 buff [ ( result_dest + 1 ) & 0xffff ] = lsb ;
 if ( print_level_1 ) {
 proto_tree_add_none_format ( udvm_tree , hf_sigcomp_loading_result , bytecode_tvb , 0 , - 1 , " Loading result %u at %u" , result , result_dest ) ;
 }
 current_address = next_operand_address ;
 goto execute_next_instruction ;
 break ;
 case SIGCOMP_INSTR_NOT : if ( show_instr_detail_level == 2 ) {
 proto_item_append_text ( addr_item , " ($operand_1)" ) ;
 }
 start_offset = offset ;
 operand_address = current_address + 1 ;
 next_operand_address = dissect_udvm_reference_operand_memory ( buff , operand_address , & operand_1 , & result_dest ) ;
 if ( next_operand_address < operand_address ) goto decompression_failure ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_operand_1 , bytecode_tvb , offset , ( next_operand_address - operand_address ) , operand_1 , "Addr: %u operand_1 %u" , operand_address , operand_1 ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 if ( show_instr_detail_level == 1 ) {
 proto_tree_add_none_format ( udvm_tree , hf_sigcomp_decompress_instruction , bytecode_tvb , start_offset , offset - start_offset , "Addr: %u ## NOT (operand_1=%u)" , current_address , operand_1 ) ;
 }
 result = operand_1 ^ 0xffff ;
 lsb = result & 0xff ;
 msb = result >> 8 ;
 buff [ result_dest ] = msb ;
 buff [ ( result_dest + 1 ) & 0xffff ] = lsb ;
 if ( print_level_1 ) {
 proto_tree_add_none_format ( udvm_tree , hf_sigcomp_loading_result , bytecode_tvb , 0 , - 1 , " Loading result %u at %u" , result , result_dest ) ;
 }
 current_address = next_operand_address ;
 goto execute_next_instruction ;
 break ;
 case SIGCOMP_INSTR_LSHIFT : if ( show_instr_detail_level == 2 ) {
 proto_item_append_text ( addr_item , " ($operand_1, operand_2)" ) ;
 }
 start_offset = offset ;
 operand_address = current_address + 1 ;
 next_operand_address = dissect_udvm_reference_operand_memory ( buff , operand_address , & operand_1 , & result_dest ) ;
 if ( next_operand_address < operand_address ) goto decompression_failure ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_operand_1 , bytecode_tvb , offset , ( next_operand_address - operand_address ) , operand_1 , "Addr: %u operand_1 %u" , operand_address , operand_1 ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & operand_2 ) ;
 if ( show_instr_detail_level == 2 ) {
 ti = proto_tree_add_uint_format ( udvm_tree , hf_udvm_operand_2 , bytecode_tvb , offset , ( next_operand_address - operand_address ) , operand_2 , "Addr: %u operand_2 %u" , operand_address , operand_2 ) ;
 }
 if ( operand_2 > 15 ) {
 expert_add_info ( pinfo , ti , & ei_sigcomp_invalid_shift_value ) ;
 break ;
 }
 offset += ( next_operand_address - operand_address ) ;
 if ( show_instr_detail_level == 1 ) {
 proto_tree_add_none_format ( udvm_tree , hf_sigcomp_decompress_instruction , bytecode_tvb , start_offset , offset - start_offset , "Addr: %u ## LSHIFT (operand_1=%u, operand_2=%u)" , current_address , operand_1 , operand_2 ) ;
 }
 result = operand_1 << operand_2 ;
 lsb = result & 0xff ;
 msb = result >> 8 ;
 buff [ result_dest ] = msb ;
 buff [ ( result_dest + 1 ) & 0xffff ] = lsb ;
 if ( print_level_1 ) {
 proto_tree_add_none_format ( udvm_tree , hf_sigcomp_loading_result , bytecode_tvb , 0 , - 1 , " Loading result %u at %u" , result , result_dest ) ;
 }
 current_address = next_operand_address ;
 goto execute_next_instruction ;
 break ;
 case SIGCOMP_INSTR_RSHIFT : if ( show_instr_detail_level == 2 ) {
 proto_item_append_text ( addr_item , " (operand_1, operand_2)" ) ;
 }
 start_offset = offset ;
 operand_address = current_address + 1 ;
 next_operand_address = dissect_udvm_reference_operand_memory ( buff , operand_address , & operand_1 , & result_dest ) ;
 if ( next_operand_address < operand_address ) goto decompression_failure ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_operand_1 , bytecode_tvb , offset , ( next_operand_address - operand_address ) , operand_1 , "Addr: %u operand_1 %u" , operand_address , operand_1 ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & operand_2 ) ;
 if ( show_instr_detail_level == 2 ) {
 ti = proto_tree_add_uint_format ( udvm_tree , hf_udvm_operand_2 , bytecode_tvb , offset , ( next_operand_address - operand_address ) , operand_2 , "Addr: %u operand_2 %u" , operand_address , operand_2 ) ;
 }
 if ( operand_2 > 15 ) {
 expert_add_info ( pinfo , ti , & ei_sigcomp_invalid_shift_value ) ;
 break ;
 }
 offset += ( next_operand_address - operand_address ) ;
 if ( show_instr_detail_level == 1 ) {
 proto_tree_add_none_format ( udvm_tree , hf_sigcomp_decompress_instruction , bytecode_tvb , start_offset , offset - start_offset , "Addr: %u ## RSHIFT (operand_1=%u, operand_2=%u)" , current_address , operand_1 , operand_2 ) ;
 }
 result = operand_1 >> operand_2 ;
 lsb = result & 0xff ;
 msb = result >> 8 ;
 buff [ result_dest ] = msb ;
 buff [ ( result_dest + 1 ) & 0xffff ] = lsb ;
 if ( print_level_1 ) {
 proto_tree_add_none_format ( udvm_tree , hf_sigcomp_loading_result , bytecode_tvb , 0 , - 1 , " Loading result %u at %u" , result , result_dest ) ;
 }
 current_address = next_operand_address ;
 goto execute_next_instruction ;
 break ;
 case SIGCOMP_INSTR_ADD : if ( show_instr_detail_level == 2 ) {
 proto_item_append_text ( addr_item , " (operand_1, operand_2)" ) ;
 }
 start_offset = offset ;
 operand_address = current_address + 1 ;
 next_operand_address = dissect_udvm_reference_operand_memory ( buff , operand_address , & operand_1 , & result_dest ) ;
 if ( next_operand_address < operand_address ) goto decompression_failure ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_operand_1 , bytecode_tvb , offset , ( next_operand_address - operand_address ) , operand_1 , "Addr: %u operand_1 %u" , operand_address , operand_1 ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & operand_2 ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_operand_2 , bytecode_tvb , offset , ( next_operand_address - operand_address ) , operand_2 , "Addr: %u operand_2 %u" , operand_address , operand_2 ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 if ( show_instr_detail_level == 1 ) {
 proto_tree_add_none_format ( udvm_tree , hf_sigcomp_decompress_instruction , bytecode_tvb , start_offset , offset - start_offset , "Addr: %u ## ADD (operand_1=%u, operand_2=%u)" , current_address , operand_1 , operand_2 ) ;
 }
 result = operand_1 + operand_2 ;
 lsb = result & 0xff ;
 msb = result >> 8 ;
 buff [ result_dest ] = msb ;
 buff [ ( result_dest + 1 ) & 0xffff ] = lsb ;
 if ( print_level_1 ) {
 proto_tree_add_none_format ( udvm_tree , hf_sigcomp_loading_result , bytecode_tvb , 0 , - 1 , " Loading result %u at %u" , result , result_dest ) ;
 }
 current_address = next_operand_address ;
 goto execute_next_instruction ;
 case SIGCOMP_INSTR_SUBTRACT : if ( show_instr_detail_level == 2 ) {
 proto_item_append_text ( addr_item , " (operand_1, operand_2)" ) ;
 }
 start_offset = offset ;
 operand_address = current_address + 1 ;
 next_operand_address = dissect_udvm_reference_operand_memory ( buff , operand_address , & operand_1 , & result_dest ) ;
 if ( next_operand_address < operand_address ) goto decompression_failure ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_operand_1 , bytecode_tvb , offset , ( next_operand_address - operand_address ) , operand_1 , "Addr: %u operand_1 %u" , operand_address , operand_1 ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & operand_2 ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_operand_2 , bytecode_tvb , offset , ( next_operand_address - operand_address ) , operand_2 , "Addr: %u operand_2 %u" , operand_address , operand_2 ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 if ( show_instr_detail_level == 1 ) {
 proto_tree_add_none_format ( udvm_tree , hf_sigcomp_decompress_instruction , bytecode_tvb , start_offset , offset - start_offset , "Addr: %u ## SUBTRACT (operand_1=%u, operand_2=%u)" , current_address , operand_1 , operand_2 ) ;
 }
 result = operand_1 - operand_2 ;
 lsb = result & 0xff ;
 msb = result >> 8 ;
 buff [ result_dest ] = msb ;
 buff [ ( result_dest + 1 ) & 0xffff ] = lsb ;
 if ( print_level_1 ) {
 proto_tree_add_none_format ( udvm_tree , hf_sigcomp_loading_result , bytecode_tvb , 0 , - 1 , " Loading result %u at %u" , result , result_dest ) ;
 }
 current_address = next_operand_address ;
 goto execute_next_instruction ;
 break ;
 case SIGCOMP_INSTR_MULTIPLY : if ( show_instr_detail_level == 2 ) {
 proto_item_append_text ( addr_item , " (operand_1, operand_2)" ) ;
 }
 start_offset = offset ;
 operand_address = current_address + 1 ;
 next_operand_address = dissect_udvm_reference_operand_memory ( buff , operand_address , & operand_1 , & result_dest ) ;
 if ( next_operand_address < operand_address ) goto decompression_failure ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_operand_1 , bytecode_tvb , offset , ( next_operand_address - operand_address ) , operand_1 , "Addr: %u operand_1 %u" , operand_address , operand_1 ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & operand_2 ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_operand_2 , bytecode_tvb , offset , ( next_operand_address - operand_address ) , operand_2 , "Addr: %u operand_2 %u" , operand_address , operand_2 ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 if ( show_instr_detail_level == 1 ) {
 proto_tree_add_none_format ( udvm_tree , hf_sigcomp_decompress_instruction , bytecode_tvb , start_offset , offset - start_offset , "Addr: %u ## MULTIPLY (operand_1=%u, operand_2=%u)" , current_address , operand_1 , operand_2 ) ;
 }
 if ( operand_2 == 0 ) {
 result_code = 4 ;
 goto decompression_failure ;
 }
 result = operand_1 * operand_2 ;
 lsb = result & 0xff ;
 msb = result >> 8 ;
 buff [ result_dest ] = msb ;
 buff [ ( result_dest + 1 ) & 0xffff ] = lsb ;
 if ( print_level_1 ) {
 proto_tree_add_none_format ( udvm_tree , hf_sigcomp_loading_result , bytecode_tvb , 0 , - 1 , " Loading result %u at %u" , result , result_dest ) ;
 }
 current_address = next_operand_address ;
 goto execute_next_instruction ;
 break ;
 case SIGCOMP_INSTR_DIVIDE : if ( show_instr_detail_level == 2 ) {
 proto_item_append_text ( addr_item , " (operand_1, operand_2)" ) ;
 }
 start_offset = offset ;
 operand_address = current_address + 1 ;
 next_operand_address = dissect_udvm_reference_operand_memory ( buff , operand_address , & operand_1 , & result_dest ) ;
 if ( next_operand_address < operand_address ) goto decompression_failure ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_operand_1 , bytecode_tvb , offset , ( next_operand_address - operand_address ) , operand_1 , "Addr: %u operand_1 %u" , operand_address , operand_1 ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & operand_2 ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_operand_2 , bytecode_tvb , offset , ( next_operand_address - operand_address ) , operand_2 , "Addr: %u operand_2 %u" , operand_address , operand_2 ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 if ( show_instr_detail_level == 1 ) {
 proto_tree_add_none_format ( udvm_tree , hf_sigcomp_decompress_instruction , bytecode_tvb , start_offset , offset - start_offset , "Addr: %u ## DIVIDE (operand_1=%u, operand_2=%u)" , current_address , operand_1 , operand_2 ) ;
 }
 if ( operand_2 == 0 ) {
 result_code = 4 ;
 goto decompression_failure ;
 }
 result = operand_1 / operand_2 ;
 lsb = result & 0xff ;
 msb = result >> 8 ;
 buff [ result_dest ] = msb ;
 buff [ ( result_dest + 1 ) & 0xffff ] = lsb ;
 if ( print_level_1 ) {
 proto_tree_add_none_format ( udvm_tree , hf_sigcomp_loading_result , bytecode_tvb , 0 , - 1 , " Loading result %u at %u" , result , result_dest ) ;
 }
 current_address = next_operand_address ;
 goto execute_next_instruction ;
 break ;
 case SIGCOMP_INSTR_REMAINDER : if ( show_instr_detail_level == 2 ) {
 proto_item_append_text ( addr_item , " (operand_1, operand_2)" ) ;
 }
 start_offset = offset ;
 operand_address = current_address + 1 ;
 next_operand_address = dissect_udvm_reference_operand_memory ( buff , operand_address , & operand_1 , & result_dest ) ;
 if ( next_operand_address < operand_address ) goto decompression_failure ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_operand_1 , bytecode_tvb , offset , ( next_operand_address - operand_address ) , operand_1 , "Addr: %u operand_1 %u" , operand_address , operand_1 ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & operand_2 ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_operand_2 , bytecode_tvb , offset , ( next_operand_address - operand_address ) , operand_2 , "Addr: %u operand_2 %u" , operand_address , operand_2 ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 if ( show_instr_detail_level == 1 ) {
 proto_tree_add_none_format ( udvm_tree , hf_sigcomp_decompress_instruction , bytecode_tvb , start_offset , offset - start_offset , "Addr: %u ## REMAINDER (operand_1=%u, operand_2=%u)" , current_address , operand_1 , operand_2 ) ;
 }
 if ( operand_2 == 0 ) {
 result_code = 4 ;
 goto decompression_failure ;
 }
 result = operand_1 - operand_2 * ( operand_1 / operand_2 ) ;
 lsb = result & 0xff ;
 msb = result >> 8 ;
 buff [ result_dest ] = msb ;
 buff [ ( result_dest + 1 ) & 0xffff ] = lsb ;
 if ( print_level_1 ) {
 proto_tree_add_none_format ( udvm_tree , hf_sigcomp_loading_result , bytecode_tvb , 0 , - 1 , " Loading result %u at %u" , result , result_dest ) ;
 }
 current_address = next_operand_address ;
 goto execute_next_instruction ;
 break ;
 case SIGCOMP_INSTR_SORT_ASCENDING : if ( show_instr_detail_level == 2 ) {
 proto_item_append_text ( addr_item , " (start, n, k))" ) ;
 }
 proto_tree_add_expert ( udvm_tree , pinfo , & ei_sigcomp_execution_of_this_instruction_is_not_implemented , bytecode_tvb , 0 , - 1 ) ;
 break ;
 case SIGCOMP_INSTR_SORT_DESCENDING : if ( show_instr_detail_level == 2 ) {
 proto_item_append_text ( addr_item , " (start, n, k))" ) ;
 }
 proto_tree_add_expert ( udvm_tree , pinfo , & ei_sigcomp_execution_of_this_instruction_is_not_implemented , bytecode_tvb , 0 , - 1 ) ;
 break ;
 case SIGCOMP_INSTR_SHA_1 : if ( show_instr_detail_level == 2 ) {
 proto_item_append_text ( addr_item , " (position, length, destination)" ) ;
 }
 operand_address = current_address + 1 ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & position ) ;
 if ( print_level_1 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_position , bytecode_tvb , offset , ( next_operand_address - operand_address ) , position , "Addr: %u position %u" , operand_address , position ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & length ) ;
 if ( print_level_1 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_length , bytecode_tvb , offset , ( next_operand_address - operand_address ) , length , "Addr: %u Length %u" , operand_address , length ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = dissect_udvm_reference_operand_memory ( buff , operand_address , & ref_destination , & result_dest ) ;
 if ( next_operand_address < operand_address ) goto decompression_failure ;
 if ( print_level_1 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_ref_dest , bytecode_tvb , offset , ( next_operand_address - operand_address ) , ref_destination , "Addr: %u $destination %u" , operand_address , ref_destination ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 used_udvm_cycles = used_udvm_cycles + length ;
 n = 0 ;
 k = position ;
 byte_copy_right = buff [ 66 ] << 8 ;
 byte_copy_right = byte_copy_right | buff [ 67 ] ;
 byte_copy_left = buff [ 64 ] << 8 ;
 byte_copy_left = byte_copy_left | buff [ 65 ] ;
 if ( print_level_2 ) {
 proto_tree_add_bytes_format ( udvm_tree , hf_sigcomp_byte_copy , message_tvb , 0 , - 1 , NULL , "byte_copy_right = %u" , byte_copy_right ) ;
 }
 sha1_starts ( & ctx ) ;
 while ( n < length ) {
 guint16 handle_now = length ;
 if ( k < byte_copy_right && byte_copy_right <= k + ( length - n ) ) {
 handle_now = byte_copy_right - position ;
 }
 if ( ( k + handle_now >= UDVM_MEMORY_SIZE ) || ( n + handle_now >= UDVM_MEMORY_SIZE ) ) {
 goto decompression_failure ;
 }
 sha1_update ( & ctx , & buff [ k ] , handle_now ) ;
 k = ( k + handle_now ) & 0xffff ;
 n = ( n + handle_now ) & 0xffff ;
 if ( k >= byte_copy_right ) {
 k = byte_copy_left ;
 }
 }
 sha1_finish ( & ctx , sha1_digest_buf ) ;
 k = ref_destination ;
 for ( n = 0 ;
 n < STATE_BUFFER_SIZE ;
 n ++ ) {
 buff [ k ] = sha1_digest_buf [ n ] ;
 k = ( k + 1 ) & 0xffff ;
 n ++ ;
 if ( k == byte_copy_right ) {
 k = byte_copy_left ;
 }
 }
 if ( print_level_2 ) {
 proto_tree_add_bytes_with_length ( udvm_tree , hf_sigcomp_calculated_sha_1 , message_tvb , 0 , - 1 , sha1_digest_buf , STATE_BUFFER_SIZE ) ;
 }
 current_address = next_operand_address ;
 goto execute_next_instruction ;
 break ;
 case SIGCOMP_INSTR_LOAD : if ( show_instr_detail_level == 2 ) {
 proto_item_append_text ( addr_item , " (%%address, %%value)" ) ;
 }
 start_offset = offset ;
 operand_address = current_address + 1 ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & addr ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_address , bytecode_tvb , offset , ( next_operand_address - operand_address ) , addr , "Addr: %u Address %u" , operand_address , addr ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & value ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_value , bytecode_tvb , offset , ( next_operand_address - operand_address ) , value , "Addr: %u Value %u" , operand_address , value ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 lsb = value & 0xff ;
 msb = value >> 8 ;
 buff [ addr ] = msb ;
 buff [ ( addr + 1 ) & 0xffff ] = lsb ;
 if ( print_level_1 ) {
 proto_tree_add_none_format ( udvm_tree , hf_sigcomp_decompress_instruction , bytecode_tvb , start_offset , offset - start_offset , "Addr: %u ## LOAD (%%address=%u, %%value=%u)" , current_address , addr , value ) ;
 proto_tree_add_none_format ( udvm_tree , hf_sigcomp_loading_result , bytecode_tvb , 0 , - 1 , " Loading bytes at %u Value %u 0x%x" , addr , value , value ) ;
 }
 current_address = next_operand_address ;
 goto execute_next_instruction ;
 break ;
 case SIGCOMP_INSTR_MULTILOAD : if ( show_instr_detail_level == 2 ) {
 proto_item_append_text ( addr_item , " (%%address, #n, value_0, ..., value_n-1)" ) ;
 }
 start_offset = offset ;
 operand_address = current_address + 1 ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & addr ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_address , bytecode_tvb , offset , ( next_operand_address - operand_address ) , addr , "Addr: %u Address %u" , operand_address , addr ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_literal_operand ( buff , operand_address , & n ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_literal_num , bytecode_tvb , offset , ( next_operand_address - operand_address ) , n , "Addr: %u n %u" , operand_address , n ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 if ( show_instr_detail_level == 1 ) {
 proto_tree_add_none_format ( udvm_tree , hf_sigcomp_decompress_instruction , bytecode_tvb , start_offset , offset - start_offset , "Addr: %u ## MULTILOAD (%%address=%u, #n=%u, value_0, ..., value_%d)" , current_address , addr , n , n - 1 ) ;
 }
 operand_address = next_operand_address ;
 used_udvm_cycles = used_udvm_cycles + n ;
 while ( n > 0 ) {
 n = n - 1 ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & value ) ;
 lsb = value & 0xff ;
 msb = value >> 8 ;
 if ( addr >= UDVM_MEMORY_SIZE - 1 ) goto decompression_failure ;
 buff [ addr ] = msb ;
 buff [ ( addr + 1 ) & 0xffff ] = lsb ;
 length = next_operand_address - operand_address ;
 if ( print_level_1 ) {
 proto_tree_add_none_format ( udvm_tree , hf_sigcomp_loading_result , bytecode_tvb , 0 , - 1 , "Addr: %u Value %5u - Loading bytes at %5u Value %5u 0x%x" , operand_address , value , addr , value , value ) ;
 }
 addr = addr + 2 ;
 operand_address = next_operand_address ;
 }
 current_address = next_operand_address ;
 goto execute_next_instruction ;
 break ;
 case SIGCOMP_INSTR_PUSH : if ( show_instr_detail_level == 2 ) {
 proto_item_append_text ( addr_item , " (value)" ) ;
 }
 start_offset = offset ;
 operand_address = current_address + 1 ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & value ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_value , bytecode_tvb , offset , ( next_operand_address - operand_address ) , value , "Addr: %u Value %u" , operand_address , value ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 if ( show_instr_detail_level == 1 ) {
 proto_tree_add_none_format ( udvm_tree , hf_sigcomp_decompress_instruction , bytecode_tvb , start_offset , offset - start_offset , "Addr: %u ## PUSH (value=%u)" , current_address , value ) ;
 }
 current_address = next_operand_address ;
 stack_location = ( buff [ 70 ] << 8 ) | buff [ 71 ] ;
 stack_fill = ( buff [ stack_location ] << 8 ) | buff [ ( stack_location + 1 ) & 0xFFFF ] ;
 addr = ( stack_location + stack_fill * 2 + 2 ) & 0xFFFF ;
 if ( addr >= UDVM_MEMORY_SIZE - 1 ) goto decompression_failure ;
 buff [ addr ] = ( value >> 8 ) & 0x00FF ;
 buff [ ( addr + 1 ) & 0xFFFF ] = value & 0x00FF ;
 if ( stack_location >= UDVM_MEMORY_SIZE - 1 ) goto decompression_failure ;
 stack_fill = ( stack_fill + 1 ) & 0xFFFF ;
 buff [ stack_location ] = ( stack_fill >> 8 ) & 0x00FF ;
 buff [ ( stack_location + 1 ) & 0xFFFF ] = stack_fill & 0x00FF ;
 goto execute_next_instruction ;
 break ;
 case SIGCOMP_INSTR_POP : if ( show_instr_detail_level == 2 ) {
 proto_item_append_text ( addr_item , " (value)" ) ;
 }
 start_offset = offset ;
 operand_address = current_address + 1 ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & destination ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_address , bytecode_tvb , offset , ( next_operand_address - operand_address ) , destination , "Addr: %u Value %u" , operand_address , destination ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 if ( show_instr_detail_level == 1 ) {
 proto_tree_add_none_format ( udvm_tree , hf_sigcomp_decompress_instruction , bytecode_tvb , start_offset , offset - start_offset , "Addr: %u ## POP (address=%u)" , current_address , destination ) ;
 }
 current_address = next_operand_address ;
 stack_location = ( buff [ 70 ] << 8 ) | buff [ 71 ] ;
 stack_fill = ( buff [ stack_location ] << 8 ) | buff [ ( stack_location + 1 ) & 0xFFFF ] ;
 if ( stack_fill == 0 ) {
 result_code = 16 ;
 goto decompression_failure ;
 }
 if ( stack_location >= UDVM_MEMORY_SIZE - 1 ) goto decompression_failure ;
 stack_fill = ( stack_fill - 1 ) & 0xFFFF ;
 buff [ stack_location ] = ( stack_fill >> 8 ) & 0x00FF ;
 buff [ ( stack_location + 1 ) & 0xFFFF ] = stack_fill & 0x00FF ;
 addr = ( stack_location + stack_fill * 2 + 2 ) & 0xFFFF ;
 if ( addr >= UDVM_MEMORY_SIZE - 1 ) goto decompression_failure ;
 value = ( buff [ addr ] << 8 ) | buff [ ( addr + 1 ) & 0xFFFF ] ;
 if ( destination >= UDVM_MEMORY_SIZE - 1 ) goto decompression_failure ;
 buff [ destination ] = ( value >> 8 ) & 0x00FF ;
 buff [ ( destination + 1 ) & 0xFFFF ] = value & 0x00FF ;
 goto execute_next_instruction ;
 break ;
 case SIGCOMP_INSTR_COPY : if ( show_instr_detail_level == 2 ) {
 proto_item_append_text ( addr_item , " (position, length, destination)" ) ;
 }
 start_offset = offset ;
 operand_address = current_address + 1 ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & position ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_position , bytecode_tvb , offset , ( next_operand_address - operand_address ) , position , "Addr: %u position %u" , operand_address , position ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & length ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_length , bytecode_tvb , offset , ( next_operand_address - operand_address ) , length , "Addr: %u Length %u" , operand_address , length ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & destination ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_ref_dest , bytecode_tvb , offset , ( next_operand_address - operand_address ) , destination , "Addr: %u Destination %u" , operand_address , destination ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 if ( show_instr_detail_level == 1 ) {
 proto_tree_add_none_format ( udvm_tree , hf_sigcomp_decompress_instruction , bytecode_tvb , start_offset , offset - start_offset , "Addr: %u ## COPY (position=%u, length=%u, destination=%u)" , current_address , position , length , destination ) ;
 }
 current_address = next_operand_address ;
 n = 0 ;
 k = destination ;
 byte_copy_right = buff [ 66 ] << 8 ;
 byte_copy_right = byte_copy_right | buff [ 67 ] ;
 byte_copy_left = buff [ 64 ] << 8 ;
 byte_copy_left = byte_copy_left | buff [ 65 ] ;
 if ( print_level_2 ) {
 proto_tree_add_bytes_format ( udvm_tree , hf_sigcomp_byte_copy , message_tvb , input_address , 1 , NULL , " byte_copy_right = %u" , byte_copy_right ) ;
 }
 while ( n < length ) {
 buff [ k ] = buff [ position ] ;
 if ( print_level_2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_sigcomp_copying_value , message_tvb , input_address , 1 , buff [ position ] , " Copying value: %u (0x%x) to Addr: %u" , buff [ position ] , buff [ position ] , k ) ;
 }
 position = ( position + 1 ) & 0xffff ;
 k = ( k + 1 ) & 0xffff ;
 n ++ ;
 if ( k == byte_copy_right ) {
 k = byte_copy_left ;
 }
 if ( position == byte_copy_right ) {
 position = byte_copy_left ;
 }
 }
 used_udvm_cycles = used_udvm_cycles + length ;
 goto execute_next_instruction ;
 break ;
 case SIGCOMP_INSTR_COPY_LITERAL : if ( show_instr_detail_level == 2 ) {
 proto_item_append_text ( addr_item , " (position, length, $destination)" ) ;
 }
 start_offset = offset ;
 operand_address = current_address + 1 ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & position ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_position , bytecode_tvb , offset , ( next_operand_address - operand_address ) , position , "Addr: %u position %u" , operand_address , position ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & length ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_length , bytecode_tvb , offset , ( next_operand_address - operand_address ) , length , "Addr: %u Length %u" , operand_address , length ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = dissect_udvm_reference_operand_memory ( buff , operand_address , & ref_destination , & result_dest ) ;
 if ( next_operand_address < operand_address ) goto decompression_failure ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_ref_dest , bytecode_tvb , offset , ( next_operand_address - operand_address ) , ref_destination , "Addr: %u destination %u" , operand_address , ref_destination ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 if ( show_instr_detail_level == 1 ) {
 proto_tree_add_none_format ( udvm_tree , hf_sigcomp_decompress_instruction , bytecode_tvb , start_offset , offset - start_offset , "Addr: %u ## COPY-LITERAL (position=%u, length=%u, $destination=%u)" , current_address , position , length , ref_destination ) ;
 }
 current_address = next_operand_address ;
 n = 0 ;
 k = ref_destination ;
 byte_copy_right = buff [ 66 ] << 8 ;
 byte_copy_right = byte_copy_right | buff [ 67 ] ;
 byte_copy_left = buff [ 64 ] << 8 ;
 byte_copy_left = byte_copy_left | buff [ 65 ] ;
 if ( print_level_2 ) {
 proto_tree_add_bytes_format ( udvm_tree , hf_sigcomp_byte_copy , message_tvb , input_address , 1 , NULL , " byte_copy_right = %u" , byte_copy_right ) ;
 }
 while ( n < length ) {
 buff [ k ] = buff [ position ] ;
 if ( print_level_2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_sigcomp_copying_value , message_tvb , input_address , 1 , buff [ position ] , " Copying value: %u (0x%x) to Addr: %u" , buff [ position ] , buff [ position ] , k ) ;
 }
 position = ( position + 1 ) & 0xffff ;
 k = ( k + 1 ) & 0xffff ;
 n ++ ;
 if ( k == byte_copy_right ) {
 k = byte_copy_left ;
 }
 if ( position == byte_copy_right ) {
 position = byte_copy_left ;
 }
 }
 buff [ result_dest ] = k >> 8 ;
 buff [ ( result_dest + 1 ) & 0xffff ] = k & 0x00ff ;
 used_udvm_cycles = used_udvm_cycles + length ;
 goto execute_next_instruction ;
 break ;
 case SIGCOMP_INSTR_COPY_OFFSET : if ( show_instr_detail_level == 2 ) {
 proto_item_append_text ( addr_item , " (offset, length, $destination)" ) ;
 }
 start_offset = offset ;
 operand_address = current_address + 1 ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & multy_offset ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_offset , bytecode_tvb , offset , ( next_operand_address - operand_address ) , multy_offset , "Addr: %u offset %u" , operand_address , multy_offset ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & length ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_length , bytecode_tvb , offset , ( next_operand_address - operand_address ) , length , "Addr: %u Length %u" , operand_address , length ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = dissect_udvm_reference_operand_memory ( buff , operand_address , & ref_destination , & result_dest ) ;
 if ( next_operand_address < operand_address ) goto decompression_failure ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_ref_dest , bytecode_tvb , offset , ( next_operand_address - operand_address ) , ref_destination , "Addr: %u $destination %u" , operand_address , ref_destination ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 if ( show_instr_detail_level == 1 ) {
 proto_tree_add_none_format ( udvm_tree , hf_sigcomp_decompress_instruction , bytecode_tvb , start_offset , offset - start_offset , "Addr: %u ## COPY-OFFSET (offset=%u, length=%u, $destination=%u)" , current_address , multy_offset , length , result_dest ) ;
 }
 current_address = next_operand_address ;
 byte_copy_left = buff [ 64 ] << 8 ;
 byte_copy_left = byte_copy_left | buff [ 65 ] ;
 byte_copy_right = buff [ 66 ] << 8 ;
 byte_copy_right = byte_copy_right | buff [ 67 ] ;
 for ( position = ref_destination , i = 0 ;
 i < multy_offset ;
 i ++ ) {
 if ( position == byte_copy_left ) {
 position = ( byte_copy_right - 1 ) & 0xffff ;
 }
 else {
 position = ( position - 1 ) & 0xffff ;
 }
 }
 if ( print_level_2 ) {
 proto_tree_add_bytes_format ( udvm_tree , hf_sigcomp_byte_copy , message_tvb , input_address , 1 , NULL , " byte_copy_left = %u byte_copy_right = %u position= %u" , byte_copy_left , byte_copy_right , position ) ;
 }
 n = 0 ;
 k = ref_destination ;
 if ( print_level_2 ) {
 proto_tree_add_bytes_format ( udvm_tree , hf_sigcomp_byte_copy , message_tvb , input_address , 1 , NULL , " byte_copy_left = %u byte_copy_right = %u" , byte_copy_left , byte_copy_right ) ;
 }
 while ( n < length ) {
 buff [ k ] = buff [ position ] ;
 if ( print_level_2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_sigcomp_copying_value , message_tvb , input_address , 1 , buff [ position ] , " Copying value: %5u (0x%x) from Addr: %u to Addr: %u" , buff [ position ] , buff [ position ] , ( position ) , k ) ;
 }
 n ++ ;
 k = ( k + 1 ) & 0xffff ;
 position = ( position + 1 ) & 0xffff ;
 if ( k == byte_copy_right ) {
 k = byte_copy_left ;
 }
 if ( position == byte_copy_right ) {
 position = byte_copy_left ;
 }
 }
 buff [ result_dest ] = k >> 8 ;
 buff [ result_dest + 1 ] = k & 0x00ff ;
 used_udvm_cycles = used_udvm_cycles + length ;
 goto execute_next_instruction ;
 break ;
 case SIGCOMP_INSTR_MEMSET : if ( show_instr_detail_level == 2 ) {
 proto_item_append_text ( addr_item , " (address, length, start_value, offset)" ) ;
 }
 start_offset = offset ;
 operand_address = current_address + 1 ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & addr ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_address , bytecode_tvb , offset , ( next_operand_address - operand_address ) , addr , "Addr: %u Address %u" , operand_address , addr ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & length ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_length , bytecode_tvb , offset , ( next_operand_address - operand_address ) , length , "Addr: %u Length %u" , operand_address , length ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & start_value ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_start_value , bytecode_tvb , offset , ( next_operand_address - operand_address ) , start_value , "Addr: %u start_value %u" , operand_address , start_value ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & multy_offset ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_offset , bytecode_tvb , offset , ( next_operand_address - operand_address ) , multy_offset , "Addr: %u offset %u" , operand_address , multy_offset ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 if ( show_instr_detail_level == 1 ) {
 proto_tree_add_none_format ( udvm_tree , hf_sigcomp_decompress_instruction , bytecode_tvb , start_offset , offset - start_offset , "Addr: %u ## MEMSET (address=%u, length=%u, start_value=%u, offset=%u)" , current_address , addr , length , start_value , multy_offset ) ;
 }
 current_address = next_operand_address ;
 n = 0 ;
 k = addr ;
 byte_copy_right = buff [ 66 ] << 8 ;
 byte_copy_right = byte_copy_right | buff [ 67 ] ;
 byte_copy_left = buff [ 64 ] << 8 ;
 byte_copy_left = byte_copy_left | buff [ 65 ] ;
 if ( print_level_2 ) {
 proto_tree_add_bytes_format ( udvm_tree , hf_sigcomp_byte_copy , message_tvb , input_address , 1 , NULL , " byte_copy_left = %u byte_copy_right = %u" , byte_copy_left , byte_copy_right ) ;
 }
 while ( n < length ) {
 if ( k == byte_copy_right ) {
 k = byte_copy_left ;
 }
 buff [ k ] = ( start_value + ( n * multy_offset ) ) & 0xff ;
 if ( print_level_2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_sigcomp_storing_value , message_tvb , input_address , 1 , buff [ k ] , " Storing value: %u (0x%x) at Addr: %u" , buff [ k ] , buff [ k ] , k ) ;
 }
 k = ( k + 1 ) & 0xffff ;
 n ++ ;
 }
 used_udvm_cycles = used_udvm_cycles + length ;
 goto execute_next_instruction ;
 break ;
 case SIGCOMP_INSTR_JUMP : if ( show_instr_detail_level == 2 ) {
 proto_item_append_text ( addr_item , " (@address)" ) ;
 }
 start_offset = offset ;
 operand_address = current_address + 1 ;
 next_operand_address = decode_udvm_address_operand ( buff , operand_address , & at_address , current_address ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_at_address , bytecode_tvb , offset , ( next_operand_address - operand_address ) , at_address , "Addr: %u @Address %u" , operand_address , at_address ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 if ( show_instr_detail_level == 1 ) {
 proto_tree_add_none_format ( udvm_tree , hf_sigcomp_decompress_instruction , bytecode_tvb , start_offset , offset - start_offset , "Addr: %u ## JUMP (@address=%u)" , current_address , at_address ) ;
 }
 current_address = at_address ;
 goto execute_next_instruction ;
 break ;
 case SIGCOMP_INSTR_COMPARE : if ( show_instr_detail_level == 2 ) {
 proto_item_append_text ( addr_item , " (value_1, value_2, @address_1, @address_2, @address_3)" ) ;
 }
 start_offset = offset ;
 operand_address = current_address + 1 ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & value_1 ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_value , bytecode_tvb , offset , ( next_operand_address - operand_address ) , value_1 , "Addr: %u Value %u" , operand_address , value_1 ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & value_2 ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_value , bytecode_tvb , offset , ( next_operand_address - operand_address ) , value_2 , "Addr: %u Value %u" , operand_address , value_2 ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & at_address_1 ) ;
 at_address_1 = ( current_address + at_address_1 ) & 0xffff ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_at_address , bytecode_tvb , offset , ( next_operand_address - operand_address ) , at_address_1 , "Addr: %u @Address %u" , operand_address , at_address_1 ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & at_address_2 ) ;
 at_address_2 = ( current_address + at_address_2 ) & 0xffff ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_at_address , bytecode_tvb , offset , ( next_operand_address - operand_address ) , at_address_2 , "Addr: %u @Address %u" , operand_address , at_address_2 ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & at_address_3 ) ;
 at_address_3 = ( current_address + at_address_3 ) & 0xffff ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_at_address , bytecode_tvb , offset , ( next_operand_address - operand_address ) , at_address_3 , "Addr: %u @Address %u" , operand_address , at_address_3 ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 if ( show_instr_detail_level == 1 ) {
 proto_tree_add_none_format ( udvm_tree , hf_sigcomp_decompress_instruction , bytecode_tvb , start_offset , offset - start_offset , "Addr: %u ## COMPARE (value_1=%u, value_2=%u, @address_1=%u, @address_2=%u, @address_3=%u)" , current_address , value_1 , value_2 , at_address_1 , at_address_2 , at_address_3 ) ;
 }
 if ( value_1 < value_2 ) current_address = at_address_1 ;
 if ( value_1 == value_2 ) current_address = at_address_2 ;
 if ( value_1 > value_2 ) current_address = at_address_3 ;
 goto execute_next_instruction ;
 break ;
 case SIGCOMP_INSTR_CALL : if ( show_instr_detail_level == 2 ) {
 proto_item_append_text ( addr_item , " (@address) (PUSH addr )" ) ;
 }
 start_offset = offset ;
 operand_address = current_address + 1 ;
 next_operand_address = decode_udvm_address_operand ( buff , operand_address , & at_address , current_address ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_at_address , bytecode_tvb , offset , ( next_operand_address - operand_address ) , at_address , "Addr: %u @Address %u" , operand_address , at_address ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 if ( show_instr_detail_level == 1 ) {
 proto_tree_add_none_format ( udvm_tree , hf_sigcomp_decompress_instruction , bytecode_tvb , start_offset , offset - start_offset , "Addr: %u ## CALL (@address=%u)" , current_address , at_address ) ;
 }
 current_address = next_operand_address ;
 stack_location = ( buff [ 70 ] << 8 ) | buff [ 71 ] ;
 stack_fill = ( buff [ stack_location ] << 8 ) | buff [ ( stack_location + 1 ) & 0xFFFF ] ;
 addr = ( stack_location + stack_fill * 2 + 2 ) & 0xFFFF ;
 if ( addr >= UDVM_MEMORY_SIZE - 1 ) goto decompression_failure ;
 buff [ addr ] = ( current_address >> 8 ) & 0x00FF ;
 buff [ ( addr + 1 ) & 0xFFFF ] = current_address & 0x00FF ;
 stack_fill = ( stack_fill + 1 ) & 0xFFFF ;
 if ( stack_location >= UDVM_MEMORY_SIZE - 1 ) goto decompression_failure ;
 buff [ stack_location ] = ( stack_fill >> 8 ) & 0x00FF ;
 buff [ ( stack_location + 1 ) & 0xFFFF ] = stack_fill & 0x00FF ;
 current_address = at_address ;
 goto execute_next_instruction ;
 break ;
 case SIGCOMP_INSTR_RETURN : stack_location = ( buff [ 70 ] << 8 ) | buff [ 71 ] ;
 stack_fill = ( buff [ stack_location ] << 8 ) | buff [ ( stack_location + 1 ) & 0xFFFF ] ;
 if ( stack_fill == 0 ) {
 result_code = 16 ;
 goto decompression_failure ;
 }
 stack_fill = ( stack_fill - 1 ) & 0xFFFF ;
 if ( stack_location >= UDVM_MEMORY_SIZE - 1 ) goto decompression_failure ;
 buff [ stack_location ] = ( stack_fill >> 8 ) & 0x00FF ;
 buff [ ( stack_location + 1 ) & 0xFFFF ] = stack_fill & 0x00FF ;
 addr = ( stack_location + stack_fill * 2 + 2 ) & 0xFFFF ;
 at_address = ( buff [ addr ] << 8 ) | buff [ ( addr + 1 ) & 0xFFFF ] ;
 current_address = at_address ;
 goto execute_next_instruction ;
 break ;
 case SIGCOMP_INSTR_SWITCH : instruction_address = current_address ;
 if ( show_instr_detail_level == 2 ) {
 proto_item_append_text ( addr_item , " (#n, j, @address_0, @address_1, ... , @address_n-1))" ) ;
 }
 operand_address = current_address + 1 ;
 next_operand_address = decode_udvm_literal_operand ( buff , operand_address , & n ) ;
 if ( print_level_2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_literal_num , bytecode_tvb , offset , ( next_operand_address - operand_address ) , n , "Addr: %u n %u" , operand_address , n ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & j ) ;
 if ( print_level_2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_j , bytecode_tvb , offset , ( next_operand_address - operand_address ) , j , "Addr: %u j %u" , operand_address , j ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 m = 0 ;
 while ( m < n ) {
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & at_address_1 ) ;
 at_address_1 = ( instruction_address + at_address_1 ) & 0xffff ;
 if ( print_level_2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_at_address , bytecode_tvb , offset , ( next_operand_address - operand_address ) , at_address_1 , "Addr: %u @Address %u" , operand_address , at_address_1 ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 if ( j == m ) {
 current_address = at_address_1 ;
 }
 operand_address = next_operand_address ;
 m ++ ;
 }
 if ( ( j == n ) || ( j > n ) ) {
 result_code = 5 ;
 goto decompression_failure ;
 }
 if ( current_address > UDVM_MEMORY_SIZE ) {
 result_code = 6 ;
 goto decompression_failure ;
 }
 used_udvm_cycles = used_udvm_cycles + n ;
 goto execute_next_instruction ;
 break ;
 case SIGCOMP_INSTR_CRC : if ( show_instr_detail_level == 2 ) {
 proto_item_append_text ( addr_item , " (value, position, length, @address)" ) ;
 }
 start_offset = offset ;
 operand_address = current_address + 1 ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & value ) ;
 if ( print_level_2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_value , bytecode_tvb , offset , ( next_operand_address - operand_address ) , value , "Addr: %u Value %u" , operand_address , value ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & position ) ;
 if ( print_level_2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_position , bytecode_tvb , offset , ( next_operand_address - operand_address ) , position , "Addr: %u position %u" , operand_address , position ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & length ) ;
 if ( print_level_2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_length , bytecode_tvb , offset , ( next_operand_address - operand_address ) , length , "Addr: %u Length %u" , operand_address , length ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & at_address ) ;
 at_address = ( current_address + at_address ) & 0xffff ;
 if ( print_level_2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_at_address , bytecode_tvb , offset , ( next_operand_address - operand_address ) , at_address , "Addr: %u @Address %u" , operand_address , at_address ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 used_udvm_cycles = used_udvm_cycles + length ;
 n = 0 ;
 k = position ;
 byte_copy_right = buff [ 66 ] << 8 ;
 byte_copy_right = byte_copy_right | buff [ 67 ] ;
 byte_copy_left = buff [ 64 ] << 8 ;
 byte_copy_left = byte_copy_left | buff [ 65 ] ;
 result = 0 ;
 if ( print_level_2 ) {
 proto_tree_add_bytes_format ( udvm_tree , hf_sigcomp_byte_copy , message_tvb , 0 , - 1 , NULL , "byte_copy_right = %u" , byte_copy_right ) ;
 }
 while ( n < length ) {
 guint16 handle_now = length - n ;
 if ( k < byte_copy_right && byte_copy_right <= k + ( length - n ) ) {
 handle_now = byte_copy_right - k ;
 }
 if ( k + handle_now >= UDVM_MEMORY_SIZE ) goto decompression_failure ;
 result = crc16_ccitt_seed ( & buff [ k ] , handle_now , ( guint16 ) ( result ^ 0xffff ) ) ;
 k = ( k + handle_now ) & 0xffff ;
 n = ( n + handle_now ) & 0xffff ;
 if ( k >= byte_copy_right ) {
 k = byte_copy_left ;
 }
 }
 result = result ^ 0xffff ;
 if ( print_level_1 ) {
 proto_tree_add_none_format ( udvm_tree , hf_sigcomp_decompress_instruction , bytecode_tvb , start_offset , offset - start_offset , "Calculated CRC %u" , result ) ;
 }
 if ( result != value ) {
 current_address = at_address ;
 }
 else {
 current_address = next_operand_address ;
 }
 goto execute_next_instruction ;
 break ;
 case SIGCOMP_INSTR_INPUT_BYTES : if ( show_instr_detail_level == 2 ) {
 proto_item_append_text ( addr_item , " length, destination, @address)" ) ;
 }
 start_offset = offset ;
 operand_address = current_address + 1 ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & length ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_length , bytecode_tvb , offset , ( next_operand_address - operand_address ) , length , "Addr: %u Length %u" , operand_address , length ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & destination ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_destination , bytecode_tvb , offset , ( next_operand_address - operand_address ) , destination , "Addr: %u Destination %u" , operand_address , destination ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & at_address ) ;
 at_address = ( current_address + at_address ) & 0xffff ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_at_address , bytecode_tvb , offset , ( next_operand_address - operand_address ) , at_address , "Addr: %u @Address %u" , operand_address , at_address ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 if ( show_instr_detail_level == 1 ) {
 proto_tree_add_none_format ( udvm_tree , hf_sigcomp_decompress_instruction , bytecode_tvb , start_offset , offset - start_offset , "Addr: %u ## INPUT-BYTES length=%u, destination=%u, @address=%u)" , current_address , length , destination , at_address ) ;
 }
 n = 0 ;
 k = destination ;
 byte_copy_right = buff [ 66 ] << 8 ;
 byte_copy_right = byte_copy_right | buff [ 67 ] ;
 byte_copy_left = buff [ 64 ] << 8 ;
 byte_copy_left = byte_copy_left | buff [ 65 ] ;
 if ( print_level_1 ) {
 proto_tree_add_bytes_format ( udvm_tree , hf_sigcomp_byte_copy , message_tvb , input_address , 1 , NULL , " byte_copy_right = %u" , byte_copy_right ) ;
 }
 remaining_bits = 0 ;
 input_bits = 0 ;
 while ( n < length ) {
 if ( input_address > ( msg_end - 1 ) ) {
 current_address = at_address ;
 result_code = 14 ;
 goto execute_next_instruction ;
 }
 if ( k == byte_copy_right ) {
 k = byte_copy_left ;
 }
 octet = tvb_get_guint8 ( message_tvb , input_address ) ;
 buff [ k ] = octet ;
 if ( print_level_1 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_sigcomp_loading_value , message_tvb , input_address , 1 , octet , " Loading value: %u (0x%x) at Addr: %u" , octet , octet , k ) ;
 }
 input_address ++ ;
 k = ( k + 1 ) & 0xffff ;
 n ++ ;
 }
 used_udvm_cycles = used_udvm_cycles + length ;
 current_address = next_operand_address ;
 goto execute_next_instruction ;
 break ;
 case SIGCOMP_INSTR_INPUT_BITS : if ( show_instr_detail_level == 2 ) {
 proto_item_append_text ( addr_item , " (length, destination, @address)" ) ;
 }
 start_offset = offset ;
 operand_address = current_address + 1 ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & length ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_length , bytecode_tvb , offset , ( next_operand_address - operand_address ) , length , "Addr: %u length %u" , operand_address , length ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & destination ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_destination , bytecode_tvb , offset , ( next_operand_address - operand_address ) , destination , "Addr: %u Destination %u" , operand_address , destination ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_address_operand ( buff , operand_address , & at_address , current_address ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_at_address , bytecode_tvb , offset , ( next_operand_address - operand_address ) , at_address , "Addr: %u @Address %u" , operand_address , at_address ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 if ( show_instr_detail_level == 1 ) {
 proto_tree_add_none_format ( udvm_tree , hf_sigcomp_decompress_instruction , bytecode_tvb , start_offset , offset - start_offset , "Addr: %u ## INPUT-BITS length=%u, destination=%u, @address=%u)" , current_address , length , destination , at_address ) ;
 }
 current_address = next_operand_address ;
 input_bit_order = buff [ 68 ] << 8 ;
 input_bit_order = input_bit_order | buff [ 69 ] ;
 if ( length > 16 ) {
 result_code = 7 ;
 goto decompression_failure ;
 }
 if ( input_bit_order > 7 ) {
 result_code = 8 ;
 goto decompression_failure ;
 }
 bit_order = ( input_bit_order & 0x0004 ) >> 2 ;
 value = decomp_dispatch_get_bits ( message_tvb , udvm_tree , bit_order , buff , & old_input_bit_order , & remaining_bits , & input_bits , & input_address , length , & result_code , msg_end , print_level_1 ) ;
 if ( result_code == 11 ) {
 current_address = at_address ;
 goto execute_next_instruction ;
 }
 msb = value >> 8 ;
 lsb = value & 0x00ff ;
 if ( destination >= UDVM_MEMORY_SIZE - 1 ) goto decompression_failure ;
 buff [ destination ] = msb ;
 buff [ ( destination + 1 ) & 0xffff ] = lsb ;
 if ( print_level_1 ) {
 proto_tree_add_none_format ( udvm_tree , hf_sigcomp_loading_result , message_tvb , input_address , 1 , " Loading value: %u (0x%x) at Addr: %u, remaining_bits: %u" , value , value , destination , remaining_bits ) ;
 }
 goto execute_next_instruction ;
 break ;
 case SIGCOMP_INSTR_INPUT_HUFFMAN : if ( show_instr_detail_level == 2 ) {
 proto_item_append_text ( addr_item , " (destination, @address, #n, bits_1, lower_bound_1,upper_bound_1, uncompressed_1, ... , bits_n, lower_bound_n,upper_bound_n, uncompressed_n)" ) ;
 }
 start_offset = offset ;
 operand_address = current_address + 1 ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & destination ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_destination , bytecode_tvb , offset , ( next_operand_address - operand_address ) , destination , "Addr: %u Destination %u" , operand_address , destination ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_address_operand ( buff , operand_address , & at_address , current_address ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_at_address , bytecode_tvb , offset , ( next_operand_address - operand_address ) , at_address , "Addr: %u @Address %u" , operand_address , at_address ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_literal_operand ( buff , operand_address , & n ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_literal_num , bytecode_tvb , offset , ( next_operand_address - operand_address ) , n , "Addr: %u n %u" , operand_address , n ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 if ( show_instr_detail_level == 1 ) {
 proto_tree_add_none_format ( udvm_tree , hf_sigcomp_decompress_instruction , bytecode_tvb , start_offset , offset - start_offset , "Addr: %u ## INPUT-HUFFMAN (destination=%u, @address=%u, #n=%u, bits_1, lower_1,upper_1, unc_1, ... , bits_%d, lower_%d,upper_%d, unc_%d)" , current_address , destination , at_address , n , n , n , n , n ) ;
 }
 used_udvm_cycles = used_udvm_cycles + n ;
 input_bit_order = buff [ 68 ] << 8 ;
 input_bit_order = input_bit_order | buff [ 69 ] ;
 bit_order = ( input_bit_order & 0x0002 ) >> 1 ;
 j = 1 ;
 H = 0 ;
 m = n ;
 outside_huffman_boundaries = TRUE ;
 print_in_loop = print_level_3 ;
 while ( m > 0 ) {
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & bits_n ) ;
 if ( print_in_loop ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_bits , bytecode_tvb , offset , ( next_operand_address - operand_address ) , bits_n , "Addr: %u bits_n %u" , operand_address , bits_n ) ;
 }
 if ( bits_n > 31 ) break ;
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & lower_bound_n ) ;
 if ( print_in_loop ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_lower_bound , bytecode_tvb , offset , ( next_operand_address - operand_address ) , lower_bound_n , "Addr: %u lower_bound_n %u" , operand_address , lower_bound_n ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & upper_bound_n ) ;
 if ( print_in_loop ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_upper_bound , bytecode_tvb , offset , ( next_operand_address - operand_address ) , upper_bound_n , "Addr: %u upper_bound_n %u" , operand_address , upper_bound_n ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & uncompressed_n ) ;
 if ( print_in_loop ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_uncompressed , bytecode_tvb , offset , ( next_operand_address - operand_address ) , uncompressed_n , "Addr: %u uncompressed_n %u" , operand_address , uncompressed_n ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 if ( outside_huffman_boundaries ) {
 k = decomp_dispatch_get_bits ( message_tvb , udvm_tree , bit_order , buff , & old_input_bit_order , & remaining_bits , & input_bits , & input_address , bits_n , & result_code , msg_end , print_level_1 ) ;
 if ( result_code == 11 ) {
 current_address = at_address ;
 goto execute_next_instruction ;
 }
 oldH = H ;
 H = ( H << bits_n ) | k ;
 if ( print_level_3 ) {
 proto_tree_add_bytes_format ( udvm_tree , hf_sigcomp_set_hu , bytecode_tvb , 0 , - 1 , NULL , " Set H(%u) := H(%u) * 2^bits_j(%u) + k(%u)" , H , oldH , 1 << bits_n , k ) ;
 }
 if ( ( H < lower_bound_n ) || ( H > upper_bound_n ) ) {
 outside_huffman_boundaries = TRUE ;
 }
 else {
 outside_huffman_boundaries = FALSE ;
 print_in_loop = FALSE ;
 if ( print_level_2 ) {
 proto_tree_add_bytes_format ( udvm_tree , hf_sigcomp_set_hu , bytecode_tvb , 0 , - 1 , NULL , " H(%u) = H(%u) + uncompressed_n(%u) - lower_bound_n(%u)" , ( H + uncompressed_n - lower_bound_n ) , H , uncompressed_n , lower_bound_n ) ;
 }
 H = H + uncompressed_n - lower_bound_n ;
 msb = H >> 8 ;
 lsb = H & 0x00ff ;
 if ( destination >= UDVM_MEMORY_SIZE - 1 ) goto decompression_failure ;
 buff [ destination ] = msb ;
 buff [ ( destination + 1 ) & 0xffff ] = lsb ;
 if ( print_level_1 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_sigcomp_loading_h , message_tvb , input_address , 1 , H , " Loading H: %u (0x%x) at Addr: %u,j = %u remaining_bits: %u" , H , H , destination , ( n - m + 1 ) , remaining_bits ) ;
 }
 }
 }
 m = m - 1 ;
 }
 if ( outside_huffman_boundaries ) {
 result_code = 10 ;
 goto decompression_failure ;
 }
 current_address = next_operand_address ;
 goto execute_next_instruction ;
 break ;
 case SIGCOMP_INSTR_STATE_ACCESS : if ( show_instr_detail_level == 2 ) {
 proto_item_append_text ( addr_item , " (partial_identifier_start, partial_identifier_length,state_begin, state_length, state_address, state_instruction)" ) ;
 }
 start_offset = offset ;
 operand_address = current_address + 1 ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & p_id_start ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_partial_identifier_start , bytecode_tvb , offset , ( next_operand_address - operand_address ) , p_id_start , "Addr: %u partial_identifier_start %u" , operand_address , p_id_start ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & p_id_length ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_partial_identifier_length , bytecode_tvb , offset , ( next_operand_address - operand_address ) , p_id_length , "Addr: %u partial_identifier_length %u" , operand_address , p_id_length ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & state_begin ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_state_begin , bytecode_tvb , offset , ( next_operand_address - operand_address ) , state_begin , "Addr: %u state_begin %u" , operand_address , state_begin ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & state_length ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_state_length , bytecode_tvb , offset , ( next_operand_address - operand_address ) , state_length , "Addr: %u state_length %u" , operand_address , state_length ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & state_address ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_state_address , bytecode_tvb , offset , ( next_operand_address - operand_address ) , state_address , "Addr: %u state_address %u" , operand_address , state_address ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & state_instruction ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_state_instr , bytecode_tvb , offset , ( next_operand_address - operand_address ) , state_instruction , "Addr: %u state_instruction %u" , operand_address , state_instruction ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 if ( show_instr_detail_level == 1 ) {
 proto_tree_add_none_format ( udvm_tree , hf_sigcomp_decompress_instruction , bytecode_tvb , start_offset , offset - start_offset , "Addr: %u ## STATE-ACCESS(31) (partial_identifier_start=%u, partial_identifier_length=%u,state_begin=%u, state_length=%u, state_address=%u, state_instruction=%u)" , current_address , p_id_start , p_id_length , state_begin , state_length , state_address , state_instruction ) ;
 }
 current_address = next_operand_address ;
 byte_copy_right = buff [ 66 ] << 8 ;
 byte_copy_right = byte_copy_right | buff [ 67 ] ;
 byte_copy_left = buff [ 64 ] << 8 ;
 byte_copy_left = byte_copy_left | buff [ 65 ] ;
 if ( print_level_2 ) {
 proto_tree_add_bytes_format ( udvm_tree , hf_sigcomp_byte_copy , message_tvb , input_address , 1 , NULL , " byte_copy_right = %u, byte_copy_left = %u" , byte_copy_right , byte_copy_left ) ;
 }
 result_code = udvm_state_access ( message_tvb , udvm_tree , buff , p_id_start , p_id_length , state_begin , & state_length , & state_address , & state_instruction , hf_id ) ;
 if ( result_code != 0 ) {
 goto decompression_failure ;
 }
 used_udvm_cycles = used_udvm_cycles + state_length ;
 goto execute_next_instruction ;
 break ;
 case SIGCOMP_INSTR_STATE_CREATE : if ( show_instr_detail_level == 2 ) {
 proto_item_append_text ( addr_item , " (state_length, state_address, state_instruction,minimum_access_length, state_retention_priority)" ) ;
 }
 start_offset = offset ;
 operand_address = current_address + 1 ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & state_length ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_state_length , bytecode_tvb , offset , ( next_operand_address - operand_address ) , state_length , "Addr: %u state_length %u" , operand_address , state_length ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & state_address ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_state_address , bytecode_tvb , offset , ( next_operand_address - operand_address ) , state_address , "Addr: %u state_address %u" , operand_address , state_address ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & state_instruction ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_state_instr , bytecode_tvb , offset , ( next_operand_address - operand_address ) , state_instruction , "Addr: %u state_instruction %u" , operand_address , state_instruction ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & minimum_access_length ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_min_acc_len , bytecode_tvb , offset , ( next_operand_address - operand_address ) , minimum_access_length , "Addr: %u minimum_access_length %u" , operand_address , minimum_access_length ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & state_retention_priority ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_state_ret_pri , bytecode_tvb , offset , ( next_operand_address - operand_address ) , state_retention_priority , "Addr: %u state_retention_priority %u" , operand_address , state_retention_priority ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 if ( show_instr_detail_level == 1 ) {
 proto_tree_add_none_format ( udvm_tree , hf_sigcomp_decompress_instruction , bytecode_tvb , start_offset , offset - start_offset , "Addr: %u ## STATE-CREATE(32) (state_length=%u, state_address=%u, state_instruction=%u,minimum_access_length=%u, state_retention_priority=%u)" , current_address , state_length , state_address , state_instruction , minimum_access_length , state_retention_priority ) ;
 }
 current_address = next_operand_address ;
 no_of_state_create ++ ;
 if ( no_of_state_create > 4 ) {
 result_code = 12 ;
 goto decompression_failure ;
 }
 if ( ( minimum_access_length < 6 ) || ( minimum_access_length > STATE_BUFFER_SIZE ) ) {
 result_code = 1 ;
 goto decompression_failure ;
 }
 if ( state_retention_priority == 65535 ) {
 result_code = 13 ;
 goto decompression_failure ;
 }
 state_length_buff [ no_of_state_create ] = state_length ;
 state_address_buff [ no_of_state_create ] = state_address ;
 state_instruction_buff [ no_of_state_create ] = state_instruction ;
 state_minimum_access_length_buff [ no_of_state_create ] = minimum_access_length ;
 used_udvm_cycles = used_udvm_cycles + state_length ;
 byte_copy_right = buff [ 66 ] << 8 ;
 byte_copy_right = byte_copy_right | buff [ 67 ] ;
 byte_copy_left = buff [ 64 ] << 8 ;
 byte_copy_left = byte_copy_left | buff [ 65 ] ;
 n = 0 ;
 k = state_address ;
 while ( n < state_length ) {
 if ( k == byte_copy_right ) {
 k = byte_copy_left ;
 }
 string [ 0 ] = buff [ k ] ;
 string [ 1 ] = '\0' ;
 if ( print_level_3 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_sigcomp_state_value , bytecode_tvb , 0 , 0 , buff [ k ] , " Addr: %5u State value: %u (0x%x) ASCII(%s)" , k , buff [ k ] , buff [ k ] , format_text ( string , 1 ) ) ;
 }
 k = ( k + 1 ) & 0xffff ;
 n ++ ;
 }
 goto execute_next_instruction ;
 break ;
 case SIGCOMP_INSTR_STATE_FREE : if ( show_instr_detail_level == 2 ) {
 proto_item_append_text ( addr_item , " (partial_identifier_start, partial_identifier_length)" ) ;
 }
 start_offset = offset ;
 operand_address = current_address + 1 ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & p_id_start ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_partial_identifier_start , bytecode_tvb , offset , ( next_operand_address - operand_address ) , p_id_start , "Addr: %u partial_identifier_start %u" , operand_address , p_id_start ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & p_id_length ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_partial_identifier_length , bytecode_tvb , offset , ( next_operand_address - operand_address ) , p_id_length , "Addr: %u partial_identifier_length %u" , operand_address , p_id_length ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 if ( show_instr_detail_level == 1 ) {
 proto_tree_add_none_format ( udvm_tree , hf_sigcomp_decompress_instruction , bytecode_tvb , start_offset , offset - start_offset , "Addr: %u ## STATE-FREE (partial_identifier_start=%u, partial_identifier_length=%u)" , current_address , p_id_start , p_id_length ) ;
 }
 current_address = next_operand_address ;
 udvm_state_free ( buff , p_id_start , p_id_length ) ;
 goto execute_next_instruction ;
 break ;
 case SIGCOMP_INSTR_OUTPUT : if ( show_instr_detail_level == 2 ) {
 proto_item_append_text ( addr_item , " (output_start, output_length)" ) ;
 }
 start_offset = offset ;
 operand_address = current_address + 1 ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & output_start ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_output_start , bytecode_tvb , offset , ( next_operand_address - operand_address ) , output_start , "Addr: %u output_start %u" , operand_address , output_start ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & output_length ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_output_length , bytecode_tvb , offset , ( next_operand_address - operand_address ) , output_length , "Addr: %u output_length %u" , operand_address , output_length ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 if ( show_instr_detail_level == 1 ) {
 proto_tree_add_none_format ( udvm_tree , hf_sigcomp_decompress_instruction , bytecode_tvb , start_offset , offset - start_offset , "Addr: %u ## OUTPUT (output_start=%u, output_length=%u)" , current_address , output_start , output_length ) ;
 }
 current_address = next_operand_address ;
 n = 0 ;
 k = output_start ;
 byte_copy_right = buff [ 66 ] << 8 ;
 byte_copy_right = byte_copy_right | buff [ 67 ] ;
 byte_copy_left = buff [ 64 ] << 8 ;
 byte_copy_left = byte_copy_left | buff [ 65 ] ;
 if ( print_level_3 ) {
 proto_tree_add_bytes_format ( udvm_tree , hf_sigcomp_byte_copy , bytecode_tvb , 0 , - 1 , NULL , " byte_copy_right = %u" , byte_copy_right ) ;
 }
 while ( n < output_length ) {
 if ( k == byte_copy_right ) {
 k = byte_copy_left ;
 }
 out_buff [ output_address ] = buff [ k ] ;
 string [ 0 ] = buff [ k ] ;
 string [ 1 ] = '\0' ;
 if ( print_level_3 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_sigcomp_output_value , bytecode_tvb , 0 , - 1 , buff [ k ] , " Output value: %u (0x%x) ASCII(%s) from Addr: %u ,output to dispatcher position %u" , buff [ k ] , buff [ k ] , format_text ( string , 1 ) , k , output_address ) ;
 }
 k = ( k + 1 ) & 0xffff ;
 output_address ++ ;
 n ++ ;
 }
 used_udvm_cycles = used_udvm_cycles + output_length ;
 goto execute_next_instruction ;
 break ;
 case SIGCOMP_INSTR_END_MESSAGE : if ( show_instr_detail_level == 2 ) {
 proto_item_append_text ( addr_item , " (requested_feedback_location,state_instruction, minimum_access_length,state_retention_priority)" ) ;
 }
 start_offset = offset ;
 operand_address = current_address + 1 ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & requested_feedback_location ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_req_feedback_loc , bytecode_tvb , offset , ( next_operand_address - operand_address ) , requested_feedback_location , "Addr: %u requested_feedback_location %u" , operand_address , requested_feedback_location ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & returned_parameters_location ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_ret_param_loc , bytecode_tvb , offset , ( next_operand_address - operand_address ) , returned_parameters_location , "Addr: %u returned_parameters_location %u" , operand_address , returned_parameters_location ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & state_length ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_state_length , bytecode_tvb , offset , ( next_operand_address - operand_address ) , state_length , "Addr: %u state_length %u" , operand_address , state_length ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & state_address ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_state_address , bytecode_tvb , offset , ( next_operand_address - operand_address ) , state_address , "Addr: %u state_address %u" , operand_address , state_address ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & state_instruction ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_state_instr , bytecode_tvb , offset , ( next_operand_address - operand_address ) , state_instruction , "Addr: %u state_instruction %u" , operand_address , state_instruction ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & minimum_access_length ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_min_acc_len , bytecode_tvb , offset , ( next_operand_address - operand_address ) , minimum_access_length , "Addr: %u minimum_access_length %u" , operand_address , minimum_access_length ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 operand_address = next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & state_retention_priority ) ;
 if ( show_instr_detail_level == 2 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_udvm_state_ret_pri , bytecode_tvb , offset , ( next_operand_address - operand_address ) , state_retention_priority , "Addr: %u state_retention_priority %u" , operand_address , state_retention_priority ) ;
 }
 offset += ( next_operand_address - operand_address ) ;
 if ( show_instr_detail_level == 1 ) {
 proto_tree_add_none_format ( udvm_tree , hf_sigcomp_decompress_instruction , bytecode_tvb , start_offset , offset - start_offset , "Addr: %u ## END-MESSAGE (requested_feedback_location=%u, returned_parameters_location=%u, state_length=%u, state_address=%u, state_instruction=%u, minimum_access_length=%u, state_retention_priority=%u)" , current_address , requested_feedback_location , returned_parameters_location , state_length , state_address , state_instruction , minimum_access_length , state_retention_priority ) ;
 }
 no_of_state_create ++ ;
 if ( no_of_state_create > 4 ) {
 result_code = 12 ;
 goto decompression_failure ;
 }
 state_length_buff [ no_of_state_create ] = state_length ;
 state_address_buff [ no_of_state_create ] = state_address ;
 state_instruction_buff [ no_of_state_create ] = state_instruction ;
 state_minimum_access_length_buff [ no_of_state_create ] = minimum_access_length ;
 proto_tree_add_uint ( udvm_tree , hf_sigcomp_num_state_create , bytecode_tvb , 0 , 0 , no_of_state_create ) ;
 if ( no_of_state_create != 0 ) {
 memset ( sha1_digest_buf , 0 , STATE_BUFFER_SIZE ) ;
 n = 1 ;
 byte_copy_right = buff [ 66 ] << 8 ;
 byte_copy_right = byte_copy_right | buff [ 67 ] ;
 byte_copy_left = buff [ 64 ] << 8 ;
 byte_copy_left = byte_copy_left | buff [ 65 ] ;
 while ( n < no_of_state_create + 1 ) {
 sha1buff = ( guint8 * ) g_malloc ( state_length_buff [ n ] + 8 ) ;
 sha1buff [ 0 ] = state_length_buff [ n ] >> 8 ;
 sha1buff [ 1 ] = state_length_buff [ n ] & 0xff ;
 sha1buff [ 2 ] = state_address_buff [ n ] >> 8 ;
 sha1buff [ 3 ] = state_address_buff [ n ] & 0xff ;
 sha1buff [ 4 ] = state_instruction_buff [ n ] >> 8 ;
 sha1buff [ 5 ] = state_instruction_buff [ n ] & 0xff ;
 sha1buff [ 6 ] = state_minimum_access_length_buff [ n ] >> 8 ;
 sha1buff [ 7 ] = state_minimum_access_length_buff [ n ] & 0xff ;
 if ( print_level_3 ) {
 proto_tree_add_bytes_with_length ( udvm_tree , hf_sigcomp_sha1buff , bytecode_tvb , 0 , - 1 , sha1buff , 8 ) ;
 }
 k = state_address_buff [ n ] ;
 for ( x = 0 ;
 x < state_length_buff [ n ] ;
 x ++ ) {
 if ( k == byte_copy_right ) {
 k = byte_copy_left ;
 }
 sha1buff [ 8 + x ] = buff [ k ] ;
 k = ( k + 1 ) & 0xffff ;
 }
 sha1_starts ( & ctx ) ;
 sha1_update ( & ctx , ( guint8 * ) sha1buff , state_length_buff [ n ] + 8 ) ;
 sha1_finish ( & ctx , sha1_digest_buf ) ;
 if ( print_level_3 ) {
 proto_tree_add_bytes_with_length ( udvm_tree , hf_sigcomp_sha1_digest , bytecode_tvb , 0 , - 1 , sha1_digest_buf , STATE_BUFFER_SIZE ) ;
 }
 # if 0 udvm_state_create ( sha1buff , sha1_digest_buf , state_minimum_access_length_buff [ n ] ) ;
 # endif udvm_state_create ( sha1buff , sha1_digest_buf , STATE_MIN_ACCESS_LEN ) ;
 proto_tree_add_item ( udvm_tree , hf_sigcomp_creating_state , bytecode_tvb , 0 , - 1 , ENC_NA ) ;
 proto_tree_add_string ( udvm_tree , hf_id , bytecode_tvb , 0 , 0 , bytes_to_str ( wmem_packet_scope ( ) , sha1_digest_buf , STATE_MIN_ACCESS_LEN ) ) ;
 n ++ ;
 }
 }
 decomp_tvb = tvb_new_child_real_data ( message_tvb , out_buff , output_address , output_address ) ;
 tvb_set_free_cb ( decomp_tvb , g_free ) ;
 add_new_data_source ( pinfo , decomp_tvb , "Decompressed SigComp message" ) ;
 proto_tree_add_item ( udvm_tree , hf_sigcomp_sigcomp_message_decompressed , decomp_tvb , 0 , - 1 , ENC_NA ) ;
 used_udvm_cycles += state_length ;
 proto_tree_add_uint ( udvm_tree , hf_sigcomp_max_udvm_cycles , bytecode_tvb , 0 , 0 , maximum_UDVM_cycles ) ;
 proto_tree_add_uint ( udvm_tree , hf_sigcomp_used_udvm_cycles , bytecode_tvb , 0 , 0 , used_udvm_cycles ) ;
 return decomp_tvb ;
 break ;
 default : expert_add_info_format ( pinfo , addr_item , & ei_sigcomp_invalid_instruction , "Addr %u Invalid instruction: %u (0x%x)" , current_address , current_instruction , current_instruction ) ;
 break ;
 }
 g_free ( out_buff ) ;
 return NULL ;
 decompression_failure : proto_tree_add_expert_format ( udvm_tree , pinfo , & ei_sigcomp_decompression_failure , bytecode_tvb , 0 , - 1 , "DECOMPRESSION FAILURE: %s" , val_to_str ( result_code , result_code_vals , "Unknown (%u)" ) ) ;
 g_free ( out_buff ) ;
 return NULL ;
 }