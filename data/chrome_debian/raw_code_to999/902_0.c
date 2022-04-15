void proto_register_sigcomp ( void ) {
 static hf_register_info hf [ ] = {
 {
 & hf_sigcomp_t_bit , {
 "T bit" , "sigcomp.t.bit" , FT_UINT8 , BASE_DEC , NULL , 0x04 , "Sigcomp T bit" , HFILL }
 }
 , {
 & hf_sigcomp_len , {
 "Partial state id length" , "sigcomp.length" , FT_UINT8 , BASE_HEX , VALS ( length_encoding_vals ) , 0x03 , "Sigcomp length" , HFILL }
 }
 , {
 & hf_sigcomp_returned_feedback_item , {
 "Returned_feedback item" , "sigcomp.returned.feedback.item" , FT_BYTES , BASE_NONE , NULL , 0x0 , "Returned feedback item" , HFILL }
 }
 , {
 & hf_sigcomp_partial_state , {
 "Partial state identifier" , "sigcomp.partial.state.identifier" , FT_STRING , BASE_NONE , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_sigcomp_remaining_message_bytes , {
 "Remaining SigComp message bytes" , "sigcomp.remaining-bytes" , FT_UINT32 , BASE_DEC , NULL , 0x0 , "Number of bytes remaining in message" , HFILL }
 }
 , {
 & hf_sigcomp_compression_ratio , {
 "Compression ratio (%)" , "sigcomp.compression-ratio" , FT_UINT32 , BASE_DEC , NULL , 0x0 , "Compression ratio (decompressed / compressed) %" , HFILL }
 }
 , {
 & hf_sigcomp_returned_feedback_item_len , {
 "Returned feedback item length" , "sigcomp.returned.feedback.item.len" , FT_UINT8 , BASE_DEC , NULL , 0x7f , NULL , HFILL }
 }
 , {
 & hf_sigcomp_code_len , {
 "Code length" , "sigcomp.code.len" , FT_UINT16 , BASE_HEX , NULL , 0xfff0 , NULL , HFILL }
 }
 , {
 & hf_sigcomp_destination , {
 "Destination" , "sigcomp.destination" , FT_UINT8 , BASE_HEX | BASE_EXT_STRING , & destination_address_encoding_vals_ext , 0xf , NULL , HFILL }
 }
 , {
 & hf_sigcomp_udvm_bytecode , {
 "Uploaded UDVM bytecode" , "sigcomp.udvm.byte-code" , FT_NONE , BASE_NONE , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_sigcomp_udvm_instr , {
 "UDVM instruction code" , "sigcomp.udvm.instr" , FT_UINT8 , BASE_DEC | BASE_EXT_STRING , & udvm_instruction_code_vals_ext , 0x0 , NULL , HFILL }
 }
 , {
 & hf_udvm_execution_trace , {
 "UDVM execution trace" , "sigcomp.udvm.execution-trace" , FT_NONE , BASE_NONE , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_udvm_multitype_bytecode , {
 "UDVM bytecode" , "sigcomp.udvm.multyt.bytecode" , FT_UINT8 , BASE_HEX , VALS ( display_bytecode_vals ) , 0x0 , NULL , HFILL }
 }
 , {
 & hf_udvm_reference_bytecode , {
 "UDVM bytecode" , "sigcomp.udvm.ref.bytecode" , FT_UINT8 , BASE_HEX , VALS ( display_ref_bytecode_vals ) , 0x0 , NULL , HFILL }
 }
 , {
 & hf_udvm_literal_bytecode , {
 "UDVM bytecode" , "sigcomp.udvm.lit.bytecode" , FT_UINT8 , BASE_HEX , VALS ( display_lit_bytecode_vals ) , 0x0 , NULL , HFILL }
 }
 , # if 0 {
 & hf_udvm_operand , {
 "UDVM operand" , "sigcomp.udvm.operand" , FT_UINT16 , BASE_DEC , NULL , 0x0 , NULL , HFILL }
 }
 , # endif {
 & hf_udvm_length , {
 "%Length" , "sigcomp.udvm.length" , FT_UINT16 , BASE_DEC , NULL , 0x0 , "Length" , HFILL }
 }
 , {
 & hf_udvm_addr_length , {
 "%Length[memory address]" , "sigcomp.udvm.addr.length" , FT_UINT16 , BASE_DEC , NULL , 0x0 , "Length" , HFILL }
 }
 , {
 & hf_udvm_destination , {
 "%Destination" , "sigcomp.udvm.destination" , FT_UINT16 , BASE_DEC , NULL , 0x0 , "Destination" , HFILL }
 }
 , {
 & hf_udvm_addr_destination , {
 "%Destination[memory address]" , "sigcomp.udvm.addr.destination" , FT_UINT16 , BASE_DEC , NULL , 0x0 , "Destination" , HFILL }
 }
 , {
 & hf_udvm_at_address , {
 "@Address(mem_add_of_inst + D) mod 2^16)" , "sigcomp.udvm.at.address" , FT_UINT16 , BASE_DEC , NULL , 0x0 , "Address" , HFILL }
 }
 , {
 & hf_udvm_address , {
 "%Address" , "sigcomp.udvm.length" , FT_UINT16 , BASE_DEC , NULL , 0x0 , "Address" , HFILL }
 }
 , {
 & hf_udvm_literal_num , {
 "#n" , "sigcomp.udvm.literal-num" , FT_UINT16 , BASE_DEC , NULL , 0x0 , "Literal number" , HFILL }
 }
 , {
 & hf_udvm_value , {
 "%Value" , "sigcomp.udvm.value" , FT_UINT16 , BASE_DEC , NULL , 0x0 , "Value" , HFILL }
 }
 , {
 & hf_udvm_addr_value , {
 "%Value[memory address]" , "sigcomp.udvm.value" , FT_UINT16 , BASE_DEC , NULL , 0x0 , "Value" , HFILL }
 }
 , {
 & hf_partial_identifier_start , {
 "%Partial identifier start" , "sigcomp.udvm.partial.identifier.start" , FT_UINT16 , BASE_DEC , NULL , 0x0 , "Partial identifier start" , HFILL }
 }
 , {
 & hf_partial_identifier_length , {
 "%Partial identifier length" , "sigcomp.udvm.partial.identifier.length" , FT_UINT16 , BASE_DEC , NULL , 0x0 , "Partial identifier length" , HFILL }
 }
 , {
 & hf_state_begin , {
 "%State begin" , "sigcomp.udvm.state.begin" , FT_UINT16 , BASE_DEC , NULL , 0x0 , "State begin" , HFILL }
 }
 , {
 & hf_udvm_state_length , {
 "%State length" , "sigcomp.udvm.state.length" , FT_UINT16 , BASE_DEC , NULL , 0x0 , "State length" , HFILL }
 }
 , {
 & hf_udvm_state_length_addr , {
 "%State length[memory address]" , "sigcomp.udvm.state.length.addr" , FT_UINT16 , BASE_DEC , NULL , 0x0 , "State length" , HFILL }
 }
 , {
 & hf_udvm_state_address , {
 "%State address" , "sigcomp.udvm.start.address" , FT_UINT16 , BASE_DEC , NULL , 0x0 , "State address" , HFILL }
 }
 , {
 & hf_udvm_state_address_addr , {
 "%State address[memory address]" , "sigcomp.udvm.start.address.addr" , FT_UINT16 , BASE_DEC , NULL , 0x0 , "State address" , HFILL }
 }
 , {
 & hf_udvm_state_instr , {
 "%State instruction" , "sigcomp.udvm.start.instr" , FT_UINT16 , BASE_DEC , NULL , 0x0 , "State instruction" , HFILL }
 }
 , {
 & hf_udvm_operand_1 , {
 "$Operand 1[memory address]" , "sigcomp.udvm.operand.1" , FT_UINT16 , BASE_DEC , NULL , 0x0 , "Reference $ Operand 1" , HFILL }
 }
 , {
 & hf_udvm_operand_2 , {
 "%Operand 2" , "sigcomp.udvm.operand.2" , FT_UINT16 , BASE_DEC , NULL , 0x0 , "Operand 2" , HFILL }
 }
 , {
 & hf_udvm_operand_2_addr , {
 "%Operand 2[memory address]" , "sigcomp.udvm.operand.2.addr" , FT_UINT16 , BASE_DEC , NULL , 0x0 , "Operand 2" , HFILL }
 }
 , {
 & hf_udvm_j , {
 "%j" , "sigcomp.udvm.j" , FT_UINT16 , BASE_DEC , NULL , 0x0 , "j" , HFILL }
 }
 , {
 & hf_udvm_addr_j , {
 "%j[memory address]" , "sigcomp.udvm.addr.j" , FT_UINT16 , BASE_DEC , NULL , 0x0 , "j" , HFILL }
 }
 , {
 & hf_udvm_output_start , {
 "%Output_start" , "sigcomp.output.start" , FT_UINT16 , BASE_DEC , NULL , 0x0 , "Output start" , HFILL }
 }
 , {
 & hf_udvm_addr_output_start , {
 "%Output_start[memory address]" , "sigcomp.addr.output.start" , FT_UINT16 , BASE_DEC , NULL , 0x0 , "Output start" , HFILL }
 }
 , {
 & hf_udvm_output_length , {
 "%Output_length" , "sigcomp.output.length" , FT_UINT16 , BASE_DEC , NULL , 0x0 , "Output length" , HFILL }
 }
 , {
 & hf_udvm_output_length_addr , {
 "%Output_length[memory address]" , "sigcomp.output.length.addr" , FT_UINT16 , BASE_DEC , NULL , 0x0 , "Output length" , HFILL }
 }
 , {
 & hf_udvm_req_feedback_loc , {
 "%Requested feedback location" , "sigcomp.req.feedback.loc" , FT_UINT16 , BASE_DEC , NULL , 0x0 , "Requested feedback location" , HFILL }
 }
 , {
 & hf_udvm_min_acc_len , {
 "%Minimum access length" , "sigcomp.min.acc.len" , FT_UINT16 , BASE_DEC , NULL , 0x0 , "Minimum access length" , HFILL }
 }
 , {
 & hf_udvm_state_ret_pri , {
 "%State retention priority" , "sigcomp.udvm.state.ret.pri" , FT_UINT16 , BASE_DEC , NULL , 0x0 , "State retention priority" , HFILL }
 }
 , {
 & hf_udvm_ret_param_loc , {
 "%Returned parameters location" , "sigcomp.ret.param.loc" , FT_UINT16 , BASE_DEC , NULL , 0x0 , "Returned parameters location" , HFILL }
 }
 , {
 & hf_udvm_position , {
 "%Position" , "sigcomp.udvm.position" , FT_UINT16 , BASE_DEC , NULL , 0x0 , "Position" , HFILL }
 }
 , {
 & hf_udvm_ref_dest , {
 "$Destination[memory address]" , "sigcomp.udvm.ref.destination" , FT_UINT16 , BASE_DEC , NULL , 0x0 , "(reference)Destination" , HFILL }
 }
 , {
 & hf_udvm_bits , {
 "%Bits" , "sigcomp.udvm.bits" , FT_UINT16 , BASE_DEC , NULL , 0x0 , "Bits" , HFILL }
 }
 , {
 & hf_udvm_lower_bound , {
 "%Lower bound" , "sigcomp.udvm.lower.bound" , FT_UINT16 , BASE_DEC , NULL , 0x0 , "Lower_bound" , HFILL }
 }
 , {
 & hf_udvm_upper_bound , {
 "%Upper bound" , "sigcomp.udvm.upper.bound" , FT_UINT16 , BASE_DEC , NULL , 0x0 , "Upper bound" , HFILL }
 }
 , {
 & hf_udvm_uncompressed , {
 "%Uncompressed" , "sigcomp.udvm.uncompressed" , FT_UINT16 , BASE_DEC , NULL , 0x0 , "Uncompressed" , HFILL }
 }
 , {
 & hf_udvm_start_value , {
 "%Start value" , "sigcomp.udvm.start.value" , FT_UINT16 , BASE_DEC , NULL , 0x0 , "Start value" , HFILL }
 }
 , {
 & hf_udvm_offset , {
 "%Offset" , "sigcomp.udvm.offset" , FT_UINT16 , BASE_DEC , NULL , 0x0 , "Offset" , HFILL }
 }
 , {
 & hf_udvm_addr_offset , {
 "%Offset[memory address]" , "sigcomp.udvm.addr.offset" , FT_UINT16 , BASE_DEC , NULL , 0x0 , "Offset" , HFILL }
 }
 , {
 & hf_sigcomp_nack_ver , {
 "NACK Version" , "sigcomp.nack.ver" , FT_UINT8 , BASE_DEC , NULL , 0x0f , NULL , HFILL }
 }
 , {
 & hf_sigcomp_nack_reason_code , {
 "Reason Code" , "sigcomp.nack.reason" , FT_UINT8 , BASE_DEC | BASE_EXT_STRING , & sigcomp_nack_reason_code_vals_ext , 0x0 , "NACK Reason Code" , HFILL }
 }
 , {
 & hf_sigcomp_nack_failed_op_code , {
 "OPCODE of failed instruction" , "sigcomp.nack.failed_op_code" , FT_UINT8 , BASE_DEC | BASE_EXT_STRING , & udvm_instruction_code_vals_ext , 0x0 , "NACK OPCODE of failed instruction" , HFILL }
 }
 , {
 & hf_sigcomp_nack_pc , {
 "PC of failed instruction" , "sigcomp.nack.pc" , FT_UINT16 , BASE_DEC , NULL , 0x0 , "NACK PC of failed instruction" , HFILL }
 }
 , {
 & hf_sigcomp_nack_sha1 , {
 "SHA-1 Hash of failed message" , "sigcomp.nack.sha1" , FT_BYTES , BASE_NONE , NULL , 0x0 , "NACK SHA-1 Hash of failed message" , HFILL }
 }
 , {
 & hf_sigcomp_nack_state_id , {
 "State ID (6 - 20 bytes)" , "sigcomp.nack.state_id" , FT_BYTES , BASE_NONE , NULL , 0x0 , "NACK State ID (6 - 20 bytes)" , HFILL }
 }
 , {
 & hf_sigcomp_nack_cycles_per_bit , {
 "Cycles Per Bit" , "sigcomp.nack.cycles_per_bit" , FT_UINT8 , BASE_DEC , NULL , 0x0 , "NACK Cycles Per Bit" , HFILL }
 }
 , {
 & hf_sigcomp_nack_memory_size , {
 "Memory size" , "sigcomp.memory_size" , FT_UINT16 , BASE_DEC , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_sigcomp_decompress_instruction , {
 "Instruction" , "sigcomp.decompress_instruction" , FT_NONE , BASE_NONE , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_sigcomp_loading_result , {
 "Loading result" , "sigcomp.loading_result" , FT_NONE , BASE_NONE , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_sigcomp_byte_copy , {
 "byte copy" , "sigcomp.byte_copy" , FT_BYTES , BASE_NONE , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_sigcomp_accessing_state , {
 "### Accessing state ###" , "sigcomp.accessing_state" , FT_BYTES , BASE_NONE , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_sigcomp_getting_value , {
 "Getting value" , "sigcomp.getting_value" , FT_UINT8 , BASE_HEX , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_sigcomp_load_bytecode_into_udvm_start , {
 "Load bytecode into UDVM starting at" , "sigcomp.load_bytecode_into_udvm_start" , FT_UINT8 , BASE_DEC , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_sigcomp_instruction_code , {
 "Instruction code" , "sigcomp.instruction_code" , FT_UINT8 , BASE_HEX , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_sigcomp_current_instruction , {
 "Addr" , "sigcomp.current_instruction" , FT_UINT8 , BASE_DEC | BASE_EXT_STRING , & udvm_instruction_code_vals_ext , 0x0 , NULL , HFILL }
 }
 , {
 & hf_sigcomp_decompression_failure , {
 "DECOMPRESSION-FAILURE" , "sigcomp.decompression_failure" , FT_UINT32 , BASE_DEC , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_sigcomp_wireshark_udvm_diagnostic , {
 "Wireshark UDVM diagnostic" , "sigcomp.wireshark_udvm_diagnostic" , FT_UINT32 , BASE_DEC , VALS ( result_code_vals ) , 0x0 , NULL , HFILL }
 }
 , {
 & hf_sigcomp_calculated_sha_1 , {
 "Calculated SHA-1" , "sigcomp.calculated_sha_1" , FT_BYTES , BASE_NONE , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_sigcomp_copying_value , {
 "Copying value" , "sigcomp.copying_value" , FT_UINT8 , BASE_HEX , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_sigcomp_storing_value , {
 "Storing value" , "sigcomp.storing_value" , FT_UINT8 , BASE_HEX , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_sigcomp_loading_value , {
 "Loading value" , "sigcomp.loading_value" , FT_UINT8 , BASE_HEX , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_sigcomp_set_hu , {
 "Set Hu" , "sigcomp.set_hu" , FT_BYTES , BASE_NONE , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_sigcomp_loading_h , {
 "Loading H" , "sigcomp.loading_h" , FT_UINT8 , BASE_HEX , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_sigcomp_state_value , {
 "Addr" , "sigcomp.state_value" , FT_UINT8 , BASE_HEX , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_sigcomp_output_value , {
 "Output value" , "sigcomp.output_value" , FT_UINT8 , BASE_HEX , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_sigcomp_num_state_create , {
 "no_of_state_create" , "sigcomp.num_state_create" , FT_UINT32 , BASE_DEC , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_sigcomp_sha1_digest , {
 "SHA1 digest" , "sigcomp.sha1_digest" , FT_BYTES , BASE_NONE , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_sigcomp_creating_state , {
 "### Creating state ###" , "sigcomp.creating_state" , FT_BYTES , BASE_NONE , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_sigcomp_sigcomp_message_decompressed , {
 "SigComp message Decompressed" , "sigcomp.message_decompressed" , FT_BYTES , BASE_NONE , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_sigcomp_starting_to_remove_escape_digits , {
 "Starting to remove escape digits" , "sigcomp.starting_to_remove_escape_digits" , FT_NONE , BASE_NONE , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_sigcomp_escape_digit_found , {
 "Escape digit found" , "sigcomp.escape_digit_found" , FT_NONE , BASE_NONE , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_sigcomp_illegal_escape_code , {
 "Illegal escape code" , "sigcomp.illegal_escape_code" , FT_NONE , BASE_NONE , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_sigcomp_end_of_sigcomp_message_indication_found , {
 "End of SigComp message indication found" , "sigcomp.end_of_sigcomp_message_indication_found" , FT_NONE , BASE_NONE , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_sigcomp_addr_value , {
 "Addr" , "sigcomp.addr" , FT_UINT8 , BASE_HEX , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_sigcomp_copying_bytes_literally , {
 "Copying bytes literally" , "sigcomp.copying_bytes_literally" , FT_BYTES , BASE_NONE , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_sigcomp_data_for_sigcomp_dissector , {
 "Data handed to the Sigcomp dissector" , "sigcomp.data_for_sigcomp_dissector" , FT_BYTES , BASE_NONE , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_sigcomp_remaining_sigcomp_message , {
 "Remaining SigComp message" , "sigcomp.remaining_sigcomp_message" , FT_BYTES , BASE_NONE , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_sigcomp_sha1buff , {
 "sha1buff" , "sigcomp.sha1buff" , FT_BYTES , BASE_NONE , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_sigcomp_udvm_instruction , {
 "UDVM instruction" , "sigcomp.udvm_instruction" , FT_UINT8 , BASE_HEX , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_sigcomp_remaining_bytes , {
 "Remaining bytes" , "sigcomp.remaining_bytes" , FT_BYTES , BASE_NONE , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_sigcomp_max_udvm_cycles , {
 "maximum_UDVM_cycles" , "sigcomp.max_udvm_cycles" , FT_UINT32 , BASE_DEC , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_sigcomp_used_udvm_cycles , {
 "used_udvm_cycles" , "sigcomp.used_udvm_cycles" , FT_UINT32 , BASE_DEC , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_sigcomp_udvm_execution_stated , {
 "UDVM EXECUTION STARTED" , "sigcomp.udvm_execution_stated" , FT_UINT8 , BASE_DEC , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_sigcomp_message_length , {
 "Message Length" , "sigcomp.message_length" , FT_UINT8 , BASE_DEC , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_sigcomp_byte_code_length , {
 "Byte code length" , "sigcomp.byte_code_length" , FT_UINT8 , BASE_DEC , NULL , 0x0 , NULL , HFILL }
 }
 , }
 ;
 static gint * ett [ ] = {
 & ett_sigcomp , & ett_sigcomp_udvm , & ett_sigcomp_udvm_exe , }
 ;
 static gint * ett_raw [ ] = {
 & ett_raw_text , }
 ;
 static ei_register_info ei [ ] = {
 {
 & ei_sigcomp_nack_failed_op_code , {
 "sigcomp.nack.failed_op_code.expert" , PI_SEQUENCE , PI_WARN , "SigComp NACK" , EXPFILL }
 }
 , {
 & ei_sigcomp_invalid_instruction , {
 "sigcomp.invalid_instruction" , PI_PROTOCOL , PI_WARN , "Invalid instruction" , EXPFILL }
 }
 , {
 & ei_sigcomp_invalid_shift_value , {
 "sigcomp.invalid_shift_value" , PI_PROTOCOL , PI_WARN , "Invalid shift value" , EXPFILL }
 }
 , {
 & ei_sigcomp_sigcomp_message_decompression_failure , {
 "sigcomp.message_decompression_failure" , PI_PROTOCOL , PI_WARN , "SigComp message Decompression failure" , EXPFILL }
 }
 , {
 & ei_sigcomp_execution_of_this_instruction_is_not_implemented , {
 "sigcomp.execution_of_this_instruction_is_not_implemented" , PI_UNDECODED , PI_WARN , "Execution of this instruction is NOT implemented" , EXPFILL }
 }
 , {
 & ei_sigcomp_decompression_failure , {
 "sigcomp.decompression_failure_expert" , PI_PROTOCOL , PI_WARN , "DECOMPRESSION FAILURE" , EXPFILL }
 }
 , {
 & ei_sigcomp_tcp_fragment , {
 "sigcomp.tcp_fragment" , PI_MALFORMED , PI_ERROR , "TCP Fragment" , EXPFILL }
 }
 , {
 & ei_sigcomp_failed_to_access_state_wireshark_udvm_diagnostic , {
 "sigcomp.failed_to_access_state_wireshark_udvm_diagnostic" , PI_PROTOCOL , PI_WARN , "Failed to Access state Wireshark UDVM diagnostic" , EXPFILL }
 }
 , {
 & ei_sigcomp_all_remaining_parameters_zero , {
 "sigcomp.all_remaining_parameters" , PI_PROTOCOL , PI_NOTE , "All remaining parameters = 0(Not in the uploaded code as UDVM buffer initialized to Zero" , EXPFILL }
 }
 , }
 ;
 module_t * sigcomp_module ;
 expert_module_t * expert_sigcomp ;
 static const enum_val_t udvm_detail_vals [ ] = {
 {
 "no-printout" , "No-Printout" , 0 }
 , {
 "low-detail" , "Low-detail" , 1 }
 , {
 "medium-detail" , "Medium-detail" , 2 }
 , {
 "high-detail" , "High-detail" , 3 }
 , {
 NULL , NULL , - 1 }
 }
 ;
 proto_sigcomp = proto_register_protocol ( "Signaling Compression" , "SIGCOMP" , "sigcomp" ) ;
 proto_raw_sigcomp = proto_register_protocol ( "Decompressed SigComp message as raw text" , "Raw_SigComp" , "raw_sigcomp" ) ;
 register_dissector ( "sigcomp" , dissect_sigcomp , proto_sigcomp ) ;
 proto_register_field_array ( proto_sigcomp , hf , array_length ( hf ) ) ;
 proto_register_subtree_array ( ett , array_length ( ett ) ) ;
 proto_register_subtree_array ( ett_raw , array_length ( ett_raw ) ) ;
 expert_sigcomp = expert_register_protocol ( proto_sigcomp ) ;
 expert_register_field_array ( expert_sigcomp , ei , array_length ( ei ) ) ;
 sigcomp_module = prefs_register_protocol ( proto_sigcomp , proto_reg_handoff_sigcomp ) ;
 prefs_register_uint_preference ( sigcomp_module , "udp.port" , "Sigcomp UDP Port 1" , "Set UDP port 1 for SigComp messages" , 10 , & SigCompUDPPort1 ) ;
 prefs_register_uint_preference ( sigcomp_module , "udp.port2" , "Sigcomp UDP Port 2" , "Set UDP port 2 for SigComp messages" , 10 , & SigCompUDPPort2 ) ;
 prefs_register_uint_preference ( sigcomp_module , "tcp.port" , "Sigcomp TCP Port 1" , "Set TCP port 1 for SigComp messages" , 10 , & SigCompTCPPort1 ) ;
 prefs_register_uint_preference ( sigcomp_module , "tcp.port2" , "Sigcomp TCP Port 2" , "Set TCP port 2 for SigComp messages" , 10 , & SigCompTCPPort2 ) ;
 prefs_register_bool_preference ( sigcomp_module , "display.udvm.code" , "Dissect the UDVM code" , "Preference whether to Dissect the UDVM code or not" , & dissect_udvm_code ) ;
 prefs_register_bool_preference ( sigcomp_module , "display.bytecode" , "Display the bytecode of operands" , "preference whether to display the bytecode in " "UDVM operands or not" , & display_udvm_bytecode ) ;
 prefs_register_bool_preference ( sigcomp_module , "decomp.msg" , "Decompress message" , "preference whether to decompress message or not" , & decompress ) ;
 prefs_register_bool_preference ( sigcomp_module , "display.decomp.msg.as.txt" , "Displays the decompressed message as text" , "preference whether to display the decompressed message " "as raw text or not" , & display_raw_txt ) ;
 prefs_register_enum_preference ( sigcomp_module , "show.udvm.execution" , "Level of detail of UDVM execution:" , "'No-Printout' = UDVM executes silently, then increasing detail " "about execution of UDVM instructions;
 " "Warning! CPU intense at high detail" , & udvm_print_detail_level , udvm_detail_vals , FALSE ) ;
 register_init_routine ( & sigcomp_init_udvm ) ;
 register_cleanup_routine ( & sigcomp_cleanup_udvm ) ;
 }