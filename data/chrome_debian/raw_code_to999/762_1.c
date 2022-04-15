static guint16 dissect_mpa_fpdu ( tvbuff_t * tvb , packet_info * pinfo , proto_tree * tree , mpa_state_t * state , struct tcpinfo * tcpinfo , guint8 endpoint ) {
 proto_item * mpa_item = NULL ;
 proto_item * mpa_header_item = NULL ;
 proto_tree * mpa_tree = NULL ;
 proto_tree * mpa_header_tree = NULL ;
 guint8 pad_length ;
 guint16 ulpdu_length , exp_ulpdu_length ;
 guint32 offset , total_length ;
 guint32 num_of_m = 0 ;
 if ( state -> minfo [ endpoint ] . valid && get_first_marker_offset ( state , tcpinfo , endpoint ) == 0 ) {
 offset = MPA_MARKER_LEN ;
 }
 else {
 offset = 0 ;
 }
 ulpdu_length = ( guint16 ) tvb_get_ntohs ( tvb , offset ) ;
 mpa_packetlist ( pinfo , MPA_FPDU ) ;
 if ( state -> minfo [ endpoint ] . valid ) {
 num_of_m = number_of_markers ( state , tcpinfo , endpoint ) ;
 }
 pad_length = fpdu_pad_length ( ulpdu_length ) ;
 exp_ulpdu_length = expected_ulpdu_length ( state , tcpinfo , endpoint ) ;
 if ( ! exp_ulpdu_length || exp_ulpdu_length != ( ulpdu_length + pad_length ) ) {
 proto_tree_add_expert_format ( tree , pinfo , & ei_mpa_bad_length , tvb , offset , MPA_ULPDU_LENGTH_LEN , "[ULPDU length [%u] field does not contain the expected length[%u]]" , exp_ulpdu_length , ulpdu_length + pad_length ) ;
 }
 mpa_item = proto_tree_add_item ( tree , proto_iwarp_mpa , tvb , 0 , - 1 , ENC_NA ) ;
 mpa_tree = proto_item_add_subtree ( mpa_item , ett_mpa ) ;
 mpa_header_item = proto_tree_add_item ( mpa_tree , hf_mpa_fpdu , tvb , offset , - 1 , ENC_NA ) ;
 mpa_header_tree = proto_item_add_subtree ( mpa_header_item , ett_mpa ) ;
 proto_tree_add_uint ( mpa_header_tree , hf_mpa_ulpdu_length , tvb , offset , MPA_ULPDU_LENGTH_LEN , ulpdu_length ) ;
 if ( state -> minfo [ endpoint ] . valid && num_of_m > 0 ) {
 total_length = fpdu_total_length ( tcpinfo ) ;
 if ( pad_length > 0 ) {
 proto_tree_add_item ( mpa_header_tree , hf_mpa_pad , tvb , pad_offset ( tcpinfo , total_length , pad_length ) , pad_length , ENC_NA ) ;
 }
 dissect_fpdu_crc ( tvb , mpa_header_tree , state , total_length - MPA_CRC_LEN , num_of_m * MPA_MARKER_LEN + ulpdu_length + pad_length + MPA_ULPDU_LENGTH_LEN ) ;
 dissect_fpdu_markers ( tvb , mpa_tree , state , tcpinfo , endpoint ) ;
 }
 else {
 offset += MPA_ULPDU_LENGTH_LEN + ulpdu_length ;
 if ( pad_length > 0 ) {
 proto_tree_add_item ( mpa_header_tree , hf_mpa_pad , tvb , offset , pad_length , ENC_NA ) ;
 offset += pad_length ;
 }
 dissect_fpdu_crc ( tvb , mpa_header_tree , state , offset , ulpdu_length + pad_length + MPA_ULPDU_LENGTH_LEN ) ;
 }
 return ulpdu_length ;
 }