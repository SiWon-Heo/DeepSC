static int dissect_udvm_reference_operand_memory ( guint8 * buff , guint operand_address , guint16 * value , guint * result_dest ) {
 guint bytecode ;
 guint16 operand ;
 guint offset = operand_address ;
 guint test_bits ;
 guint8 temp_data ;
 guint16 temp_data16 ;
 if ( operand_address >= UDVM_MEMORY_SIZE ) return - 1 ;
 bytecode = buff [ operand_address ] ;
 test_bits = bytecode >> 7 ;
 if ( test_bits == 1 ) {
 test_bits = bytecode >> 6 ;
 if ( test_bits == 2 ) {
 temp_data = buff [ operand_address ] & 0x3f ;
 operand = temp_data << 8 ;
 temp_data = buff [ ( operand_address + 1 ) & 0xffff ] ;
 operand = operand | temp_data ;
 operand = ( operand * 2 ) ;
 * result_dest = operand ;
 temp_data16 = buff [ operand ] << 8 ;
 temp_data16 = temp_data16 | buff [ ( operand + 1 ) & 0xffff ] ;
 * value = temp_data16 ;
 offset = offset + 2 ;
 }
 else {
 operand_address ++ ;
 operand = buff [ operand_address ] << 8 ;
 operand = operand | buff [ ( operand_address + 1 ) & 0xffff ] ;
 * result_dest = operand ;
 temp_data16 = buff [ operand ] << 8 ;
 temp_data16 = temp_data16 | buff [ ( operand + 1 ) & 0xffff ] ;
 * value = temp_data16 ;
 offset = offset + 3 ;
 }
 }
 else {
 operand = ( bytecode & 0x7f ) ;
 operand = ( operand * 2 ) ;
 * result_dest = operand ;
 temp_data16 = buff [ operand ] << 8 ;
 temp_data16 = temp_data16 | buff [ ( operand + 1 ) & 0xffff ] ;
 * value = temp_data16 ;
 offset ++ ;
 }
 if ( offset >= UDVM_MEMORY_SIZE || * result_dest >= UDVM_MEMORY_SIZE - 1 ) return - 1 ;
 return offset ;
 }