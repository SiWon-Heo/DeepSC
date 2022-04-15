static int udvm_state_access ( tvbuff_t * tvb , proto_tree * tree , guint8 * buff , guint16 p_id_start , guint16 p_id_length , guint16 state_begin , guint16 * state_length , guint16 * state_address , guint16 * state_instruction , gint hf_id ) {
 int result_code = 0 ;
 guint32 n ;
 guint16 k ;
 guint16 buf_size_real ;
 guint16 byte_copy_right ;
 guint16 byte_copy_left ;
 char partial_state [ STATE_BUFFER_SIZE ] ;
 guint8 * state_buff ;
 gchar * partial_state_str ;
 if ( ( p_id_length < STATE_MIN_ACCESS_LEN ) || ( p_id_length > STATE_BUFFER_SIZE ) ) {
 result_code = 1 ;
 return result_code ;
 }
 n = 0 ;
 while ( n < p_id_length && n < STATE_BUFFER_SIZE && p_id_start + n < UDVM_MEMORY_SIZE ) {
 partial_state [ n ] = buff [ p_id_start + n ] ;
 n ++ ;
 }
 partial_state_str = bytes_to_str ( wmem_packet_scope ( ) , partial_state , p_id_length ) ;
 proto_tree_add_item ( tree , hf_sigcomp_accessing_state , tvb , 0 , - 1 , ENC_NA ) ;
 proto_tree_add_string ( tree , hf_id , tvb , 0 , 0 , partial_state_str ) ;
 state_buff = ( guint8 * ) g_hash_table_lookup ( state_buffer_table , partial_state_str ) ;
 if ( state_buff == NULL ) {
 result_code = 2 ;
 return result_code ;
 }
 buf_size_real = ( state_buff [ 0 ] << 8 ) | state_buff [ 1 ] ;
 if ( * state_length == 0 ) {
 * state_length = buf_size_real ;
 }
 if ( * state_address == 0 ) {
 * state_address = state_buff [ 2 ] << 8 ;
 * state_address = * state_address | state_buff [ 3 ] ;
 }
 if ( * state_instruction == 0 ) {
 * state_instruction = state_buff [ 4 ] << 8 ;
 * state_instruction = * state_instruction | state_buff [ 5 ] ;
 }
 if ( ( state_begin + * state_length ) > buf_size_real ) {
 return 3 ;
 }
 if ( * state_length == 0 && state_begin != 0 ) {
 return 17 ;
 }
 n = state_begin + 8 ;
 k = * state_address ;
 byte_copy_right = buff [ 66 ] << 8 ;
 byte_copy_right = byte_copy_right | buff [ 67 ] ;
 byte_copy_left = buff [ 64 ] << 8 ;
 byte_copy_left = byte_copy_left | buff [ 65 ] ;
 while ( ( gint32 ) n < ( state_begin + * state_length + 8 ) && n < UDVM_MEMORY_SIZE ) {
 buff [ k ] = state_buff [ n ] ;
 k = ( k + 1 ) & 0xffff ;
 if ( k == byte_copy_right ) {
 k = byte_copy_left ;
 }
 n ++ ;
 }
 return 0 ;
 }