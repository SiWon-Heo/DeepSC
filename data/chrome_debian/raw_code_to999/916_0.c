static int decode_udvm_address_operand ( guint8 * buff , guint operand_address , guint16 * value , guint current_address ) {
 guint32 result ;
 guint16 value1 ;
 gint next_operand_address ;
 next_operand_address = decode_udvm_multitype_operand ( buff , operand_address , & value1 ) ;
 result = value1 & 0xffff ;
 result = result + current_address ;
 * value = result & 0xffff ;
 return next_operand_address ;
 }