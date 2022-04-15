static void dissect_q931_IEs ( tvbuff_t * tvb , packet_info * pinfo , proto_tree * root_tree , proto_tree * q931_tree , gboolean is_over_ip , int offset , int initial_codeset ) {
 proto_item * ti ;
 proto_tree * ie_tree = NULL ;
 guint8 info_element ;
 guint8 dummy ;
 guint16 info_element_len ;
 int codeset , locked_codeset ;
 gboolean non_locking_shift , first_segment ;
 tvbuff_t * h225_tvb , * next_tvb ;
 e164_info_t e164_info ;
 e164_info . e164_number_type = NONE ;
 e164_info . nature_of_address = NONE ;
 e164_info . E164_number_str = "" ;
 e164_info . E164_number_length = NONE ;
 codeset = locked_codeset = initial_codeset ;
 first_segment = FALSE ;
 while ( tvb_reported_length_remaining ( tvb , offset ) > 0 ) {
 info_element = tvb_get_guint8 ( tvb , offset ) ;
 if ( ( info_element & Q931_IE_SO_MASK ) && ( ( info_element & Q931_IE_SO_IDENTIFIER_MASK ) == Q931_IE_SHIFT ) ) {
 non_locking_shift = info_element & Q931_IE_SHIFT_NON_LOCKING ;
 codeset = info_element & Q931_IE_SHIFT_CODESET ;
 if ( ! non_locking_shift ) locked_codeset = codeset ;
 if ( q931_tree != NULL ) {
 proto_tree_add_uint_format ( q931_tree , hf_q931_locking_codeset , tvb , offset , 1 , codeset , "%s shift to codeset %u: %s" , ( non_locking_shift ? "Non-locking" : "Locking" ) , codeset , val_to_str ( codeset , q931_codeset_vals , "Unknown (0x%02X)" ) ) ;
 }
 offset += 1 ;
 continue ;
 }
 if ( info_element & Q931_IE_SO_MASK ) {
 if ( dissector_get_uint_handle ( codeset_dissector_table , codeset ) || dissector_get_uint_handle ( ie_dissector_table , ( codeset << 8 ) | ( info_element & Q931_IE_SO_IDENTIFIER_MASK ) ) ) {
 next_tvb = tvb_new_subset_length ( tvb , offset , 1 ) ;
 if ( dissector_try_uint ( ie_dissector_table , ( codeset << 8 ) | ( info_element & Q931_IE_SO_IDENTIFIER_MASK ) , next_tvb , pinfo , q931_tree ) || dissector_try_uint ( codeset_dissector_table , codeset , next_tvb , pinfo , q931_tree ) ) {
 offset += 1 ;
 codeset = locked_codeset ;
 continue ;
 }
 }
 switch ( ( codeset << 8 ) | ( info_element & Q931_IE_SO_IDENTIFIER_MASK ) ) {
 case CS0 | Q931_IE_MORE_DATA_OR_SEND_COMP : switch ( info_element ) {
 case Q931_IE_MORE_DATA : proto_tree_add_item ( q931_tree , hf_q931_more_data , tvb , offset , 1 , ENC_NA ) ;
 break ;
 case Q931_IE_SENDING_COMPLETE : proto_tree_add_item ( q931_tree , hf_q931_sending_complete , tvb , offset , 1 , ENC_NA ) ;
 break ;
 default : proto_tree_add_expert_format ( q931_tree , pinfo , & ei_q931_information_element , tvb , offset , 1 , "Unknown information element (0x%02X)" , info_element ) ;
 break ;
 }
 break ;
 case CS0 | Q931_IE_CONGESTION_LEVEL : proto_tree_add_item ( q931_tree , hf_q931_congestion_level , tvb , offset , 1 , ENC_BIG_ENDIAN ) ;
 break ;
 case CS0 | Q931_IE_REPEAT_INDICATOR : proto_tree_add_item ( q931_tree , hf_q931_repeat_indicator , tvb , offset , 1 , ENC_BIG_ENDIAN ) ;
 break ;
 default : proto_tree_add_expert_format ( q931_tree , pinfo , & ei_q931_information_element , tvb , offset , 1 , "Unknown information element (0x%02X)" , info_element ) ;
 break ;
 }
 offset += 1 ;
 codeset = locked_codeset ;
 continue ;
 }
 if ( is_over_ip && tvb_bytes_exist ( tvb , offset , 4 ) && codeset == 0 && tvb_get_guint8 ( tvb , offset ) == Q931_IE_USER_USER && tvb_get_guint8 ( tvb , offset + 3 ) == Q931_PROTOCOL_DISCRIMINATOR_ASN1 ) {
 info_element_len = tvb_get_ntohs ( tvb , offset + 1 ) ;
 if ( q931_tree != NULL ) {
 ie_tree = proto_tree_add_subtree ( q931_tree , tvb , offset , 1 + 2 + info_element_len , ett_q931_ie [ info_element ] , NULL , val_to_str ( info_element , q931_info_element_vals [ codeset ] , "Unknown information element (0x%02X)" ) ) ;
 proto_tree_add_uint_format_value ( ie_tree , hf_q931_information_element , tvb , offset , 1 , info_element , "%s" , val_to_str ( info_element , q931_info_element_vals [ codeset ] , "Unknown (0x%02X)" ) ) ;
 proto_tree_add_item ( ie_tree , hf_q931_information_element_len , tvb , offset + 1 , 2 , ENC_BIG_ENDIAN ) ;
 proto_tree_add_item ( ie_tree , hf_q931_user_protocol_discriminator , tvb , offset + 3 , 1 , ENC_NA ) ;
 }
 if ( info_element_len > 1 ) {
 if ( ! pinfo -> can_desegment ) {
 info_element_len = MIN ( info_element_len , tvb_captured_length_remaining ( tvb , offset + 3 ) ) ;
 }
 if ( h225_handle != NULL ) {
 h225_tvb = tvb_new_subset_length ( tvb , offset + 4 , info_element_len - 1 ) ;
 call_dissector ( h225_handle , h225_tvb , pinfo , root_tree ) ;
 }
 else {
 proto_tree_add_item ( ie_tree , hf_q931_user_information_bytes , tvb , offset + 4 , info_element_len - 1 , ENC_NA ) ;
 }
 }
 offset += 1 + 2 + info_element_len ;
 }
 else {
 info_element_len = tvb_get_guint8 ( tvb , offset + 1 ) ;
 if ( first_segment && ( tvb_reported_length_remaining ( tvb , offset + 2 ) < info_element_len ) ) {
 proto_tree_add_expert ( q931_tree , pinfo , & ei_q931_incomplete_ie , tvb , offset , - 1 ) ;
 break ;
 }
 if ( dissector_get_uint_handle ( codeset_dissector_table , codeset ) || dissector_get_uint_handle ( ie_dissector_table , ( codeset << 8 ) | info_element ) ) {
 next_tvb = tvb_new_subset_length ( tvb , offset , info_element_len + 2 ) ;
 if ( dissector_try_uint ( ie_dissector_table , ( codeset << 8 ) | info_element , next_tvb , pinfo , q931_tree ) || dissector_try_uint ( codeset_dissector_table , codeset , next_tvb , pinfo , q931_tree ) ) {
 offset += 2 + info_element_len ;
 codeset = locked_codeset ;
 continue ;
 }
 }
 ie_tree = proto_tree_add_subtree ( q931_tree , tvb , offset , 1 + 1 + info_element_len , ett_q931_ie [ info_element ] , & ti , val_to_str ( info_element , q931_info_element_vals [ codeset ] , "Unknown information element (0x%02X)" ) ) ;
 proto_tree_add_uint_format_value ( ie_tree , hf_q931_information_element , tvb , offset , 1 , info_element , "%s" , val_to_str ( info_element , q931_info_element_vals [ codeset ] , "Unknown (0x%02X)" ) ) ;
 proto_tree_add_uint ( ie_tree , hf_q931_information_element_len , tvb , offset + 1 , 1 , info_element_len ) ;
 if ( ( ( codeset << 8 ) | info_element ) == ( CS0 | Q931_IE_SEGMENTED_MESSAGE ) ) {
 dissect_q931_segmented_message_ie ( tvb , pinfo , offset + 2 , info_element_len , ie_tree , ti ) ;
 col_append_fstr ( pinfo -> cinfo , COL_INFO , " of %s" , val_to_str_ext ( tvb_get_guint8 ( tvb , offset + 3 ) , & q931_message_type_vals_ext , "Unknown message type (0x%02X)" ) ) ;
 if ( tvb_get_guint8 ( tvb , offset + 2 ) & 0x80 ) {
 first_segment = TRUE ;
 }
 else {
 proto_tree_add_item ( q931_tree , hf_q931_message_segment , tvb , offset + 4 , - 1 , ENC_NA ) ;
 info_element_len += tvb_reported_length_remaining ( tvb , offset + 4 ) ;
 }
 }
 else {
 switch ( ( codeset << 8 ) | info_element ) {
 case CS0 | Q931_IE_BEARER_CAPABILITY : case CS0 | Q931_IE_LOW_LAYER_COMPAT : if ( q931_tree != NULL ) {
 dissect_q931_bearer_capability_ie ( tvb , offset + 2 , info_element_len , ie_tree ) ;
 }
 break ;
 case CS0 | Q931_IE_CAUSE : dissect_q931_cause_ie_unsafe ( tvb , offset + 2 , info_element_len , ie_tree , hf_q931_cause_value , & dummy , q931_info_element_vals0 ) ;
 break ;
 case CS0 | Q931_IE_CHANGE_STATUS : if ( q931_tree != NULL ) {
 dissect_q931_change_status_ie ( tvb , offset + 2 , info_element_len , ie_tree ) ;
 }
 break ;
 case CS0 | Q931_IE_CALL_STATE : if ( q931_tree != NULL ) {
 dissect_q931_call_state_ie ( tvb , offset + 2 , info_element_len , ie_tree ) ;
 }
 break ;
 case CS0 | Q931_IE_CHANNEL_IDENTIFICATION : if ( q931_tree != NULL ) {
 dissect_q931_channel_identification_ie ( tvb , offset + 2 , info_element_len , ie_tree ) ;
 }
 break ;
 case CS0 | Q931_IE_PROGRESS_INDICATOR : if ( q931_tree != NULL ) {
 dissect_q931_progress_indicator_ie ( tvb , offset + 2 , info_element_len , ie_tree ) ;
 }
 break ;
 case CS0 | Q931_IE_NETWORK_SPECIFIC_FACIL : case CS0 | Q931_IE_TRANSIT_NETWORK_SEL : if ( q931_tree != NULL ) {
 dissect_q931_ns_facilities_ie ( tvb , offset + 2 , info_element_len , ie_tree ) ;
 }
 break ;
 case CS0 | Q931_IE_NOTIFICATION_INDICATOR : if ( q931_tree != NULL ) {
 dissect_q931_notification_indicator_ie ( tvb , offset + 2 , info_element_len , ie_tree ) ;
 }
 break ;
 case CS0 | Q931_IE_DISPLAY : if ( q931_tree != NULL ) {
 dissect_q931_ia5_ie ( tvb , offset + 2 , info_element_len , ie_tree , hf_q931_display_information ) ;
 }
 break ;
 case CS0 | Q931_IE_DATE_TIME : dissect_q931_date_time_ie ( tvb , pinfo , offset + 2 , info_element_len , ie_tree ) ;
 break ;
 case CS0 | Q931_IE_KEYPAD_FACILITY : if ( q931_tree != NULL ) {
 dissect_q931_ia5_ie ( tvb , offset + 2 , info_element_len , ie_tree , hf_q931_keypad_facility ) ;
 }
 break ;
 case CS0 | Q931_IE_SIGNAL : dissect_q931_signal_ie ( tvb , pinfo , offset + 2 , info_element_len , ie_tree , ti ) ;
 break ;
 case CS0 | Q931_IE_INFORMATION_RATE : dissect_q931_information_rate_ie ( tvb , pinfo , offset + 2 , info_element_len , ie_tree , ti ) ;
 break ;
 case CS0 | Q931_IE_E2E_TRANSIT_DELAY : dissect_q931_e2e_transit_delay_ie ( tvb , pinfo , offset + 2 , info_element_len , ie_tree , ti ) ;
 break ;
 case CS0 | Q931_IE_TD_SELECTION_AND_INT : dissect_q931_td_selection_and_int_ie ( tvb , pinfo , offset + 2 , info_element_len , ie_tree , ti ) ;
 break ;
 case CS0 | Q931_IE_PL_BINARY_PARAMETERS : if ( q931_tree != NULL ) {
 dissect_q931_pl_binary_parameters_ie ( tvb , offset + 2 , info_element_len , ie_tree ) ;
 }
 break ;
 case CS0 | Q931_IE_PL_WINDOW_SIZE : if ( q931_tree != NULL ) {
 dissect_q931_pl_window_size_ie ( tvb , offset + 2 , info_element_len , ie_tree ) ;
 }
 break ;
 case CS0 | Q931_IE_PACKET_SIZE : if ( q931_tree != NULL ) {
 dissect_q931_packet_size_ie ( tvb , offset + 2 , info_element_len , ie_tree ) ;
 }
 break ;
 case CS0 | Q931_IE_CUG : if ( q931_tree != NULL ) {
 dissect_q931_cug_ie ( tvb , offset + 2 , info_element_len , ie_tree ) ;
 }
 break ;
 case CS0 | Q931_IE_REVERSE_CHARGE_IND : if ( q931_tree != NULL ) {
 dissect_q931_reverse_charge_ind_ie ( tvb , offset + 2 , info_element_len , ie_tree ) ;
 }
 break ;
 case CS0 | Q931_IE_CONNECTED_NUMBER_DEFAULT : if ( q931_tree != NULL ) {
 dissect_q931_number_ie ( tvb , offset + 2 , info_element_len , ie_tree , hf_q931_connected_number , e164_info ) ;
 }
 break ;
 case CS0 | Q931_IE_CALLING_PARTY_NUMBER : e164_info . e164_number_type = CALLING_PARTY_NUMBER ;
 dissect_q931_number_ie ( tvb , offset + 2 , info_element_len , ie_tree , hf_q931_calling_party_number , e164_info ) ;
 break ;
 case CS0 | Q931_IE_CALLED_PARTY_NUMBER : e164_info . e164_number_type = CALLED_PARTY_NUMBER ;
 dissect_q931_number_ie ( tvb , offset + 2 , info_element_len , ie_tree , hf_q931_called_party_number , e164_info ) ;
 break ;
 case CS0 | Q931_IE_CALLING_PARTY_SUBADDR : case CS0 | Q931_IE_CALLED_PARTY_SUBADDR : if ( q931_tree != NULL ) {
 dissect_q931_party_subaddr_ie ( tvb , offset + 2 , info_element_len , ie_tree ) ;
 }
 break ;
 case CS0 | Q931_IE_REDIRECTING_NUMBER : if ( q931_tree != NULL ) {
 dissect_q931_number_ie ( tvb , offset + 2 , info_element_len , ie_tree , hf_q931_redirecting_number , e164_info ) ;
 }
 break ;
 case CS0 | Q931_IE_RESTART_INDICATOR : dissect_q931_restart_indicator_ie ( tvb , pinfo , offset + 2 , info_element_len , ie_tree , ti ) ;
 break ;
 case CS0 | Q931_IE_HIGH_LAYER_COMPAT : if ( q931_tree != NULL ) {
 dissect_q931_high_layer_compat_ie ( tvb , offset + 2 , info_element_len , ie_tree ) ;
 }
 break ;
 case CS0 | Q931_IE_USER_USER : if ( q931_tree != NULL ) {
 dissect_q931_user_user_ie ( tvb , pinfo , offset + 2 , info_element_len , ie_tree ) ;
 }
 break ;
 case CS5 | Q931_IE_PARTY_CATEGORY : if ( q931_tree != NULL ) {
 dissect_q931_party_category_ie ( tvb , offset + 2 , info_element_len , ie_tree ) ;
 }
 break ;
 case CS6 | Q931_IE_DISPLAY : if ( q931_tree != NULL ) {
 dissect_q931_ia5_ie ( tvb , offset + 2 , info_element_len , ie_tree , hf_q931_avaya_display ) ;
 }
 break ;
 default : if ( q931_tree != NULL ) {
 proto_tree_add_item ( ie_tree , hf_q931_data , tvb , offset + 2 , info_element_len , ENC_NA ) ;
 }
 break ;
 }
 }
 offset += 1 + 1 + info_element_len ;
 }
 codeset = locked_codeset ;
 }
 if ( have_valid_q931_pi ) {
 tap_queue_packet ( q931_tap , pinfo , q931_pi ) ;
 }
 have_valid_q931_pi = FALSE ;
 }