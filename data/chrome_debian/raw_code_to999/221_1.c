static int decode_udvm_address_operand ( guint8 * buff , guint operand_address , guint16 * value , guint current_address ) {
 guint32 result ;
 guint16 value1 ;
 guint next_opreand_address ;
 next_opreand_address = decode_udvm_multitype_operand ( buff , operand_address , & value1 ) ;
 result = value1 & 0xffff ;
 result = result + current_address ;
 * value = result & 0xffff ;
 return next_opreand_address ;
 }