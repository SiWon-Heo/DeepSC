static int decomp_dispatch_get_bits ( tvbuff_t * message_tvb , proto_tree * udvm_tree , guint8 bit_order , guint8 * buff , guint16 * old_input_bit_order , guint16 * remaining_bits , guint16 * input_bits , guint * input_address , guint16 length , guint16 * result_code , guint msg_end , gboolean print_level_1 ) {
 guint16 input_bit_order ;
 guint16 bits_still_required = length ;
 guint16 value = 0 ;
 guint8 octet ;
 gint extra_bytes_available = msg_end - * input_address ;
 gint p_bit ;
 gint prev_p_bit = * old_input_bit_order & 0x0001 ;
 gint bits_to_use = 0 ;
 input_bit_order = buff [ 68 ] << 8 ;
 input_bit_order = input_bit_order | buff [ 69 ] ;
 * result_code = 0 ;
 p_bit = ( input_bit_order & 0x0001 ) != 0 ;
 if ( prev_p_bit != p_bit ) {
 * remaining_bits = 0 ;
 * old_input_bit_order = input_bit_order ;
 }
 if ( * remaining_bits + extra_bytes_available * 8 < length ) {
 * result_code = 11 ;
 return 0xfbad ;
 }
 while ( bits_still_required > 0 ) {
 if ( * remaining_bits == 0 ) {
 octet = tvb_get_guint8 ( message_tvb , * input_address ) ;
 if ( print_level_1 ) {
 proto_tree_add_uint_format ( udvm_tree , hf_sigcomp_getting_value , message_tvb , * input_address , 1 , octet , " Getting value: %u (0x%x) From Addr: %u" , octet , octet , * input_address ) ;
 }
 * input_address = * input_address + 1 ;
 if ( p_bit != 0 ) {
 octet = reverse [ octet ] ;
 }
 * input_bits = octet ;
 * remaining_bits = 8 ;
 }
 bits_to_use = bits_still_required < * remaining_bits ? bits_still_required : * remaining_bits ;
 bits_still_required -= bits_to_use ;
 * input_bits <<= bits_to_use ;
 value = ( value << bits_to_use ) | ( ( * input_bits >> 8 ) & 0xFF ) ;
 * remaining_bits -= bits_to_use ;
 * input_bits &= 0x00FF ;
 }
 if ( ( bit_order != 0 ) && ( length <= 16 ) ) {
 guint16 lsb = reverse [ ( value >> 8 ) & 0xFF ] ;
 guint16 msb = reverse [ value & 0xFF ] ;
 value = ( ( msb << 8 ) | lsb ) >> ( 16 - length ) ;
 }
 return value ;
 }