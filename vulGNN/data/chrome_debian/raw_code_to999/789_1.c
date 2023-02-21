static void dissect_q931_cause_ie_unsafe ( tvbuff_t * tvb , int offset , int len , proto_tree * tree , int hf_cause_value , guint8 * cause_value , const value_string * ie_vals ) {
 guint8 octet ;
 guint8 coding_standard ;
 guint8 rejection_reason ;
 if ( len == 0 ) return ;
 octet = tvb_get_guint8 ( tvb , offset ) ;
 coding_standard = octet & 0x60 ;
 if ( coding_standard != Q931_ITU_STANDARDIZED_CODING && ! g931_iso_iec_cause && coding_standard != Q931_ISO_IEC_STANDARDIZED_CODING ) {
 proto_tree_add_uint ( tree , hf_q931_coding_standard , tvb , offset , 1 , octet ) ;
 proto_tree_add_item ( tree , hf_q931_cause_data , tvb , offset , len , ENC_NA ) ;
 return ;
 }
 proto_tree_add_uint ( tree , hf_q931_cause_location , tvb , offset , 1 , octet ) ;
 proto_tree_add_uint ( tree , hf_q931_coding_standard , tvb , offset , 1 , octet ) ;
 proto_tree_add_boolean ( tree , hf_q931_extension_ind , tvb , offset , 1 , octet ) ;
 offset += 1 ;
 len -= 1 ;
 if ( ! ( octet & Q931_IE_VL_EXTENSION ) ) {
 if ( len == 0 ) return ;
 octet = tvb_get_guint8 ( tvb , offset ) ;
 proto_tree_add_item ( tree , hf_q931_cause_recommendation , tvb , offset , 1 , ENC_BIG_ENDIAN ) ;
 proto_tree_add_boolean ( tree , hf_q931_extension_ind , tvb , offset , 1 , octet ) ;
 offset += 1 ;
 len -= 1 ;
 }
 if ( len == 0 ) return ;
 octet = tvb_get_guint8 ( tvb , offset ) ;
 * cause_value = octet & 0x7F ;
 if ( have_valid_q931_pi ) {
 q931_pi -> cause_value = * cause_value ;
 }
 proto_tree_add_uint ( tree , hf_cause_value , tvb , offset , 1 , * cause_value ) ;
 proto_tree_add_boolean ( tree , hf_q931_extension_ind , tvb , offset , 1 , octet ) ;
 offset += 1 ;
 len -= 1 ;
 if ( len == 0 ) return ;
 switch ( * cause_value ) {
 case Q931_CAUSE_UNALLOC_NUMBER : case Q931_CAUSE_NO_ROUTE_TO_DEST : case Q931_CAUSE_QOS_UNAVAILABLE : proto_tree_add_item ( tree , hf_q931_network_service , tvb , offset , 1 , ENC_NA ) ;
 proto_tree_add_item ( tree , hf_q931_extension_condition_type , tvb , offset , 1 , ENC_NA ) ;
 proto_tree_add_item ( tree , hf_q931_extension_condition , tvb , offset , 1 , ENC_BIG_ENDIAN ) ;
 break ;
 case Q931_CAUSE_CALL_REJECTED : rejection_reason = octet & 0x7C ;
 proto_tree_add_item ( tree , hf_q931_cause_call_rejection_reason , tvb , offset , 1 , ENC_BIG_ENDIAN ) ;
 proto_tree_add_item ( tree , hf_q931_cause_call_condition , tvb , offset , 1 , ENC_BIG_ENDIAN ) ;
 offset += 1 ;
 len -= 1 ;
 if ( len == 0 ) return ;
 switch ( rejection_reason ) {
 case Q931_REJ_USER_SPECIFIC : proto_tree_add_item ( tree , hf_q931_cause_call_user_specific_diagnostic , tvb , offset , len , ENC_NA ) ;
 break ;
 case Q931_REJ_IE_MISSING : proto_tree_add_uint_format_value ( tree , hf_q931_missing_info_element , tvb , offset , 1 , tvb_get_guint8 ( tvb , offset ) , "%s" , val_to_str ( tvb_get_guint8 ( tvb , offset ) , ie_vals , "Unknown (0x%02X)" ) ) ;
 break ;
 case Q931_REJ_IE_INSUFFICIENT : proto_tree_add_uint_format_value ( tree , hf_q931_insufficient_info_element , tvb , offset , 1 , tvb_get_guint8 ( tvb , offset ) , "%s" , val_to_str ( tvb_get_guint8 ( tvb , offset ) , ie_vals , "Unknown (0x%02X)" ) ) ;
 break ;
 default : proto_tree_add_item ( tree , hf_q931_cause_call_diagnostic , tvb , offset , len , ENC_NA ) ;
 break ;
 }
 break ;
 case Q931_CAUSE_ACCESS_INFO_DISC : case Q931_CAUSE_INCOMPATIBLE_DEST : case Q931_CAUSE_MAND_IE_MISSING : case Q931_CAUSE_IE_NONEX_OR_UNIMPL : case Q931_CAUSE_INVALID_IE_CONTENTS : do {
 proto_tree_add_uint_format_value ( tree , hf_q931_information_element , tvb , offset , 1 , tvb_get_guint8 ( tvb , offset ) , "%s" , val_to_str ( tvb_get_guint8 ( tvb , offset ) , ie_vals , "Unknown (0x%02X)" ) ) ;
 offset += 1 ;
 len -= 1 ;
 }
 while ( len != 0 ) ;
 break ;
 case Q931_CAUSE_MT_NONEX_OR_UNIMPL : case Q931_CAUSE_MSG_INCOMPAT_W_CS : proto_tree_add_item ( tree , hf_q931_cause_call_message_type , tvb , offset , 1 , ENC_BIG_ENDIAN ) ;
 break ;
 case Q931_CAUSE_REC_TIMER_EXP : if ( len < 3 ) return ;
 proto_tree_add_item ( tree , hf_q931_cause_call_rec_timer , tvb , offset , 3 , ENC_NA | ENC_ASCII ) ;
 break ;
 default : proto_tree_add_item ( tree , hf_q931_cause_call_diagnostic , tvb , offset , len , ENC_NA ) ;
 }
 }