static gint dissect_attribute_value ( proto_tree * tree , proto_item * patron_item , packet_info * pinfo , tvbuff_t * old_tvb , gint old_offset , gint length , guint16 handle , bluetooth_uuid_t uuid , btatt_data_t * att_data ) {
 proto_item * sub_item ;
 proto_tree * sub_tree = NULL ;
 tvbuff_t * tvb ;
 gint offset = 0 ;
 bluetooth_uuid_t sub_uuid ;
 bluetooth_uuid_t service_uuid ;
 guint16 sub_handle ;
 guint32 value ;
 guint32 flags ;
 guint32 operator_value ;
 guint32 opcode ;
 guint32 operand_offset ;
 const gint * * hfs ;
 bluetooth_data_t * bluetooth_data = NULL ;
 tvb = tvb_new_subset_length_caplen ( old_tvb , old_offset , length , length ) ;
 DISSECTOR_ASSERT ( att_data ) ;
 bluetooth_data = att_data -> bluetooth_data ;
 if ( p_get_proto_data ( pinfo -> pool , pinfo , proto_btatt , PROTO_DATA_BTATT_HANDLE ) == NULL ) {
 guint16 * value_data ;
 value_data = wmem_new ( wmem_file_scope ( ) , guint16 ) ;
 * value_data = handle ;
 p_add_proto_data ( pinfo -> pool , pinfo , proto_btatt , PROTO_DATA_BTATT_HANDLE , value_data ) ;
 }
 if ( btatt_dissect_attribute_handle ( handle , tvb , pinfo , tree , att_data ) ) return old_offset + length ;
 if ( p_get_proto_data ( pinfo -> pool , pinfo , proto_bluetooth , PROTO_DATA_BLUETOOTH_SERVICE_UUID ) == NULL ) {
 guint8 * value_data ;
 value_data = wmem_strdup ( wmem_file_scope ( ) , print_numeric_uuid ( & uuid ) ) ;
 p_add_proto_data ( pinfo -> pool , pinfo , proto_bluetooth , PROTO_DATA_BLUETOOTH_SERVICE_UUID , value_data ) ;
 }
 if ( dissector_try_string ( bluetooth_uuid_table , print_numeric_uuid ( & uuid ) , tvb , pinfo , tree , att_data ) ) return old_offset + length ;
 else if ( ! uuid . bt_uuid ) {
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) return old_offset ;
 proto_tree_add_item ( tree , hf_btatt_value , tvb , offset , - 1 , ENC_NA ) ;
 return old_offset + tvb_captured_length ( tvb ) ;
 }
 service_uuid = get_service_uuid_from_handle ( pinfo , handle , bluetooth_data ) ;
 switch ( uuid . bt_uuid ) {
 case 0x2800 : case 0x2801 : if ( is_readable_request ( att_data -> opcode ) || att_data -> opcode == ATT_OPCODE_READ_BY_GROUP_TYPE_REQUEST ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_READ_BY_GROUP_TYPE_RESPONSE ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 if ( tvb_reported_length_remaining ( tvb , offset ) == 2 ) {
 proto_tree_add_item ( tree , hf_btatt_uuid16 , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 sub_uuid = get_uuid ( tvb , offset , 2 ) ;
 proto_item_append_text ( patron_item , ", UUID: %s" , print_uuid ( & sub_uuid ) ) ;
 offset += 2 ;
 col_append_fstr ( pinfo -> cinfo , COL_INFO , ", %s" , print_uuid ( & sub_uuid ) ) ;
 save_handle ( pinfo , sub_uuid , handle , ATTRIBUTE_TYPE_SERVICE , bluetooth_data ) ;
 }
 else if ( tvb_reported_length_remaining ( tvb , offset ) == 16 ) {
 proto_tree_add_item ( tree , hf_btatt_uuid128 , tvb , offset , 16 , ENC_NA ) ;
 sub_uuid = get_uuid ( tvb , offset , 16 ) ;
 proto_item_append_text ( patron_item , ", UUID128: %s" , print_uuid ( & sub_uuid ) ) ;
 offset += 16 ;
 col_append_fstr ( pinfo -> cinfo , COL_INFO , ", %s" , print_uuid ( & sub_uuid ) ) ;
 save_handle ( pinfo , sub_uuid , handle , ATTRIBUTE_TYPE_SERVICE , bluetooth_data ) ;
 }
 else {
 sub_item = proto_tree_add_item ( tree , hf_btatt_value , tvb , offset , - 1 , ENC_NA ) ;
 expert_add_info ( pinfo , sub_item , & ei_btatt_bad_data ) ;
 offset = tvb_captured_length ( tvb ) ;
 }
 break ;
 case 0x2802 : if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 offset = dissect_handle ( tree , pinfo , hf_btatt_included_service_handle , tvb , offset , bluetooth_data , NULL , HANDLE_TVB ) ;
 sub_handle = tvb_get_guint16 ( tvb , offset - 2 , ENC_LITTLE_ENDIAN ) ;
 proto_tree_add_item ( tree , hf_btatt_ending_handle , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( tree , hf_btatt_uuid16 , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 sub_uuid = get_uuid ( tvb , offset , 2 ) ;
 proto_item_append_text ( patron_item , ", Included Handle: 0x%04x, UUID: %s" , sub_handle , print_uuid ( & sub_uuid ) ) ;
 offset += 2 ;
 col_append_fstr ( pinfo -> cinfo , COL_INFO , ", %s" , print_uuid ( & sub_uuid ) ) ;
 save_handle ( pinfo , sub_uuid , sub_handle , ATTRIBUTE_TYPE_OTHER , bluetooth_data ) ;
 break ;
 case 0x2803 : if ( is_readable_request ( att_data -> opcode ) || att_data -> opcode == ATT_OPCODE_READ_BY_TYPE_REQUEST ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_READ_BY_TYPE_RESPONSE ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_characteristic_properties , ett_btatt_characteristic_properties , hfx_btatt_characteristic_properties , ENC_NA ) ;
 offset += 1 ;
 offset = dissect_handle ( tree , pinfo , hf_btatt_characteristic_value_handle , tvb , offset , bluetooth_data , NULL , HANDLE_TVB ) ;
 sub_handle = tvb_get_guint16 ( tvb , offset - 2 , ENC_LITTLE_ENDIAN ) ;
 if ( tvb_reported_length_remaining ( tvb , offset ) == 16 ) {
 proto_tree_add_item ( tree , hf_btatt_uuid128 , tvb , offset , 16 , ENC_NA ) ;
 sub_uuid = get_uuid ( tvb , offset , 16 ) ;
 proto_item_append_text ( patron_item , ", Characteristic Handle: 0x%04x, UUID128: %s" , tvb_get_guint16 ( tvb , offset - 2 , ENC_LITTLE_ENDIAN ) , print_uuid ( & sub_uuid ) ) ;
 offset += 16 ;
 col_append_fstr ( pinfo -> cinfo , COL_INFO , ", %s" , print_uuid ( & sub_uuid ) ) ;
 save_handle ( pinfo , sub_uuid , sub_handle , ATTRIBUTE_TYPE_CHARACTERISTIC , bluetooth_data ) ;
 }
 else if ( tvb_reported_length_remaining ( tvb , offset ) == 2 ) {
 proto_tree_add_item ( tree , hf_btatt_uuid16 , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 sub_uuid = get_uuid ( tvb , offset , 2 ) ;
 proto_item_append_text ( patron_item , ", Characteristic Handle: 0x%04x, UUID: %s" , sub_handle , print_uuid ( & sub_uuid ) ) ;
 offset += 2 ;
 col_append_fstr ( pinfo -> cinfo , COL_INFO , ", %s" , print_uuid ( & sub_uuid ) ) ;
 save_handle ( pinfo , sub_uuid , sub_handle , ATTRIBUTE_TYPE_CHARACTERISTIC , bluetooth_data ) ;
 }
 else {
 sub_item = proto_tree_add_item ( tree , hf_btatt_value , tvb , offset , - 1 , ENC_NA ) ;
 expert_add_info ( pinfo , sub_item , & ei_btatt_bad_data ) ;
 offset = tvb_captured_length ( tvb ) ;
 }
 break ;
 case 0x2900 : if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_characteristic_extended_properties , ett_btatt_value , hfx_btatt_characteristic_extended_properties , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 case 0x2901 : if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_characteristic_user_description , tvb , offset , tvb_captured_length_remaining ( tvb , offset ) , ENC_NA | ENC_UTF_8 ) ;
 offset += tvb_captured_length_remaining ( tvb , offset ) ;
 break ;
 case 0x2902 : if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) || att_data -> opcode == ATT_OPCODE_HANDLE_VALUE_CONFIRMATION ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_characteristic_configuration_client , ett_btatt_value , hfx_btatt_characteristic_configuration_client , ENC_LITTLE_ENDIAN ) ;
 value = tvb_get_guint16 ( tvb , offset , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 {
 bluetooth_uuid_t characteristic_uuid ;
 characteristic_uuid = get_characteristic_uuid_from_handle ( pinfo , handle , bluetooth_data ) ;
 if ( value & 0x1 ) switch ( characteristic_uuid . bt_uuid ) {
 case 0x2A05 : case 0x2A1C : case 0x2A21 : case 0x2A35 : case 0x2A52 : case 0x2A55 : case 0x2A66 : case 0x2A6B : case 0x2A99 : case 0x2A9C : case 0x2A9D : case 0x2A9F : case 0x2ABC : case 0x2AC5 : case 0x2AC6 : case 0x2AC8 : case 0x2AC9 : case 0x2ACC : case 0x2AD4 : case 0x2AD5 : case 0x2AD6 : case 0x2AD7 : case 0x2AD8 : case 0x2AD9 : expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 break ;
 case 0x2A18 : case 0x2A19 : case 0x2A1E : case 0x2A22 : case 0x2A2C : case 0x2A2B : case 0x2A31 : case 0x2A33 : case 0x2A34 : case 0x2A36 : case 0x2A37 : case 0x2A3F : case 0x2A45 : case 0x2A46 : case 0x2A4D : case 0x2A53 : case 0x2A56 : case 0x2A58 : case 0x2A5A : case 0x2A5B : case 0x2A63 : case 0x2A64 : case 0x2A67 : case 0x2A68 : case 0x2A6C : case 0x2A6D : case 0x2A6E : case 0x2A6F : case 0x2A70 : case 0x2A71 : case 0x2A72 : case 0x2A73 : case 0x2A74 : case 0x2A75 : case 0x2A76 : case 0x2A77 : case 0x2A78 : case 0x2A79 : case 0x2A7A : case 0x2A7B : case 0x2AA0 : case 0x2AA1 : case 0x2AA3 : case 0x2AA7 : case 0x2AB8 : case 0x2ACD : case 0x2ACE : case 0x2ACF : case 0x2AD0 : case 0x2AD1 : case 0x2AD2 : case 0x2AD3 : case 0x2ADA : default : break ;
 }
 if ( value & 0x2 ) switch ( characteristic_uuid . bt_uuid ) {
 case 0x2A18 : case 0x2A19 : case 0x2A1E : case 0x2A22 : case 0x2A2B : case 0x2A2C : case 0x2A31 : case 0x2A33 : case 0x2A34 : case 0x2A36 : case 0x2A37 : case 0x2A3F : case 0x2A45 : case 0x2A46 : case 0x2A4D : case 0x2A53 : case 0x2A5B : case 0x2A63 : case 0x2A64 : case 0x2A67 : case 0x2A68 : case 0x2A6C : case 0x2A6D : case 0x2A6E : case 0x2A6F : case 0x2A70 : case 0x2A71 : case 0x2A72 : case 0x2A73 : case 0x2A74 : case 0x2A75 : case 0x2A76 : case 0x2A77 : case 0x2A78 : case 0x2A79 : case 0x2A7A : case 0x2A7B : case 0x2AA0 : case 0x2AA1 : case 0x2AA3 : case 0x2AA7 : case 0x2AB8 : case 0x2AC9 : case 0x2ACC : case 0x2ACD : case 0x2ACE : case 0x2ACF : case 0x2AD0 : case 0x2AD1 : case 0x2AD2 : case 0x2AD3 : case 0x2AD4 : case 0x2AD5 : case 0x2AD6 : case 0x2AD7 : case 0x2AD8 : case 0x2ADA : expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 break ;
 case 0x2A05 : case 0x2A1C : case 0x2A21 : case 0x2A35 : case 0x2A52 : case 0x2A55 : case 0x2A56 : case 0x2A58 : case 0x2A5A : case 0x2A66 : case 0x2A6B : case 0x2A99 : case 0x2A9C : case 0x2A9D : case 0x2A9F : case 0x2ABC : case 0x2AC5 : case 0x2AC6 : case 0x2AC8 : case 0x2AD9 : default : break ;
 }
 if ( value > 0x3 ) expert_add_info ( pinfo , tree , & ei_btatt_bad_data ) ;
 }
 break ;
 case 0x2903 : if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_characteristic_configuration_server , ett_btatt_value , hfx_btatt_characteristic_configuration_server , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 case 0x2904 : if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_characteristic_presentation_format , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_characteristic_presentation_exponent , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_characteristic_presentation_unit , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( tree , hf_btatt_characteristic_presentation_namespace , tvb , offset , 1 , ENC_NA ) ;
 value = tvb_get_guint8 ( tvb , offset ) ;
 offset += 1 ;
 if ( value == 0x01 ) proto_tree_add_item ( tree , hf_btatt_characteristic_presentation_namespace_description_btsig , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 else proto_tree_add_item ( tree , hf_btatt_characteristic_presentation_namespace_description , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 case 0x2905 : if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 sub_item = proto_tree_add_none_format ( tree , hf_btatt_handles_info , tvb , offset , tvb_captured_length ( tvb ) , "Handles (%i items)" , tvb_captured_length ( tvb ) / 2 ) ;
 sub_tree = proto_item_add_subtree ( sub_item , ett_btatt_list ) ;
 while ( offset < ( gint64 ) tvb_captured_length ( tvb ) ) {
 offset = dissect_handle ( sub_tree , pinfo , hf_btatt_handle , tvb , offset , bluetooth_data , NULL , HANDLE_TVB ) ;
 }
 break ;
 case 0x2906 : {
 bluetooth_uuid_t characteristic_uuid ;
 guint8 * characteristic_dissector_name ;
 dissector_handle_t characteristic_dissector ;
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 characteristic_uuid = get_characteristic_uuid_from_handle ( pinfo , handle , bluetooth_data ) ;
 characteristic_dissector_name = wmem_strdup_printf ( wmem_packet_scope ( ) , "btgatt.uuid0x%s" , print_numeric_uuid ( & characteristic_uuid ) ) ;
 characteristic_dissector = find_dissector ( characteristic_dissector_name ) ;
 sub_item = proto_tree_add_item ( tree , hf_btatt_valid_range_lower_inclusive_value , tvb , offset , tvb_reported_length_remaining ( tvb , offset ) / 2 , ENC_NA ) ;
 sub_tree = proto_item_add_subtree ( sub_item , ett_btatt_list ) ;
 if ( characteristic_dissector ) call_dissector_with_data ( characteristic_dissector , tvb_new_subset_length_caplen ( tvb , offset , tvb_reported_length_remaining ( tvb , offset ) / 2 , tvb_reported_length_remaining ( tvb , offset ) / 2 ) , pinfo , sub_tree , att_data ) ;
 sub_item = proto_tree_add_item ( tree , hf_btatt_valid_range_upper_inclusive_value , tvb , offset + tvb_reported_length_remaining ( tvb , offset ) / 2 , tvb_reported_length_remaining ( tvb , offset ) / 2 , ENC_NA ) ;
 sub_tree = proto_item_add_subtree ( sub_item , ett_btatt_list ) ;
 if ( characteristic_dissector ) call_dissector_with_data ( characteristic_dissector , tvb_new_subset_length_caplen ( tvb , offset + tvb_reported_length_remaining ( tvb , offset ) / 2 , tvb_reported_length_remaining ( tvb , offset ) / 2 , tvb_reported_length_remaining ( tvb , offset ) / 2 ) , pinfo , sub_tree , att_data ) ;
 offset += tvb_reported_length_remaining ( tvb , offset ) ;
 }
 break ;
 case 0x2907 : if ( service_uuid . bt_uuid == GATT_SERVICE_HUMAN_INTERFACE_DEVICE ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 if ( tvb_reported_length_remaining ( tvb , offset ) == 2 ) {
 proto_tree_add_item ( tree , hf_btatt_uuid16 , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 else if ( tvb_reported_length_remaining ( tvb , offset ) == 16 ) {
 proto_tree_add_item ( tree , hf_btatt_uuid128 , tvb , offset , 16 , ENC_NA ) ;
 offset += 16 ;
 }
 else {
 sub_item = proto_tree_add_item ( tree , hf_btatt_value , tvb , offset , - 1 , ENC_NA ) ;
 expert_add_info ( pinfo , sub_item , & ei_btatt_bad_data ) ;
 offset = tvb_captured_length ( tvb ) ;
 }
 break ;
 case 0x2908 : if ( service_uuid . bt_uuid == GATT_SERVICE_HUMAN_INTERFACE_DEVICE ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_report_reference_report_id , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_report_reference_report_type , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2909 : if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_number_of_digitals , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x290A : if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_value_trigger_setting_condition , tvb , offset , 1 , ENC_NA ) ;
 value = tvb_get_guint8 ( tvb , offset ) ;
 offset += 1 ;
 if ( value >= 1 && value <= 3 ) {
 proto_tree_add_item ( tree , hf_btatt_value_trigger_setting_analog , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 else if ( value == 4 ) {
 call_dissector_with_data ( find_dissector ( "btgatt.uuid0x2a56" ) , tvb_new_subset_length_caplen ( tvb , offset , 1 , 1 ) , pinfo , tree , att_data ) ;
 offset += 1 ;
 }
 else if ( value == 5 || value == 6 ) {
 proto_tree_add_item ( tree , hf_btatt_value_trigger_setting_analog_one , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( tree , hf_btatt_value_trigger_setting_analog_two , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 break ;
 case 0x290B : if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_esp_trigger_logic , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x290C : if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_esp_flags , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( tree , hf_btatt_esp_sampling_function , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_esp_measurement_period , tvb , offset , 3 , ENC_LITTLE_ENDIAN ) ;
 offset += 3 ;
 proto_tree_add_item ( tree , hf_btatt_esp_update_interval , tvb , offset , 3 , ENC_LITTLE_ENDIAN ) ;
 offset += 3 ;
 proto_tree_add_item ( tree , hf_btatt_esp_application , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_esp_measurement_uncertainty , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x290D : if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_esp_condition , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_esp_operand , tvb , offset , tvb_captured_length_remaining ( tvb , offset ) , ENC_NA ) ;
 offset += tvb_captured_length_remaining ( tvb , offset ) ;
 break ;
 case 0x290E : if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_time_trigger_setting_condition , tvb , offset , 1 , ENC_NA ) ;
 value = tvb_get_guint8 ( tvb , offset ) ;
 offset += 1 ;
 if ( value == 0 ) {
 proto_tree_add_item ( tree , hf_btatt_time_trigger_setting_value , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 }
 else if ( value == 1 || value == 2 ) {
 proto_tree_add_item ( tree , hf_btatt_time_trigger_setting_value_time_interval , tvb , offset , 3 , ENC_LITTLE_ENDIAN ) ;
 offset += 3 ;
 }
 else if ( value == 3 ) {
 proto_tree_add_item ( tree , hf_btatt_time_trigger_setting_value_count , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 break ;
 case 0x2A00 : if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_device_name , tvb , offset , tvb_captured_length_remaining ( tvb , offset ) , ENC_NA | ENC_UTF_8 ) ;
 offset += tvb_captured_length_remaining ( tvb , offset ) ;
 break ;
 case 0x2A01 : if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 switch ( ( tvb_get_guint16 ( tvb , offset , ENC_LITTLE_ENDIAN ) & 0xFFC0 ) >> 6 ) {
 case 0x003 : hfs = hfx_btatt_appearance_watch ;
 break ;
 case 0x00C : hfs = hfx_btatt_appearance_thermometer ;
 break ;
 case 0x00D : hfs = hfx_btatt_appearance_heart_rate ;
 break ;
 case 0x00E : hfs = hfx_btatt_appearance_blood_pressure ;
 break ;
 case 0x00F : hfs = hfx_btatt_appearance_hid ;
 break ;
 case 0x011 : hfs = hfx_btatt_appearance_running_walking_sensor ;
 break ;
 case 0x012 : hfs = hfx_btatt_appearance_cycling ;
 break ;
 case 0x031 : hfs = hfx_btatt_appearance_pulse_oximeter ;
 break ;
 case 0x033 : hfs = hfx_btatt_appearance_personal_mobility_device ;
 break ;
 case 0x035 : hfs = hfx_btatt_appearance_insulin_pump ;
 break ;
 case 0x051 : hfs = hfx_btatt_appearance_outdoor_sports_activity ;
 break ;
 default : hfs = hfx_btatt_appearance ;
 }
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_appearance , ett_btatt_value , hfs , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 case 0x2A02 : if ( service_uuid . bt_uuid == GATT_SERVICE_GENERIC_ACCESS_PROFILE ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_peripheral_privacy_flag , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A03 : if ( service_uuid . bt_uuid == GATT_SERVICE_GENERIC_ACCESS_PROFILE ) {
 if ( is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 offset = dissect_bd_addr ( hf_btatt_reconnection_address , pinfo , tree , tvb , offset , FALSE , bluetooth_data -> interface_id , bluetooth_data -> adapter_id , NULL ) ;
 break ;
 case 0x2A04 : if ( service_uuid . bt_uuid == GATT_SERVICE_GENERIC_ACCESS_PROFILE ) {
 if ( ! ( is_readable_request ( att_data -> opcode ) || is_readable_response ( att_data -> opcode ) ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_minimum_connection_interval , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( tree , hf_btatt_maximum_connection_interval , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( tree , hf_btatt_slave_latency , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( tree , hf_btatt_connection_supervision_timeout_multiplier , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 case 0x2A05 : if ( service_uuid . bt_uuid == GATT_SERVICE_GENERIC_ATTRIBUTE_PROFILE ) {
 if ( att_data -> opcode == ATT_OPCODE_HANDLE_VALUE_CONFIRMATION ) break ;
 if ( att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_INDICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_starting_handle , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( tree , hf_btatt_ending_handle , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 case 0x2A06 : if ( service_uuid . bt_uuid == GATT_SERVICE_IMMEDIATE_ALERT ) {
 if ( att_data -> opcode != ATT_OPCODE_WRITE_COMMAND ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 else if ( service_uuid . bt_uuid == GATT_SERVICE_LINK_LOSS ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_alert_level , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A07 : if ( service_uuid . bt_uuid == GATT_SERVICE_TX_POWER ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_tx_power_level , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A08 : if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_year , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( tree , hf_btatt_month , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_day , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_hours , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_minutes , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_seconds , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A09 : if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_day_of_week , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A0A : case 0x2A0B : case 0x2A0C : case 0x2A2B : if ( uuid . bt_uuid == 0x2A2B ) {
 if ( service_uuid . bt_uuid == GATT_SERVICE_CURRENT_TIME_SERVICE ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_year , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( tree , hf_btatt_month , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_day , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_hours , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_minutes , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_seconds , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_day_of_week , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 if ( uuid . bt_uuid == 0x2A0C || uuid . bt_uuid == 0x2A2B ) {
 proto_tree_add_item ( tree , hf_btatt_fractions256 , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 }
 else if ( uuid . bt_uuid == 0x2A0B ) {
 proto_tree_add_item ( tree , hf_btatt_fractions100 , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 }
 if ( uuid . bt_uuid == 0x2A2B ) {
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_time_adjust_reason , ett_btatt_value , hfx_btatt_time_adjust_reason , ENC_NA ) ;
 offset += 1 ;
 }
 break ;
 case 0x2A0D : if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_dst_offset , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A0E : if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_timezone , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A0F : case 0x2A10 : if ( service_uuid . bt_uuid == GATT_SERVICE_CURRENT_TIME_SERVICE ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 if ( uuid . bt_uuid == 0x2A10 ) {
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_timezone_information , ett_btatt_value , hfx_btatt_timezone_information , ENC_NA ) ;
 offset += 1 ;
 }
 proto_tree_add_item ( tree , hf_btatt_timezone , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_dst_offset , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A11 : if ( service_uuid . bt_uuid == GATT_SERVICE_NEXT_DST_CHANGE_SERVICE ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_year , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( tree , hf_btatt_month , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_day , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_hours , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_minutes , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_seconds , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_dst_offset , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A12 : if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_time_accuracy , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A13 : if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_time_source , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A14 : if ( service_uuid . bt_uuid == GATT_SERVICE_CURRENT_TIME_SERVICE ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_time_source , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_time_accuracy , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_time_days_since_update , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_time_hours_since_update , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A15 : if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 call_dissector_with_data ( find_dissector ( "btgatt.uuid0x2A0C" ) , tvb_new_subset_length_caplen ( tvb , offset , 9 , 9 ) , pinfo , tree , att_data ) ;
 offset += 9 ;
 call_dissector_with_data ( find_dissector ( "btgatt.uuid0x2A0F" ) , tvb_new_subset_length_caplen ( tvb , offset , 2 , 2 ) , pinfo , tree , att_data ) ;
 offset += 2 ;
 call_dissector_with_data ( find_dissector ( "btgatt.uuid0x2A14" ) , tvb_new_subset_length_caplen ( tvb , offset , 4 , 4 ) , pinfo , tree , att_data ) ;
 offset += 4 ;
 break ;
 case 0x2A16 : if ( service_uuid . bt_uuid == GATT_SERVICE_REFERENCE_TIME_UPDATE_SERVICE ) {
 if ( att_data -> opcode != ATT_OPCODE_WRITE_COMMAND ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_time_update_control_point , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A17 : if ( service_uuid . bt_uuid == GATT_SERVICE_REFERENCE_TIME_UPDATE_SERVICE ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_time_current_state , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_time_result , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A18 : if ( service_uuid . bt_uuid == GATT_SERVICE_GLUCOSE ) {
 if ( att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_glucose_measurement_flags , ett_btatt_value , hfx_btatt_glucose_measurement_flags , ENC_NA ) ;
 flags = tvb_get_guint8 ( tvb , offset ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_glucose_measurement_sequence_number , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 sub_item = proto_tree_add_item ( tree , hf_btatt_glucose_measurement_base_time , tvb , offset , 7 , ENC_NA ) ;
 sub_tree = proto_item_add_subtree ( sub_item , ett_btatt_list ) ;
 proto_tree_add_item ( sub_tree , hf_btatt_year , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( sub_tree , hf_btatt_month , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( sub_tree , hf_btatt_day , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( sub_tree , hf_btatt_hours , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( sub_tree , hf_btatt_minutes , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( sub_tree , hf_btatt_seconds , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 if ( flags & 0x01 ) {
 proto_tree_add_item ( tree , hf_btatt_glucose_measurement_time_offset , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 if ( ( flags & 0x02 ) && ! ( flags & 0x04 ) ) {
 proto_tree_add_item ( tree , hf_btatt_glucose_measurement_glucose_concentration_kg_per_l , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 if ( ( flags & 0x02 ) && ( flags & 0x04 ) ) {
 proto_tree_add_item ( tree , hf_btatt_glucose_measurement_glucose_concentration_mol_per_l , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 if ( flags & 0x02 ) {
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_glucose_measurement_type_and_sample_location , ett_btatt_value , hfx_btatt_glucose_measurement_type_and_sample_location , ENC_NA ) ;
 offset += 1 ;
 }
 if ( flags & 0x08 ) {
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_glucose_measurement_sensor_status_annunciation , ett_btatt_value , hfx_btatt_glucose_measurement_sensor_status_annunciation , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 break ;
 case 0x2A19 : {
 if ( service_uuid . bt_uuid == GATT_SERVICE_BATTERY_SERVICE ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 guint32 battery_level ;
 sub_item = proto_tree_add_item_ret_uint ( tree , hf_btatt_battery_level , tvb , offset , 1 , ENC_NA , & battery_level ) ;
 if ( battery_level > 100 ) expert_add_info ( pinfo , sub_item , & ei_btatt_bad_data ) ;
 offset += 1 ;
 }
 break ;
 case 0x2A1A : if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_battery_power_state , ett_btatt_value , hfx_btatt_battery_power_state , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A1B : {
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 guint32 battery_level ;
 sub_item = proto_tree_add_item_ret_uint ( tree , hf_btatt_battery_level , tvb , offset , 1 , ENC_NA , & battery_level ) ;
 if ( battery_level > 100 ) expert_add_info ( pinfo , sub_item , & ei_btatt_bad_data ) ;
 offset += 1 ;
 if ( tvb_reported_length_remaining ( tvb , offset ) >= 1 ) {
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_battery_power_state , ett_btatt_value , hfx_btatt_battery_power_state , ENC_NA ) ;
 offset += 1 ;
 }
 }
 break ;
 case 0x2A1C : case 0x2A1E : if ( uuid . bt_uuid == 0x2A1C ) {
 if ( service_uuid . bt_uuid == GATT_SERVICE_HEALTH_THERMOMETER ) {
 if ( att_data -> opcode == ATT_OPCODE_HANDLE_VALUE_CONFIRMATION ) break ;
 if ( att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_INDICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 }
 else if ( uuid . bt_uuid == 0x2A1E ) {
 if ( service_uuid . bt_uuid == GATT_SERVICE_HEALTH_THERMOMETER ) {
 if ( att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_temperature_measurement_flags , ett_btatt_value , hfx_btatt_temperature_measurement_flags , ENC_NA ) ;
 flags = tvb_get_guint8 ( tvb , offset ) ;
 offset += 1 ;
 if ( flags & 0x01 ) {
 proto_tree_add_item ( tree , hf_btatt_temperature_measurement_value_celsius , tvb , offset , 4 , ENC_LITTLE_ENDIAN ) ;
 offset += 4 ;
 }
 else {
 proto_tree_add_item ( tree , hf_btatt_temperature_measurement_value_fahrenheit , tvb , offset , 4 , ENC_LITTLE_ENDIAN ) ;
 offset += 4 ;
 }
 if ( flags & 0x02 ) {
 sub_item = proto_tree_add_item ( tree , hf_btatt_temperature_measurement_timestamp , tvb , offset , 7 , ENC_NA ) ;
 sub_tree = proto_item_add_subtree ( sub_item , ett_btatt_list ) ;
 proto_tree_add_item ( sub_tree , hf_btatt_year , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( sub_tree , hf_btatt_month , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( sub_tree , hf_btatt_day , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( sub_tree , hf_btatt_hours , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( sub_tree , hf_btatt_minutes , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( sub_tree , hf_btatt_seconds , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 }
 if ( flags & 0x04 ) {
 proto_tree_add_item ( tree , hf_btatt_temperature_type , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 }
 break ;
 case 0x2A1D : if ( service_uuid . bt_uuid == GATT_SERVICE_HEALTH_THERMOMETER ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_temperature_type , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A21 : if ( service_uuid . bt_uuid == GATT_SERVICE_HEALTH_THERMOMETER ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) || att_data -> opcode == ATT_OPCODE_HANDLE_VALUE_CONFIRMATION ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_INDICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_measurement_interval , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 case 0x2A22 : if ( service_uuid . bt_uuid == GATT_SERVICE_HUMAN_INTERFACE_DEVICE ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 call_dissector_with_data ( usb_hid_boot_keyboard_input_report_handle , tvb_new_subset_remaining ( tvb , offset ) , pinfo , tree , NULL ) ;
 offset += tvb_reported_length_remaining ( tvb , offset ) ;
 break ;
 case 0x2A23 : if ( service_uuid . bt_uuid == GATT_SERVICE_DEVICE_INFORMATION ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_system_id_manufacturer_identifier , tvb , offset , 5 , ENC_LITTLE_ENDIAN ) ;
 offset += 5 ;
 proto_tree_add_item ( tree , hf_btatt_system_id_organizationally_unique_identifier , tvb , offset , 3 , ENC_LITTLE_ENDIAN ) ;
 offset += 3 ;
 break ;
 case 0x2A24 : if ( service_uuid . bt_uuid == GATT_SERVICE_DEVICE_INFORMATION ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_model_number_string , tvb , offset , tvb_captured_length_remaining ( tvb , offset ) , ENC_NA | ENC_UTF_8 ) ;
 offset += tvb_captured_length_remaining ( tvb , offset ) ;
 break ;
 case 0x2A25 : if ( service_uuid . bt_uuid == GATT_SERVICE_DEVICE_INFORMATION ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_serial_number_string , tvb , offset , tvb_captured_length_remaining ( tvb , offset ) , ENC_NA | ENC_UTF_8 ) ;
 offset += tvb_captured_length_remaining ( tvb , offset ) ;
 break ;
 case 0x2A26 : if ( service_uuid . bt_uuid == GATT_SERVICE_DEVICE_INFORMATION ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_firmware_revision_string , tvb , offset , tvb_captured_length_remaining ( tvb , offset ) , ENC_NA | ENC_UTF_8 ) ;
 offset += tvb_captured_length_remaining ( tvb , offset ) ;
 break ;
 case 0x2A27 : if ( service_uuid . bt_uuid == GATT_SERVICE_DEVICE_INFORMATION ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_hardware_revision_string , tvb , offset , tvb_captured_length_remaining ( tvb , offset ) , ENC_NA | ENC_UTF_8 ) ;
 offset += tvb_captured_length_remaining ( tvb , offset ) ;
 break ;
 case 0x2A28 : if ( service_uuid . bt_uuid == GATT_SERVICE_DEVICE_INFORMATION ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_software_revision_string , tvb , offset , tvb_captured_length_remaining ( tvb , offset ) , ENC_NA | ENC_UTF_8 ) ;
 offset += tvb_captured_length_remaining ( tvb , offset ) ;
 break ;
 case 0x2A29 : if ( service_uuid . bt_uuid == GATT_SERVICE_DEVICE_INFORMATION ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_manufacturer_string , tvb , offset , tvb_captured_length_remaining ( tvb , offset ) , ENC_NA | ENC_UTF_8 ) ;
 offset += tvb_captured_length_remaining ( tvb , offset ) ;
 break ;
 case 0x2A2A : {
 guint16 count ;
 guint16 list_length = 0 ;
 if ( service_uuid . bt_uuid == GATT_SERVICE_DEVICE_INFORMATION ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_regulatory_certification_data_list_count , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 count = tvb_get_guint16 ( tvb , offset , ENC_LITTLE_ENDIAN ) ;
 list_length += 2 ;
 offset += 2 ;
 proto_tree_add_item ( tree , hf_btatt_regulatory_certification_data_list_length , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 list_length += 2 ;
 offset += 2 ;
 while ( count -- ) {
 proto_item * authorizing_body_data_item ;
 proto_tree * authorizing_body_data_tree ;
 guint8 item_type ;
 guint16 item_length ;
 guint16 certification_data_list_count = 0 ;
 guint16 certification_data_list_length = 0 ;
 proto_item * list_length_item ;
 sub_item = proto_tree_add_item ( tree , hf_btatt_regulatory_certification_data_list_item , tvb , offset , 0 , ENC_NA ) ;
 sub_tree = proto_item_add_subtree ( sub_item , ett_btatt_list ) ;
 proto_tree_add_item ( sub_tree , hf_btatt_regulatory_certification_data_list_item_body , tvb , offset , 1 , ENC_NA ) ;
 list_length += 1 ;
 offset += 1 ;
 proto_tree_add_item ( sub_tree , hf_btatt_regulatory_certification_data_list_item_body_structure_type , tvb , offset , 1 , ENC_NA ) ;
 item_type = tvb_get_guint8 ( tvb , offset ) ;
 list_length += 1 ;
 offset += 1 ;
 list_length_item = proto_tree_add_item ( sub_tree , hf_btatt_regulatory_certification_data_list_item_body_structure_length , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 item_length = tvb_get_guint16 ( tvb , offset , ENC_LITTLE_ENDIAN ) ;
 list_length += 2 + item_length ;
 offset += 2 ;
 if ( item_type == 0x01 ) {
 authorizing_body_data_item = proto_tree_add_item ( sub_tree , hf_btatt_regulatory_certification_data_list_item_authorizing_body_data , tvb , offset , item_length , ENC_NA ) ;
 authorizing_body_data_tree = proto_item_add_subtree ( authorizing_body_data_item , ett_btatt_list ) ;
 if ( item_length > 0 ) {
 proto_tree_add_item ( authorizing_body_data_tree , hf_btatt_regulatory_certification_data_list_item_authorizing_body_data_major_ig_version , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 }
 if ( item_length > 1 ) {
 proto_tree_add_item ( authorizing_body_data_tree , hf_btatt_regulatory_certification_data_list_item_authorizing_body_data_minor_ig_version , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 }
 if ( item_length > 2 ) {
 proto_tree_add_item ( authorizing_body_data_tree , hf_btatt_regulatory_certification_data_list_item_authorizing_body_data_certification_data_list_count , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 certification_data_list_count = tvb_get_guint16 ( tvb , offset , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 if ( item_length > 4 ) {
 proto_tree_add_item ( authorizing_body_data_tree , hf_btatt_regulatory_certification_data_list_item_authorizing_body_data_certification_data_list_length , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 certification_data_list_length = tvb_get_guint16 ( tvb , offset , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 if ( item_length > 6 && certification_data_list_count ) {
 proto_item * certification_data_list_item ;
 proto_tree * certification_data_list_tree ;
 certification_data_list_item = proto_tree_add_item ( sub_tree , hf_btatt_regulatory_certification_data_list_item_authorizing_body_data_certification_data_list , tvb , offset , certification_data_list_length , ENC_NA ) ;
 certification_data_list_tree = proto_item_add_subtree ( certification_data_list_item , ett_btatt_list ) ;
 while ( certification_data_list_count -- ) {
 proto_tree_add_item ( certification_data_list_tree , hf_btatt_regulatory_certification_data_list_item_authorizing_body_data_certified_device_class , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 }
 }
 else if ( item_type == 0x02 ) {
 proto_tree_add_item ( sub_tree , hf_btatt_regulatory_certification_data_list_item_regulation_bit_field_type , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 else {
 proto_tree_add_item ( sub_tree , hf_btatt_regulatory_certification_data_list_item_data , tvb , offset , item_length , ENC_NA ) ;
 offset += item_length ;
 }
 proto_item_set_len ( sub_item , 1 + 1 + 2 + item_length ) ;
 if ( list_length != length ) expert_add_info ( pinfo , list_length_item , & ei_btatt_invalid_length ) ;
 }
 }
 break ;
 case 0x2A2C : if ( service_uuid . bt_uuid == GATT_SERVICE_ENVIRONMENTAL_SENSING ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_magnetic_declination , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 case 0x2A31 : if ( service_uuid . bt_uuid == GATT_SERVICE_SCAN_PARAMETERS ) {
 if ( att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_scan_refresh , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A32 : if ( service_uuid . bt_uuid == GATT_SERVICE_HUMAN_INTERFACE_DEVICE ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_WRITE_COMMAND ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 call_dissector_with_data ( usb_hid_boot_keyboard_output_report_handle , tvb_new_subset_remaining ( tvb , offset ) , pinfo , tree , NULL ) ;
 offset += tvb_reported_length_remaining ( tvb , offset ) ;
 break ;
 case 0x2A33 : if ( service_uuid . bt_uuid == GATT_SERVICE_HUMAN_INTERFACE_DEVICE ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 call_dissector_with_data ( usb_hid_boot_mouse_input_report_handle , tvb_new_subset_remaining ( tvb , offset ) , pinfo , tree , NULL ) ;
 offset += tvb_reported_length_remaining ( tvb , offset ) ;
 break ;
 case 0x2A34 : if ( service_uuid . bt_uuid == GATT_SERVICE_GLUCOSE ) {
 if ( att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_glucose_measurement_context_flags , ett_btatt_value , hfx_btatt_glucose_measurement_context_flags , ENC_NA ) ;
 flags = tvb_get_guint8 ( tvb , offset ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_glucose_measurement_context_sequence_number , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 if ( flags & 0x80 ) {
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_glucose_measurement_context_extended_flags , ett_btatt_value , hfx_btatt_glucose_measurement_context_extended_flags , ENC_NA ) ;
 offset += 1 ;
 }
 if ( flags & 0x01 ) {
 proto_tree_add_item ( tree , hf_btatt_glucose_measurement_context_carbohydrate_id , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_glucose_measurement_context_carbohydrate_kg , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 if ( flags & 0x02 ) {
 proto_tree_add_item ( tree , hf_btatt_glucose_measurement_context_meal , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 }
 if ( flags & 0x04 ) {
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_glucose_measurement_context_tester_health , ett_btatt_value , hfx_btatt_glucose_measurement_context_tester_health , ENC_NA ) ;
 offset += 1 ;
 }
 if ( flags & 0x08 ) {
 proto_tree_add_item ( tree , hf_btatt_glucose_measurement_context_exercise_duration , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( tree , hf_btatt_glucose_measurement_context_exercise_intensity , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 }
 if ( flags & 0x10 ) {
 proto_tree_add_item ( tree , hf_btatt_glucose_measurement_context_medication_id , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 if ( flags & 0x20 ) {
 proto_tree_add_item ( tree , hf_btatt_glucose_measurement_context_medication_l , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 else {
 proto_tree_add_item ( tree , hf_btatt_glucose_measurement_context_medication_kg , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 }
 if ( flags & 0x40 ) {
 proto_tree_add_item ( tree , hf_btatt_glucose_measurement_context_hba1c , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 break ;
 case 0x2A35 : case 0x2A36 : if ( uuid . bt_uuid == 0x2A35 ) {
 if ( service_uuid . bt_uuid == GATT_SERVICE_BLOOD_PRESSURE ) {
 if ( att_data -> opcode == ATT_OPCODE_HANDLE_VALUE_CONFIRMATION ) break ;
 if ( att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_INDICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 }
 else if ( uuid . bt_uuid == 0x2A36 ) {
 if ( service_uuid . bt_uuid == GATT_SERVICE_BLOOD_PRESSURE ) {
 if ( att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_blood_pressure_measurement_flags , ett_btatt_value , hfx_btatt_blood_pressure_measurement_flags , ENC_NA ) ;
 flags = tvb_get_guint8 ( tvb , offset ) ;
 offset += 1 ;
 if ( flags & 0x01 ) {
 proto_tree_add_item ( tree , hf_btatt_blood_pressure_measurement_compound_value_systolic_kpa , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( tree , hf_btatt_blood_pressure_measurement_compound_value_diastolic_kpa , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( tree , hf_btatt_blood_pressure_measurement_compound_value_mean_arterial_pressure_kpa , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 else {
 proto_tree_add_item ( tree , hf_btatt_blood_pressure_measurement_compound_value_systolic_mmhg , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( tree , hf_btatt_blood_pressure_measurement_compound_value_diastolic_mmhg , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( tree , hf_btatt_blood_pressure_measurement_compound_value_mean_arterial_pressure_mmhg , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 if ( flags & 0x02 ) {
 sub_item = proto_tree_add_item ( tree , hf_btatt_blood_pressure_measurement_timestamp , tvb , offset , 7 , ENC_NA ) ;
 sub_tree = proto_item_add_subtree ( sub_item , ett_btatt_list ) ;
 proto_tree_add_item ( sub_tree , hf_btatt_year , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( sub_tree , hf_btatt_month , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( sub_tree , hf_btatt_day , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( sub_tree , hf_btatt_hours , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( sub_tree , hf_btatt_minutes , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( sub_tree , hf_btatt_seconds , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 }
 if ( flags & 0x04 ) {
 proto_tree_add_item ( tree , hf_btatt_blood_pressure_measurement_pulse_rate , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 if ( flags & 0x08 ) {
 proto_tree_add_item ( tree , hf_btatt_blood_pressure_measurement_user_id , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 }
 if ( flags & 0x10 ) {
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_blood_pressure_measurement_status , ett_btatt_value , hfx_btatt_blood_pressure_measurement_status , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 break ;
 case 0x2A37 : if ( service_uuid . bt_uuid == GATT_SERVICE_HEART_RATE ) {
 if ( att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_heart_rate_measurement_flags , ett_btatt_value , hfx_btatt_heart_rate_measurement_flags , ENC_NA ) ;
 flags = tvb_get_guint8 ( tvb , offset ) ;
 offset += 1 ;
 if ( flags & 0x01 ) {
 proto_tree_add_item ( tree , hf_btatt_heart_rate_measurement_value_16 , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 else {
 proto_tree_add_item ( tree , hf_btatt_heart_rate_measurement_value_8 , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 }
 if ( flags & 0x08 ) {
 proto_tree_add_item ( tree , hf_btatt_heart_rate_measurement_energy_expended , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 if ( flags & 0x10 ) {
 guint interval_count = 0 ;
 sub_item = proto_tree_add_item ( tree , hf_btatt_heart_rate_measurement_rr_intervals , tvb , offset , tvb_captured_length_remaining ( tvb , offset ) , ENC_NA ) ;
 sub_tree = proto_item_add_subtree ( sub_item , ett_btatt_list ) ;
 while ( tvb_reported_length_remaining ( tvb , offset ) ) {
 proto_tree_add_item ( sub_tree , hf_btatt_heart_rate_measurement_rr_interval , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 interval_count += 1 ;
 }
 proto_item_append_text ( sub_item , " [count = %2u]" , interval_count ) ;
 }
 break ;
 case 0x2A38 : if ( service_uuid . bt_uuid == GATT_SERVICE_HEART_RATE ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_body_sensor_location , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A39 : if ( service_uuid . bt_uuid == GATT_SERVICE_HEART_RATE ) {
 if ( is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_heart_rate_control_point , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A3F : if ( service_uuid . bt_uuid == GATT_SERVICE_PHONE_ALERT_STATUS_SERVICE ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_alert_status , ett_btatt_value , hfx_btatt_alert_status , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A40 : if ( service_uuid . bt_uuid == GATT_SERVICE_PHONE_ALERT_STATUS_SERVICE ) {
 if ( att_data -> opcode != ATT_OPCODE_WRITE_COMMAND ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_ringer_control_point , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A41 : if ( service_uuid . bt_uuid == GATT_SERVICE_PHONE_ALERT_STATUS_SERVICE ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_ringer_setting , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A42 : case 0x2A47 : case 0x2A48 : if ( uuid . bt_uuid == 0x2A47 || uuid . bt_uuid == 0x2A48 ) {
 if ( service_uuid . bt_uuid == GATT_SERVICE_ALERT_NOTIFICATION_SERVICE ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_alert_category_id_bitmask_1 , ett_btatt_value , hfx_btatt_alert_category_id_bitmask_1 , ENC_NA ) ;
 offset += 1 ;
 if ( tvb_reported_length_remaining ( tvb , offset ) >= 1 ) {
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_alert_category_id_bitmask_2 , ett_btatt_value , hfx_btatt_alert_category_id_bitmask_2 , ENC_NA ) ;
 offset += 1 ;
 }
 break ;
 case 0x2A43 : if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_alert_category_id , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A44 : if ( service_uuid . bt_uuid == GATT_SERVICE_ALERT_NOTIFICATION_SERVICE ) {
 if ( is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_alert_command_id , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_alert_category_id , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A45 : if ( service_uuid . bt_uuid == GATT_SERVICE_ALERT_NOTIFICATION_SERVICE ) {
 if ( att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_alert_category_id , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_alert_unread_count , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A46 : if ( service_uuid . bt_uuid == GATT_SERVICE_ALERT_NOTIFICATION_SERVICE ) {
 if ( att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_alert_category_id , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_alert_number_of_new_alert , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 if ( tvb_reported_length_remaining ( tvb , offset ) > 0 ) {
 proto_tree_add_item ( tree , hf_btatt_alert_text_string_information , tvb , offset , tvb_captured_length_remaining ( tvb , offset ) , ENC_NA | ENC_UTF_8 ) ;
 offset += tvb_captured_length_remaining ( tvb , offset ) ;
 }
 break ;
 case 0x2A49 : if ( service_uuid . bt_uuid == GATT_SERVICE_BLOOD_PRESSURE ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_blood_pressure_feature , ett_btatt_value , hfx_btatt_blood_pressure_feature , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 case 0x2A4A : if ( service_uuid . bt_uuid == GATT_SERVICE_HUMAN_INTERFACE_DEVICE ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_hogp_bcd_hid , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( tree , hf_btatt_hogp_b_country_code , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_hogp_flags , ett_btatt_value , hfx_btatt_hogp_flags , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A4B : if ( service_uuid . bt_uuid == GATT_SERVICE_HUMAN_INTERFACE_DEVICE ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 offset = dissect_usb_hid_get_report_descriptor ( pinfo , tree , tvb , offset , NULL ) ;
 break ;
 case 0x2A4C : if ( service_uuid . bt_uuid == GATT_SERVICE_HUMAN_INTERFACE_DEVICE ) {
 if ( att_data -> opcode != ATT_OPCODE_WRITE_COMMAND ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_hogp_hid_control_point_command , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A4D : if ( service_uuid . bt_uuid == GATT_SERVICE_HUMAN_INTERFACE_DEVICE ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_WRITE_COMMAND && att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 sub_item = proto_tree_add_item ( tree , hf_btatt_value , tvb , offset , - 1 , ENC_NA ) ;
 expert_add_info ( pinfo , sub_item , & ei_btatt_undecoded ) ;
 offset = tvb_captured_length ( tvb ) ;
 break ;
 case 0x2A4E : if ( service_uuid . bt_uuid == GATT_SERVICE_HUMAN_INTERFACE_DEVICE ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_WRITE_COMMAND ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_hogp_protocol_mode , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A4F : if ( service_uuid . bt_uuid == GATT_SERVICE_SCAN_PARAMETERS ) {
 if ( att_data -> opcode != ATT_OPCODE_WRITE_COMMAND ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_le_scan_interval , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( tree , hf_btatt_le_scan_window , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 case 0x2A50 : if ( service_uuid . bt_uuid == GATT_SERVICE_DEVICE_INFORMATION ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_pnp_id_vendor_id_source , tvb , offset , 1 , ENC_NA ) ;
 value = tvb_get_guint8 ( tvb , offset ) ;
 offset += 1 ;
 if ( value == 1 ) proto_tree_add_item ( tree , hf_btatt_pnp_id_vendor_id_bluetooth_sig , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 else if ( value == 2 ) proto_tree_add_item ( tree , hf_btatt_pnp_id_vendor_id_usb_forum , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 else proto_tree_add_item ( tree , hf_btatt_pnp_id_vendor_id , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( tree , hf_btatt_pnp_id_product_id , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( tree , hf_btatt_pnp_id_product_version , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 case 0x2A51 : if ( service_uuid . bt_uuid == GATT_SERVICE_GLUCOSE ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_glucose_feature , ett_btatt_value , hfx_btatt_glucose_feature , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 case 0x2A52 : if ( service_uuid . bt_uuid == GATT_SERVICE_GLUCOSE || service_uuid . bt_uuid == GATT_SERVICE_CONTINUOUS_GLUCOSE_MONITORING || service_uuid . bt_uuid == GATT_SERVICE_PULSE_OXIMETER ) {
 if ( is_writeable_response ( att_data -> opcode ) || att_data -> opcode == ATT_OPCODE_HANDLE_VALUE_CONFIRMATION ) break ;
 if ( ! is_writeable_request ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_INDICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_record_access_control_point_opcode , tvb , offset , 1 , ENC_NA ) ;
 opcode = tvb_get_guint8 ( tvb , offset ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_record_access_control_point_operator , tvb , offset , 1 , ENC_NA ) ;
 operator_value = tvb_get_guint8 ( tvb , offset ) ;
 offset += 1 ;
 sub_item = proto_tree_add_item ( tree , hf_btatt_record_access_control_point_operand , tvb , offset , 0 , ENC_NA ) ;
 sub_tree = proto_item_add_subtree ( sub_item , ett_btatt_list ) ;
 operand_offset = offset ;
 switch ( opcode ) {
 case 1 : case 2 : case 4 : switch ( operator_value ) {
 case 0 : case 1 : case 5 : case 6 : break ;
 case 2 : proto_tree_add_item ( sub_tree , hf_btatt_record_access_control_point_operand_filter_type , tvb , offset , 1 , ENC_NA ) ;
 value = tvb_get_guint8 ( tvb , offset ) ;
 offset += 1 ;
 if ( value == 0x01 ) {
 proto_tree_add_item ( sub_tree , hf_btatt_record_access_control_point_operand_max_time_offset , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 else {
 proto_tree_add_item ( sub_tree , hf_btatt_value , tvb , offset , - 1 , ENC_NA ) ;
 offset = tvb_captured_length ( tvb ) ;
 }
 break ;
 case 3 : proto_tree_add_item ( sub_tree , hf_btatt_record_access_control_point_operand_filter_type , tvb , offset , 1 , ENC_NA ) ;
 value = tvb_get_guint8 ( tvb , offset ) ;
 offset += 1 ;
 if ( value == 0x01 ) {
 proto_tree_add_item ( sub_tree , hf_btatt_record_access_control_point_operand_min_time_offset , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 else {
 proto_tree_add_item ( sub_tree , hf_btatt_value , tvb , offset , - 1 , ENC_NA ) ;
 offset = tvb_captured_length ( tvb ) ;
 }
 break ;
 case 4 : proto_tree_add_item ( sub_tree , hf_btatt_record_access_control_point_operand_filter_type , tvb , offset , 1 , ENC_NA ) ;
 value = tvb_get_guint8 ( tvb , offset ) ;
 offset += 1 ;
 if ( value == 0x01 ) {
 proto_tree_add_item ( sub_tree , hf_btatt_record_access_control_point_operand_min_time_offset , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( sub_tree , hf_btatt_record_access_control_point_operand_max_time_offset , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 else {
 proto_tree_add_item ( sub_tree , hf_btatt_value , tvb , offset , - 1 , ENC_NA ) ;
 offset = tvb_captured_length ( tvb ) ;
 }
 break ;
 }
 break ;
 case 3 : break ;
 case 5 : proto_tree_add_item ( sub_tree , hf_btatt_record_access_control_point_operand_number_of_records , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 case 6 : proto_tree_add_item ( sub_tree , hf_btatt_record_access_control_point_request_opcode , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( sub_tree , hf_btatt_record_access_control_point_response_code , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 }
 ;
 proto_item_set_len ( sub_item , offset - operand_offset ) ;
 break ;
 case 0x2A53 : if ( service_uuid . bt_uuid == GATT_SERVICE_RUNNING_SPEED_AND_CADENCE ) {
 if ( att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_rsc_measurement_flags , ett_btatt_value , hfx_btatt_rsc_measurement_flags , ENC_NA ) ;
 flags = tvb_get_guint8 ( tvb , offset ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_rsc_measurement_instantaneous_speed , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( tree , hf_btatt_rsc_measurement_instantaneous_cadence , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 if ( flags & 0x01 ) {
 proto_tree_add_item ( tree , hf_btatt_rsc_measurement_instantaneous_stride_length , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 if ( flags & 0x02 ) {
 proto_tree_add_item ( tree , hf_btatt_rsc_measurement_total_distance , tvb , offset , 4 , ENC_LITTLE_ENDIAN ) ;
 offset += 4 ;
 }
 break ;
 case 0x2A54 : if ( service_uuid . bt_uuid == GATT_SERVICE_RUNNING_SPEED_AND_CADENCE ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_rsc_feature , ett_btatt_value , hfx_btatt_rsc_feature , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 case 0x2A55 : if ( service_uuid . bt_uuid == GATT_SERVICE_RUNNING_SPEED_AND_CADENCE || service_uuid . bt_uuid == GATT_SERVICE_CYCLING_SPEED_AND_CADENCE ) {
 if ( is_writeable_response ( att_data -> opcode ) || att_data -> opcode == ATT_OPCODE_HANDLE_VALUE_CONFIRMATION ) break ;
 if ( ! is_writeable_request ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_INDICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_sc_control_point_opcode , tvb , offset , 1 , ENC_NA ) ;
 opcode = tvb_get_guint8 ( tvb , offset ) ;
 offset += 1 ;
 switch ( opcode ) {
 case 1 : proto_tree_add_item ( tree , hf_btatt_sc_control_point_cumulative_value , tvb , offset , 4 , ENC_LITTLE_ENDIAN ) ;
 offset += 4 ;
 break ;
 case 3 : proto_tree_add_item ( tree , hf_btatt_sensor_location , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 16 : proto_tree_add_item ( tree , hf_btatt_sc_control_point_request_opcode , tvb , offset , 1 , ENC_NA ) ;
 value = tvb_get_guint8 ( tvb , offset ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_sc_control_point_response_value , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 if ( value == 0x04 && tvb_get_guint8 ( tvb , offset ) == 0x01 ) {
 while ( tvb_captured_length_remaining ( tvb , offset ) ) {
 proto_tree_add_item ( tree , hf_btatt_sensor_location , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 }
 }
 break ;
 case 2 : case 4 : break ;
 }
 break ;
 case 0x2A56 : if ( service_uuid . bt_uuid == GATT_SERVICE_AUTOMATION_IO ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) || att_data -> opcode == ATT_OPCODE_HANDLE_VALUE_CONFIRMATION ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_WRITE_COMMAND && att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_digital , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A58 : if ( service_uuid . bt_uuid == GATT_SERVICE_AUTOMATION_IO ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) || att_data -> opcode == ATT_OPCODE_HANDLE_VALUE_CONFIRMATION ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_WRITE_COMMAND && att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_analog , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 case 0x2A5A : if ( service_uuid . bt_uuid == GATT_SERVICE_AUTOMATION_IO ) {
 if ( is_readable_request ( att_data -> opcode ) || att_data -> opcode == ATT_OPCODE_HANDLE_VALUE_CONFIRMATION ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 call_dissector_with_data ( find_dissector ( "btgatt.uuid0x2a56" ) , tvb_new_subset_length_caplen ( tvb , offset , 1 , 1 ) , pinfo , tree , att_data ) ;
 offset += 1 ;
 call_dissector_with_data ( find_dissector ( "btgatt.uuid0x2a58" ) , tvb_new_subset_length_caplen ( tvb , offset , 2 , 2 ) , pinfo , tree , att_data ) ;
 offset += 2 ;
 break ;
 case 0x2A5B : if ( service_uuid . bt_uuid == GATT_SERVICE_CYCLING_SPEED_AND_CADENCE ) {
 if ( att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_csc_measurement_flags , ett_btatt_value , hfx_btatt_csc_measurement_flags , ENC_NA ) ;
 flags = tvb_get_guint8 ( tvb , offset ) ;
 offset += 1 ;
 if ( flags & 0x01 ) {
 proto_tree_add_item ( tree , hf_btatt_csc_measurement_cumulative_wheel_revolutions , tvb , offset , 4 , ENC_LITTLE_ENDIAN ) ;
 offset += 4 ;
 proto_tree_add_item ( tree , hf_btatt_csc_measurement_last_event_time , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 if ( flags & 0x02 ) {
 proto_tree_add_item ( tree , hf_btatt_csc_measurement_cumulative_crank_revolutions , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( tree , hf_btatt_csc_measurement_last_event_time , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 break ;
 case 0x2A5C : if ( service_uuid . bt_uuid == GATT_SERVICE_CYCLING_SPEED_AND_CADENCE ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_csc_feature , ett_btatt_value , hfx_btatt_csc_feature , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 case 0x2A5D : if ( service_uuid . bt_uuid == GATT_SERVICE_RUNNING_SPEED_AND_CADENCE || service_uuid . bt_uuid == GATT_SERVICE_CYCLING_SPEED_AND_CADENCE || service_uuid . bt_uuid == GATT_SERVICE_CYCLING_POWER ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_sensor_location , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A5E : if ( service_uuid . bt_uuid == GATT_SERVICE_PULSE_OXIMETER ) {
 if ( att_data -> opcode == ATT_OPCODE_HANDLE_VALUE_CONFIRMATION ) break ;
 if ( att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_INDICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_plx_spot_check_measurement_flags , ett_btatt_value , hfx_btatt_plx_spot_check_measurement_flags , ENC_NA ) ;
 flags = tvb_get_guint8 ( tvb , offset ) ;
 offset += 1 ;
 sub_item = proto_tree_add_item ( tree , hf_btatt_plx_spo2pr_spot_check , tvb , offset , 4 , ENC_NA ) ;
 sub_tree = proto_item_add_subtree ( sub_item , ett_btatt_value ) ;
 proto_tree_add_item ( sub_tree , hf_btatt_plx_spo2 , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( sub_tree , hf_btatt_plx_pulse_rate , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 if ( flags & 0x01 ) {
 sub_item = proto_tree_add_item ( tree , hf_btatt_plx_spot_check_measurement_timestamp , tvb , offset , 7 , ENC_NA ) ;
 sub_tree = proto_item_add_subtree ( sub_item , ett_btatt_value ) ;
 call_dissector_with_data ( find_dissector ( "btgatt.uuid0x2a08" ) , tvb_new_subset_length_caplen ( tvb , offset , 7 , 7 ) , pinfo , sub_tree , att_data ) ;
 offset += 7 ;
 }
 if ( flags & 0x02 ) {
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_plx_measurement_status , ett_btatt_value , hfx_btatt_plx_measurement_status , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 if ( flags & 0x04 ) {
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_plx_device_and_sensor_status , ett_btatt_value , hfx_btatt_plx_device_and_sensor_status , ENC_LITTLE_ENDIAN ) ;
 offset += 3 ;
 }
 if ( flags & 0x08 ) {
 proto_tree_add_item ( tree , hf_btatt_plx_pulse_amplitude_index , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 break ;
 case 0x2A5F : if ( service_uuid . bt_uuid == GATT_SERVICE_PULSE_OXIMETER ) {
 if ( att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_plx_continuous_measurement_flags , ett_btatt_value , hfx_btatt_plx_continuous_measurement_flags , ENC_NA ) ;
 flags = tvb_get_guint8 ( tvb , offset ) ;
 offset += 1 ;
 sub_item = proto_tree_add_item ( tree , hf_btatt_plx_spo2pr_normal , tvb , offset , 4 , ENC_NA ) ;
 sub_tree = proto_item_add_subtree ( sub_item , ett_btatt_value ) ;
 proto_tree_add_item ( sub_tree , hf_btatt_plx_spo2 , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( sub_tree , hf_btatt_plx_pulse_rate , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 if ( flags & 0x01 ) {
 sub_item = proto_tree_add_item ( tree , hf_btatt_plx_spo2pr_fast , tvb , offset , 4 , ENC_NA ) ;
 sub_tree = proto_item_add_subtree ( sub_item , ett_btatt_value ) ;
 proto_tree_add_item ( sub_tree , hf_btatt_plx_spo2 , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( sub_tree , hf_btatt_plx_pulse_rate , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 if ( flags & 0x02 ) {
 sub_item = proto_tree_add_item ( tree , hf_btatt_plx_spo2pr_slow , tvb , offset , 4 , ENC_NA ) ;
 sub_tree = proto_item_add_subtree ( sub_item , ett_btatt_value ) ;
 proto_tree_add_item ( sub_tree , hf_btatt_plx_spo2 , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( sub_tree , hf_btatt_plx_pulse_rate , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 if ( flags & 0x04 ) {
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_plx_measurement_status , ett_btatt_value , hfx_btatt_plx_measurement_status , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 if ( flags & 0x08 ) {
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_plx_device_and_sensor_status , ett_btatt_value , hfx_btatt_plx_device_and_sensor_status , ENC_LITTLE_ENDIAN ) ;
 offset += 3 ;
 }
 if ( flags & 0x10 ) {
 proto_tree_add_item ( tree , hf_btatt_plx_pulse_amplitude_index , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 break ;
 case 0x2A60 : if ( service_uuid . bt_uuid == GATT_SERVICE_PULSE_OXIMETER ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_plx_features_supported_features , ett_btatt_value , hfx_btatt_plx_features_supported_features , ENC_LITTLE_ENDIAN ) ;
 flags = tvb_get_guint16 ( tvb , offset , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 if ( flags & 0x01 ) {
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_plx_measurement_status , ett_btatt_value , hfx_btatt_plx_measurement_status , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 if ( flags & 0x02 ) {
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_plx_device_and_sensor_status , ett_btatt_value , hfx_btatt_plx_device_and_sensor_status , ENC_LITTLE_ENDIAN ) ;
 offset += 3 ;
 }
 break ;
 case 0x2A63 : if ( service_uuid . bt_uuid == GATT_SERVICE_CYCLING_POWER ) {
 if ( att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_cycling_power_measurement_flags , ett_btatt_value , hfx_btatt_cycling_power_measurement_flags , ENC_LITTLE_ENDIAN ) ;
 flags = tvb_get_guint16 ( tvb , offset , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( tree , hf_btatt_cycling_power_measurement_instantaneous_power , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 if ( flags & 0x01 ) {
 proto_tree_add_item ( tree , hf_btatt_cycling_power_measurement_pedal_power_balance , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 }
 if ( flags & 0x04 ) {
 proto_tree_add_item ( tree , hf_btatt_cycling_power_measurement_accumulated_torque , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 if ( flags & 0x10 ) {
 proto_tree_add_item ( tree , hf_btatt_cycling_power_measurement_wheel_revolution_data_cumulative_wheel_revolutions , tvb , offset , 4 , ENC_LITTLE_ENDIAN ) ;
 offset += 4 ;
 proto_tree_add_item ( tree , hf_btatt_cycling_power_measurement_wheel_revolution_data_last_wheel_event_time , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 if ( flags & 0x20 ) {
 proto_tree_add_item ( tree , hf_btatt_cycling_power_measurement_crank_revolution_data_cumulative_crank_revolutions , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( tree , hf_btatt_cycling_power_measurement_crank_revolution_data_last_crank_event_time , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 if ( flags & 0x40 ) {
 proto_tree_add_item ( tree , hf_btatt_cycling_power_measurement_extreme_force_magnitudes_maximum_force_magnitude , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( tree , hf_btatt_cycling_power_measurement_extreme_force_magnitudes_minimum_force_magnitude , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 if ( flags & 0x80 ) {
 proto_tree_add_item ( tree , hf_btatt_cycling_power_measurement_extreme_torque_magnitudes_maximum_torque_magnitude , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( tree , hf_btatt_cycling_power_measurement_extreme_torque_magnitudes_minimum_torque_magnitude , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 if ( flags & 0x100 ) {
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_cycling_power_measurement_extreme_angles , ett_btatt_value , hfx_btatt_cycling_power_measurement_extreme_angles , ENC_NA ) ;
 offset += 3 ;
 }
 if ( flags & 0x200 ) {
 proto_tree_add_item ( tree , hf_btatt_cycling_power_measurement_top_dead_spot_angle , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 if ( flags & 0x400 ) {
 proto_tree_add_item ( tree , hf_btatt_cycling_power_measurement_bottom_dead_spot_angle , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 if ( flags & 0x800 ) {
 proto_tree_add_item ( tree , hf_btatt_cycling_power_measurement_accumulated_energy , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 break ;
 case 0x2A64 : if ( service_uuid . bt_uuid == GATT_SERVICE_CYCLING_POWER ) {
 if ( att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_cycling_power_vector_flags , ett_btatt_value , hfx_btatt_cycling_power_vector_flags , ENC_NA ) ;
 flags = tvb_get_guint8 ( tvb , offset ) ;
 offset += 1 ;
 if ( flags & 0x01 ) {
 proto_tree_add_item ( tree , hf_btatt_cycling_power_vector_crank_revolution_data_cumulative_crank_revolutions , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( tree , hf_btatt_cycling_power_vector_crank_revolution_data_last_crank_event_time , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 if ( flags & 0x02 ) {
 proto_tree_add_item ( tree , hf_btatt_cycling_power_vector_first_crank_measurement_angle , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 if ( flags & 0x04 ) {
 while ( tvb_reported_length_remaining ( tvb , offset ) > 0 ) {
 proto_tree_add_item ( tree , hf_btatt_cycling_power_vector_instantaneous_force_magnitude_array , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 }
 if ( flags & 0x08 ) {
 while ( tvb_reported_length_remaining ( tvb , offset ) > 0 ) {
 proto_tree_add_item ( tree , hf_btatt_cycling_power_vector_instantaneous_torque_magnitude_array , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 }
 break ;
 case 0x2A65 : if ( service_uuid . bt_uuid == GATT_SERVICE_CYCLING_POWER ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_cycling_power_feature , ett_btatt_value , hfx_btatt_cycling_power_feature , ENC_LITTLE_ENDIAN ) ;
 offset += 4 ;
 break ;
 case 0x2A66 : if ( service_uuid . bt_uuid == GATT_SERVICE_CYCLING_POWER ) {
 if ( is_writeable_response ( att_data -> opcode ) || att_data -> opcode == ATT_OPCODE_HANDLE_VALUE_CONFIRMATION ) break ;
 if ( ! is_writeable_request ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_INDICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_cycling_power_control_point_opcode , tvb , offset , 1 , ENC_NA ) ;
 opcode = tvb_get_guint8 ( tvb , offset ) ;
 offset += 1 ;
 switch ( opcode ) {
 case 1 : proto_tree_add_item ( tree , hf_btatt_cycling_power_control_point_cumulative_value , tvb , offset , 4 , ENC_LITTLE_ENDIAN ) ;
 offset += 4 ;
 break ;
 case 2 : proto_tree_add_item ( tree , hf_btatt_cycling_power_control_point_sensor_location , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 4 : proto_tree_add_item ( tree , hf_btatt_cycling_power_control_point_crank_length , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 case 6 : proto_tree_add_item ( tree , hf_btatt_cycling_power_control_point_chain_length , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 case 8 : proto_tree_add_item ( tree , hf_btatt_cycling_power_control_point_chain_weight , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 case 10 : proto_tree_add_item ( tree , hf_btatt_cycling_power_control_point_span_length , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 case 13 : proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_cycling_power_control_point_content_mask , ett_btatt_value , hfx_btatt_cycling_power_control_point_content_mask , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 case 3 : case 5 : case 7 : case 9 : case 11 : case 12 : case 14 : case 15 : break ;
 case 32 : proto_tree_add_item ( tree , hf_btatt_cycling_power_control_point_request_opcode , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_cycling_power_control_point_response_value , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 switch ( tvb_get_guint8 ( tvb , offset - 2 ) ) {
 case 1 : case 2 : case 4 : case 6 : case 8 : case 10 : case 13 : break ;
 case 3 : if ( tvb_get_guint8 ( tvb , offset - 1 ) == 0x01 ) {
 while ( tvb_captured_length_remaining ( tvb , offset ) ) {
 proto_tree_add_item ( tree , hf_btatt_sensor_location , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 }
 }
 break ;
 case 5 : if ( tvb_get_guint8 ( tvb , offset - 1 ) == 0x01 ) {
 proto_tree_add_item ( tree , hf_btatt_cycling_power_control_point_crank_length , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 break ;
 case 7 : if ( tvb_get_guint8 ( tvb , offset - 1 ) == 0x01 ) {
 proto_tree_add_item ( tree , hf_btatt_cycling_power_control_point_chain_length , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 break ;
 case 9 : if ( tvb_get_guint8 ( tvb , offset - 1 ) == 0x01 ) {
 proto_tree_add_item ( tree , hf_btatt_cycling_power_control_point_chain_weight , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 break ;
 case 11 : if ( tvb_get_guint8 ( tvb , offset - 1 ) == 0x01 ) {
 proto_tree_add_item ( tree , hf_btatt_cycling_power_control_point_span_length , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 break ;
 case 12 : if ( tvb_get_guint8 ( tvb , offset - 1 ) == 0x01 ) {
 proto_tree_add_item ( tree , hf_btatt_cycling_power_control_point_start_offset_compensation , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 break ;
 case 14 : if ( tvb_get_guint8 ( tvb , offset - 1 ) == 0x01 ) {
 proto_tree_add_item ( tree , hf_btatt_cycling_power_control_point_sampling_rate , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 }
 break ;
 case 15 : if ( tvb_get_guint8 ( tvb , offset - 1 ) == 0x01 ) {
 sub_item = proto_tree_add_item ( tree , hf_btatt_cycling_power_control_point_factory_calibration_date , tvb , offset , 7 , ENC_NA ) ;
 sub_tree = proto_item_add_subtree ( sub_item , ett_btatt_list ) ;
 proto_tree_add_item ( sub_tree , hf_btatt_year , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( sub_tree , hf_btatt_month , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( sub_tree , hf_btatt_day , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( sub_tree , hf_btatt_hours , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( sub_tree , hf_btatt_minutes , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( sub_tree , hf_btatt_seconds , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 }
 break ;
 }
 break ;
 }
 break ;
 case 0x2A67 : if ( service_uuid . bt_uuid == GATT_SERVICE_LOCATION_AND_NAVIGATION ) {
 if ( att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_location_and_speed_flags , ett_btatt_value , hfx_btatt_location_and_speed_flags , ENC_LITTLE_ENDIAN ) ;
 flags = tvb_get_guint16 ( tvb , offset , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 if ( flags & 0x01 ) {
 proto_tree_add_item ( tree , hf_btatt_location_and_speed_instantaneous_speed , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 if ( flags & 0x02 ) {
 proto_tree_add_item ( tree , hf_btatt_location_and_speed_total_distance , tvb , offset , 3 , ENC_LITTLE_ENDIAN ) ;
 offset += 3 ;
 }
 if ( flags & 0x04 ) {
 proto_tree_add_item ( tree , hf_btatt_location_and_speed_location_latitude , tvb , offset , 4 , ENC_LITTLE_ENDIAN ) ;
 offset += 4 ;
 proto_tree_add_item ( tree , hf_btatt_location_and_speed_location_longitude , tvb , offset , 4 , ENC_LITTLE_ENDIAN ) ;
 offset += 4 ;
 }
 if ( flags & 0x08 ) {
 proto_tree_add_item ( tree , hf_btatt_location_and_speed_elevation , tvb , offset , 3 , ENC_LITTLE_ENDIAN ) ;
 offset += 3 ;
 }
 if ( flags & 0x10 ) {
 proto_tree_add_item ( tree , hf_btatt_location_and_speed_heading , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 if ( flags & 0x20 ) {
 proto_tree_add_item ( tree , hf_btatt_location_and_speed_rolling_time , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 }
 if ( flags & 0x40 ) {
 sub_item = proto_tree_add_item ( tree , hf_btatt_location_and_speed_utc_time , tvb , offset , 7 , ENC_NA ) ;
 sub_tree = proto_item_add_subtree ( sub_item , ett_btatt_list ) ;
 proto_tree_add_item ( sub_tree , hf_btatt_year , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( sub_tree , hf_btatt_month , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( sub_tree , hf_btatt_day , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( sub_tree , hf_btatt_hours , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( sub_tree , hf_btatt_minutes , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( sub_tree , hf_btatt_seconds , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 }
 break ;
 case 0x2A68 : if ( service_uuid . bt_uuid == GATT_SERVICE_LOCATION_AND_NAVIGATION ) {
 if ( att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_navigation_flags , ett_btatt_value , hfx_btatt_navigation_flags , ENC_LITTLE_ENDIAN ) ;
 flags = tvb_get_guint16 ( tvb , offset , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( tree , hf_btatt_navigation_bearing , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( tree , hf_btatt_navigation_heading , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 if ( flags & 0x01 ) {
 proto_tree_add_item ( tree , hf_btatt_navigation_remaining_distance , tvb , offset , 3 , ENC_LITTLE_ENDIAN ) ;
 offset += 3 ;
 }
 if ( flags & 0x02 ) {
 proto_tree_add_item ( tree , hf_btatt_navigation_remaining_vertical_distance , tvb , offset , 3 , ENC_LITTLE_ENDIAN ) ;
 offset += 3 ;
 }
 if ( flags & 0x04 ) {
 sub_item = proto_tree_add_item ( tree , hf_btatt_navigation_estimated_time_of_arrival , tvb , offset , 7 , ENC_NA ) ;
 sub_tree = proto_item_add_subtree ( sub_item , ett_btatt_list ) ;
 proto_tree_add_item ( sub_tree , hf_btatt_year , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( sub_tree , hf_btatt_month , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( sub_tree , hf_btatt_day , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( sub_tree , hf_btatt_hours , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( sub_tree , hf_btatt_minutes , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( sub_tree , hf_btatt_seconds , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 }
 break ;
 case 0x2A69 : if ( service_uuid . bt_uuid == GATT_SERVICE_LOCATION_AND_NAVIGATION ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_position_quality_flags , ett_btatt_value , hfx_btatt_position_quality_flags , ENC_LITTLE_ENDIAN ) ;
 flags = tvb_get_guint16 ( tvb , offset , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 if ( flags & 0x01 ) {
 proto_tree_add_item ( tree , hf_btatt_position_quality_number_of_beacons_in_solution , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 }
 if ( flags & 0x02 ) {
 proto_tree_add_item ( tree , hf_btatt_position_quality_number_of_beacons_in_view , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 }
 if ( flags & 0x04 ) {
 proto_tree_add_item ( tree , hf_btatt_position_quality_time_to_first_fix , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 if ( flags & 0x08 ) {
 proto_tree_add_item ( tree , hf_btatt_position_quality_ehpe , tvb , offset , 4 , ENC_LITTLE_ENDIAN ) ;
 offset += 4 ;
 }
 if ( flags & 0x10 ) {
 proto_tree_add_item ( tree , hf_btatt_position_quality_evpe , tvb , offset , 4 , ENC_LITTLE_ENDIAN ) ;
 offset += 4 ;
 }
 if ( flags & 0x20 ) {
 proto_tree_add_item ( tree , hf_btatt_position_quality_hdop , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 }
 if ( flags & 0x40 ) {
 proto_tree_add_item ( tree , hf_btatt_position_quality_vdop , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 }
 break ;
 case 0x2A6A : if ( service_uuid . bt_uuid == GATT_SERVICE_LOCATION_AND_NAVIGATION ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_ln_feature , ett_btatt_value , hfx_btatt_ln_feature , ENC_LITTLE_ENDIAN ) ;
 offset += 4 ;
 break ;
 case 0x2A6B : if ( service_uuid . bt_uuid == GATT_SERVICE_LOCATION_AND_NAVIGATION ) {
 if ( is_writeable_response ( att_data -> opcode ) || att_data -> opcode == ATT_OPCODE_HANDLE_VALUE_CONFIRMATION ) break ;
 if ( ! is_writeable_request ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_INDICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_ln_control_point_opcode , tvb , offset , 1 , ENC_NA ) ;
 opcode = tvb_get_guint8 ( tvb , offset ) ;
 offset += 1 ;
 switch ( opcode ) {
 case 1 : proto_tree_add_item ( tree , hf_btatt_ln_control_point_cumulative_value , tvb , offset , 3 , ENC_LITTLE_ENDIAN ) ;
 offset += 3 ;
 break ;
 case 2 : proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_ln_control_point_content_mask , ett_btatt_value , hfx_btatt_ln_control_point_content_mask , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 case 3 : proto_tree_add_item ( tree , hf_btatt_ln_control_point_navigation_control , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 4 : break ;
 case 5 : case 6 : proto_tree_add_item ( tree , hf_btatt_ln_control_point_route_number , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 case 7 : proto_tree_add_item ( tree , hf_btatt_ln_control_point_fix_rate , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 8 : proto_tree_add_item ( tree , hf_btatt_ln_control_point_elevation , tvb , offset , 3 , ENC_LITTLE_ENDIAN ) ;
 offset += 3 ;
 break ;
 case 32 : proto_tree_add_item ( tree , hf_btatt_ln_control_point_request_opcode , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_ln_control_point_response_value , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 switch ( tvb_get_guint8 ( tvb , offset - 2 ) ) {
 case 1 : case 2 : case 3 : case 6 : case 7 : case 8 : break ;
 case 4 : proto_tree_add_item ( tree , hf_btatt_ln_control_point_response_value_number_of_routes , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 case 5 : proto_tree_add_item ( tree , hf_btatt_ln_control_point_response_value_name_of_route , tvb , offset , tvb_captured_length_remaining ( tvb , offset ) , ENC_NA | ENC_UTF_8 ) ;
 offset += tvb_captured_length_remaining ( tvb , offset ) ;
 break ;
 }
 break ;
 }
 break ;
 case 0x2A6C : if ( service_uuid . bt_uuid == GATT_SERVICE_ENVIRONMENTAL_SENSING ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_elevation , tvb , offset , 3 , ENC_LITTLE_ENDIAN ) ;
 offset += 3 ;
 break ;
 case 0x2A6D : if ( service_uuid . bt_uuid == GATT_SERVICE_ENVIRONMENTAL_SENSING ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_pressure , tvb , offset , 4 , ENC_LITTLE_ENDIAN ) ;
 offset += 4 ;
 break ;
 case 0x2A6E : if ( service_uuid . bt_uuid == GATT_SERVICE_ENVIRONMENTAL_SENSING ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_temperature , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 case 0x2A6F : if ( service_uuid . bt_uuid == GATT_SERVICE_ENVIRONMENTAL_SENSING ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_humidity , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 case 0x2A70 : if ( service_uuid . bt_uuid == GATT_SERVICE_ENVIRONMENTAL_SENSING ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_true_wind_speed , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 case 0x2A71 : if ( service_uuid . bt_uuid == GATT_SERVICE_ENVIRONMENTAL_SENSING ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_true_wind_direction , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 case 0x2A72 : if ( service_uuid . bt_uuid == GATT_SERVICE_ENVIRONMENTAL_SENSING ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_apparent_wind_speed , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 case 0x2A73 : if ( service_uuid . bt_uuid == GATT_SERVICE_ENVIRONMENTAL_SENSING ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_apparent_wind_direction , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 case 0x2A74 : if ( service_uuid . bt_uuid == GATT_SERVICE_ENVIRONMENTAL_SENSING ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_gust_factor , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A75 : if ( service_uuid . bt_uuid == GATT_SERVICE_ENVIRONMENTAL_SENSING ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_pollen_concentration , tvb , offset , 3 , ENC_LITTLE_ENDIAN ) ;
 offset += 3 ;
 break ;
 case 0x2A76 : if ( service_uuid . bt_uuid == GATT_SERVICE_ENVIRONMENTAL_SENSING ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_uv_index , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A77 : if ( service_uuid . bt_uuid == GATT_SERVICE_ENVIRONMENTAL_SENSING ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_irradiance , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 case 0x2A78 : if ( service_uuid . bt_uuid == GATT_SERVICE_ENVIRONMENTAL_SENSING ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_rainfall , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 case 0x2A79 : if ( service_uuid . bt_uuid == GATT_SERVICE_ENVIRONMENTAL_SENSING ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_wind_chill , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A7A : if ( service_uuid . bt_uuid == GATT_SERVICE_ENVIRONMENTAL_SENSING ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_heart_index , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A7B : if ( service_uuid . bt_uuid == GATT_SERVICE_ENVIRONMENTAL_SENSING ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_dew_point , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A7D : if ( service_uuid . bt_uuid == GATT_SERVICE_ENVIRONMENTAL_SENSING ) {
 if ( att_data -> opcode == ATT_OPCODE_HANDLE_VALUE_CONFIRMATION ) break ;
 if ( att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_INDICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_descriptor_value_changed_flags , ett_btatt_value , hfx_btatt_descriptor_value_changed_flags , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 if ( tvb_reported_length_remaining ( tvb , offset ) == 2 ) {
 proto_tree_add_item ( tree , hf_btatt_uuid16 , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 else if ( tvb_reported_length_remaining ( tvb , offset ) == 16 ) {
 proto_tree_add_item ( tree , hf_btatt_uuid128 , tvb , offset , 16 , ENC_NA ) ;
 offset += 16 ;
 }
 else {
 sub_item = proto_tree_add_item ( tree , hf_btatt_value , tvb , offset , - 1 , ENC_NA ) ;
 expert_add_info ( pinfo , sub_item , & ei_btatt_bad_data ) ;
 offset = tvb_captured_length ( tvb ) ;
 }
 break ;
 case 0x2A7E : if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_aerobic_heart_rate_lower_limit , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A7F : if ( service_uuid . bt_uuid == GATT_SERVICE_USER_DATA ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_aerobic_threshold , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A80 : if ( service_uuid . bt_uuid == GATT_SERVICE_USER_DATA ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_age , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A81 : if ( service_uuid . bt_uuid == GATT_SERVICE_USER_DATA ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_anaerobic_heart_rate_lower_limit , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A82 : if ( service_uuid . bt_uuid == GATT_SERVICE_USER_DATA ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_anaerobic_heart_rate_upper_limit , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A83 : if ( service_uuid . bt_uuid == GATT_SERVICE_USER_DATA ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_anaerobic_threshold , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A84 : if ( service_uuid . bt_uuid == GATT_SERVICE_USER_DATA ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_aerobic_heart_rate_upper_limit , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A85 : case 0x2A86 : if ( service_uuid . bt_uuid == GATT_SERVICE_USER_DATA ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_year , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( tree , hf_btatt_month , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_day , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A87 : if ( service_uuid . bt_uuid == GATT_SERVICE_USER_DATA ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_email_address , tvb , offset , tvb_captured_length_remaining ( tvb , offset ) , ENC_NA | ENC_UTF_8 ) ;
 offset += tvb_captured_length_remaining ( tvb , offset ) ;
 break ;
 case 0x2A88 : if ( service_uuid . bt_uuid == GATT_SERVICE_USER_DATA ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_fat_burn_heart_rate_lower_limit , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A89 : if ( service_uuid . bt_uuid == GATT_SERVICE_USER_DATA ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_fat_burn_heart_rate_upper_limit , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A8A : if ( service_uuid . bt_uuid == GATT_SERVICE_USER_DATA ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_first_name , tvb , offset , tvb_captured_length_remaining ( tvb , offset ) , ENC_NA | ENC_UTF_8 ) ;
 offset += tvb_captured_length_remaining ( tvb , offset ) ;
 break ;
 case 0x2A8B : if ( service_uuid . bt_uuid == GATT_SERVICE_USER_DATA ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_five_zone_heart_rate_limits_very_light_light_limit , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_five_zone_heart_rate_limits_light_moderate_limit , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_five_zone_heart_rate_limits_moderate_hard_limit , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_five_zone_heart_rate_limits_hard_maximum_limit , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A8C : if ( service_uuid . bt_uuid == GATT_SERVICE_USER_DATA ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_gender , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A8D : if ( service_uuid . bt_uuid == GATT_SERVICE_USER_DATA ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_heart_rate_max , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A8E : if ( service_uuid . bt_uuid == GATT_SERVICE_USER_DATA ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_height , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 case 0x2A8F : if ( service_uuid . bt_uuid == GATT_SERVICE_USER_DATA ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_hip_circumference , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 case 0x2A90 : if ( service_uuid . bt_uuid == GATT_SERVICE_USER_DATA ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_last_name , tvb , offset , tvb_captured_length_remaining ( tvb , offset ) , ENC_NA | ENC_UTF_8 ) ;
 offset += tvb_captured_length_remaining ( tvb , offset ) ;
 break ;
 case 0x2A91 : if ( service_uuid . bt_uuid == GATT_SERVICE_USER_DATA ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_maximum_recommended_heart_rate , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A92 : if ( service_uuid . bt_uuid == GATT_SERVICE_USER_DATA ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_resting_heart_rate , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A93 : if ( service_uuid . bt_uuid == GATT_SERVICE_USER_DATA ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_sport_type_for_aerobic_and_anaerobic_thresholds , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A94 : if ( service_uuid . bt_uuid == GATT_SERVICE_USER_DATA ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_three_zone_heart_rate_limits_light_moderate , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_three_zone_heart_rate_limits_moderate_hard , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A95 : if ( service_uuid . bt_uuid == GATT_SERVICE_USER_DATA ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_two_zone_heart_rate_limit_fat_burn_fitness , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A96 : if ( service_uuid . bt_uuid == GATT_SERVICE_USER_DATA ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_vo2_max , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A97 : if ( service_uuid . bt_uuid == GATT_SERVICE_USER_DATA ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_waist_circumference , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 case 0x2A98 : if ( service_uuid . bt_uuid == GATT_SERVICE_USER_DATA ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_weight , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 case 0x2A99 : if ( service_uuid . bt_uuid == GATT_SERVICE_USER_DATA ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_database_change_increment , tvb , offset , 4 , ENC_LITTLE_ENDIAN ) ;
 offset += 4 ;
 break ;
 case 0x2A9A : if ( service_uuid . bt_uuid == GATT_SERVICE_USER_DATA ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_user_index , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2A9B : if ( service_uuid . bt_uuid == GATT_SERVICE_BODY_COMPOSITION ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_body_composition_feature , ett_btatt_value , hfx_btatt_body_composition_feature , ENC_LITTLE_ENDIAN ) ;
 offset += 4 ;
 break ;
 case 0x2A9C : if ( service_uuid . bt_uuid == GATT_SERVICE_BODY_COMPOSITION ) {
 if ( att_data -> opcode == ATT_OPCODE_HANDLE_VALUE_CONFIRMATION ) break ;
 if ( att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_INDICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_body_composition_measurement_flags , ett_btatt_value , hfx_btatt_body_composition_measurement_flags , ENC_LITTLE_ENDIAN ) ;
 flags = tvb_get_guint16 ( tvb , offset , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( tree , hf_btatt_body_composition_measurement_body_fat_percentage , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 if ( flags & 0x02 ) {
 sub_item = proto_tree_add_item ( tree , hf_btatt_body_composition_measurement_timestamp , tvb , offset , 7 , ENC_NA ) ;
 sub_tree = proto_item_add_subtree ( sub_item , ett_btatt_list ) ;
 proto_tree_add_item ( sub_tree , hf_btatt_year , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( sub_tree , hf_btatt_month , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( sub_tree , hf_btatt_day , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( sub_tree , hf_btatt_hours , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( sub_tree , hf_btatt_minutes , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( sub_tree , hf_btatt_seconds , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 }
 if ( flags & 0x04 ) {
 proto_tree_add_item ( tree , hf_btatt_body_composition_measurement_user_id , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 }
 if ( flags & 0x08 ) {
 proto_tree_add_item ( sub_tree , hf_btatt_body_composition_measurement_basal_metabolism , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 if ( flags & 0x10 ) {
 proto_tree_add_item ( sub_tree , hf_btatt_body_composition_measurement_muscle_percentage , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 if ( flags & 0x20 ) {
 if ( flags & 0x01 ) proto_tree_add_item ( sub_tree , hf_btatt_body_composition_measurement_muscle_mass_lb , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 else proto_tree_add_item ( sub_tree , hf_btatt_body_composition_measurement_muscle_mass_kg , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 if ( flags & 0x40 ) {
 if ( flags & 0x01 ) proto_tree_add_item ( sub_tree , hf_btatt_body_composition_measurement_fat_free_mass_lb , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 else proto_tree_add_item ( sub_tree , hf_btatt_body_composition_measurement_fat_free_mass_kg , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 if ( flags & 0x80 ) {
 if ( flags & 0x01 ) proto_tree_add_item ( sub_tree , hf_btatt_body_composition_measurement_soft_lean_mass_lb , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 else proto_tree_add_item ( sub_tree , hf_btatt_body_composition_measurement_soft_lean_mass_kg , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 if ( flags & 0x100 ) {
 if ( flags & 0x01 ) proto_tree_add_item ( sub_tree , hf_btatt_body_composition_measurement_body_water_mass_lb , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 else proto_tree_add_item ( sub_tree , hf_btatt_body_composition_measurement_body_water_mass_kg , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 if ( flags & 0x200 ) {
 proto_tree_add_item ( sub_tree , hf_btatt_body_composition_measurement_impedance , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 if ( flags & 0x400 ) {
 if ( flags & 0x01 ) proto_tree_add_item ( sub_tree , hf_btatt_body_composition_measurement_weight_lb , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 else proto_tree_add_item ( sub_tree , hf_btatt_body_composition_measurement_weight_kg , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 if ( flags & 0x800 ) {
 if ( flags & 0x01 ) proto_tree_add_item ( sub_tree , hf_btatt_body_composition_measurement_height_inches , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 else proto_tree_add_item ( sub_tree , hf_btatt_body_composition_measurement_height_meter , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 break ;
 case 0x2A9D : if ( service_uuid . bt_uuid == GATT_SERVICE_WEIGHT_SCALE ) {
 if ( is_writeable_response ( att_data -> opcode ) || att_data -> opcode == ATT_OPCODE_HANDLE_VALUE_CONFIRMATION ) break ;
 if ( ! is_writeable_request ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_INDICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_weight_measurement_flags , ett_btatt_value , hfx_btatt_weight_measurement_flags , ENC_NA ) ;
 flags = tvb_get_guint8 ( tvb , offset ) ;
 offset += 1 ;
 if ( flags & 0x01 ) proto_tree_add_item ( tree , hf_btatt_weight_measurement_weight_lb , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 else proto_tree_add_item ( tree , hf_btatt_weight_measurement_weight_kg , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 if ( flags & 0x02 ) {
 sub_item = proto_tree_add_item ( tree , hf_btatt_weight_measurement_timestamp , tvb , offset , 7 , ENC_NA ) ;
 sub_tree = proto_item_add_subtree ( sub_item , ett_btatt_list ) ;
 proto_tree_add_item ( sub_tree , hf_btatt_year , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( sub_tree , hf_btatt_month , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( sub_tree , hf_btatt_day , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( sub_tree , hf_btatt_hours , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( sub_tree , hf_btatt_minutes , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( sub_tree , hf_btatt_seconds , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 }
 if ( flags & 0x04 ) {
 proto_tree_add_item ( tree , hf_btatt_weight_measurement_user_id , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 }
 if ( flags & 0x08 ) {
 proto_tree_add_item ( sub_tree , hf_btatt_weight_measurement_bmi , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 if ( flags & 0x01 ) proto_tree_add_item ( sub_tree , hf_btatt_weight_measurement_height_in , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 else proto_tree_add_item ( sub_tree , hf_btatt_weight_measurement_height_m , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 break ;
 case 0x2A9E : if ( service_uuid . bt_uuid == GATT_SERVICE_WEIGHT_SCALE ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_weight_scale_feature , ett_btatt_value , hfx_btatt_weight_scale_feature , ENC_LITTLE_ENDIAN ) ;
 offset += 4 ;
 break ;
 case 0x2A9F : if ( service_uuid . bt_uuid == GATT_SERVICE_USER_DATA ) {
 if ( is_writeable_response ( att_data -> opcode ) || att_data -> opcode == ATT_OPCODE_HANDLE_VALUE_CONFIRMATION ) break ;
 if ( ! is_writeable_request ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_INDICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_user_control_point_opcode , tvb , offset , 1 , ENC_NA ) ;
 opcode = tvb_get_guint8 ( tvb , offset ) ;
 offset += 1 ;
 switch ( opcode ) {
 case 0x01 : sub_item = proto_tree_add_item ( tree , hf_btatt_user_control_point_consent_code , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 value = tvb_get_guint16 ( tvb , offset , ENC_LITTLE_ENDIAN ) ;
 if ( value > 9999 ) expert_add_info ( pinfo , sub_item , & ei_btatt_consent_out_of_bounds ) ;
 offset += 2 ;
 break ;
 case 0x02 : proto_tree_add_item ( tree , hf_btatt_user_index , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 sub_item = proto_tree_add_item ( tree , hf_btatt_user_control_point_consent_code , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 value = tvb_get_guint16 ( tvb , offset , ENC_LITTLE_ENDIAN ) ;
 if ( value > 9999 ) expert_add_info ( pinfo , sub_item , & ei_btatt_consent_out_of_bounds ) ;
 offset += 2 ;
 break ;
 case 0x03 : break ;
 case 0x20 : proto_tree_add_item ( tree , hf_btatt_user_control_point_request_opcode , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_user_control_point_response_value , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 if ( tvb_get_guint8 ( tvb , offset - 2 ) == 0x01 && tvb_get_guint8 ( tvb , offset - 1 ) == 0x01 ) {
 proto_tree_add_item ( tree , hf_btatt_user_index , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 }
 break ;
 }
 break ;
 case 0x2AA0 : case 0x2AA1 : if ( service_uuid . bt_uuid == GATT_SERVICE_ENVIRONMENTAL_SENSING ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_magnetic_flux_density_x , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( tree , hf_btatt_magnetic_flux_density_y , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 if ( uuid . bt_uuid == 0x2AA1 ) {
 proto_tree_add_item ( tree , hf_btatt_magnetic_flux_density_z , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 break ;
 case 0x2AA2 : if ( service_uuid . bt_uuid == GATT_SERVICE_USER_DATA ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_language , tvb , offset , tvb_captured_length_remaining ( tvb , offset ) , ENC_NA | ENC_UTF_8 ) ;
 offset += tvb_captured_length_remaining ( tvb , offset ) ;
 break ;
 case 0x2AA3 : if ( service_uuid . bt_uuid == GATT_SERVICE_ENVIRONMENTAL_SENSING ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_barometric_pressure_trend , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2AA4 : if ( service_uuid . bt_uuid == GATT_SERVICE_BOND_MANAGEMENT ) {
 if ( is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_bond_management_control_point_opcode , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 if ( tvb_reported_length_remaining ( tvb , offset ) > 0 ) {
 proto_tree_add_item ( tree , hf_btatt_bond_management_control_point_authorization_code , tvb , offset , length - 1 , ENC_NA | ENC_UTF_8 ) ;
 offset += tvb_reported_length_remaining ( tvb , offset ) ;
 }
 break ;
 case 0x2AA5 : if ( service_uuid . bt_uuid == GATT_SERVICE_BOND_MANAGEMENT ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_bond_management_feature , ett_btatt_value , hfx_btatt_bond_management_feature , ENC_LITTLE_ENDIAN ) ;
 flags = tvb_get_guint24 ( tvb , offset , ENC_LITTLE_ENDIAN ) ;
 offset += 3 ;
 if ( flags & 0x800000 ) {
 do {
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_bond_management_feature_nth , ett_btatt_value , hfx_btatt_bond_management_feature_nth , ENC_LITTLE_ENDIAN ) ;
 offset += 1 ;
 }
 while ( tvb_get_guint8 ( tvb , offset - 1 ) & 0x80 ) ;
 }
 break ;
 case 0x2AA6 : if ( service_uuid . bt_uuid == GATT_SERVICE_GENERIC_ACCESS_PROFILE ) {
 if ( ! ( is_readable_request ( att_data -> opcode ) || is_readable_response ( att_data -> opcode ) ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_central_address_resolution , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2AA7 : if ( service_uuid . bt_uuid == GATT_SERVICE_CONTINUOUS_GLUCOSE_MONITORING ) {
 if ( att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 sub_item = proto_tree_add_item ( tree , hf_btatt_cgm_measurement_size , tvb , offset , 1 , ENC_NA ) ;
 if ( tvb_get_guint8 ( tvb , offset ) >= 6 ) expert_add_info ( pinfo , sub_item , & ei_btatt_cgm_size_too_small ) ;
 offset += 1 ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_cgm_measurement_flags , ett_btatt_value , hfx_btatt_cgm_measurement_flags , ENC_NA ) ;
 flags = tvb_get_guint8 ( tvb , offset ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_cgm_measurement_glucose_concentration , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( tree , hf_btatt_cgm_measurement_time_offset , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 if ( flags & 0xE0 ) {
 value = 0 ;
 if ( flags & 0x80 ) value += 1 ;
 if ( flags & 0x40 ) value += 1 ;
 if ( flags & 0x20 ) value += 1 ;
 sub_item = proto_tree_add_item ( tree , hf_btatt_cgm_sensor_status_annunciation , tvb , offset , value , ENC_NA ) ;
 sub_tree = proto_item_add_subtree ( sub_item , ett_btatt_list ) ;
 }
 if ( flags & 0x80 ) {
 proto_tree_add_bitmask ( sub_tree , tvb , offset , hf_btatt_cgm_sensor_status_annunciation_status , ett_btatt_value , hfx_btatt_cgm_sensor_status_annunciation_status , ENC_NA ) ;
 offset += 1 ;
 }
 if ( flags & 0x40 ) {
 proto_tree_add_bitmask ( sub_tree , tvb , offset , hf_btatt_cgm_sensor_status_annunciation_cal_temp , ett_btatt_value , hfx_btatt_cgm_sensor_status_annunciation_cal_temp , ENC_NA ) ;
 offset += 1 ;
 }
 if ( flags & 0x20 ) {
 proto_tree_add_bitmask ( sub_tree , tvb , offset , hf_btatt_cgm_sensor_status_annunciation_warning , ett_btatt_value , hfx_btatt_cgm_sensor_status_annunciation_warning , ENC_NA ) ;
 offset += 1 ;
 }
 if ( flags & 0x01 ) {
 proto_tree_add_item ( tree , hf_btatt_cgm_measurement_trend_information , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 if ( flags & 0x02 ) {
 proto_tree_add_item ( tree , hf_btatt_cgm_measurement_quality , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 if ( tvb_reported_length_remaining ( tvb , offset ) >= 2 ) {
 proto_tree_add_item ( tree , hf_btatt_cgm_e2e_crc , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 break ;
 case 0x2AA8 : if ( service_uuid . bt_uuid == GATT_SERVICE_CONTINUOUS_GLUCOSE_MONITORING ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_cgm_feature_feature , ett_btatt_value , hfx_btatt_cgm_feature_feature , ENC_LITTLE_ENDIAN ) ;
 offset += 3 ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_cgm_type_and_sample_location , ett_btatt_value , hfx_btatt_cgm_type_and_sample_location , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_cgm_e2e_crc , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 case 0x2AA9 : if ( service_uuid . bt_uuid == GATT_SERVICE_CONTINUOUS_GLUCOSE_MONITORING ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_cgm_time_offset , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 sub_item = proto_tree_add_item ( tree , hf_btatt_cgm_status , tvb , offset , 3 , ENC_LITTLE_ENDIAN ) ;
 sub_tree = proto_item_add_subtree ( sub_item , ett_btatt_list ) ;
 proto_tree_add_bitmask ( sub_tree , tvb , offset , hf_btatt_cgm_sensor_status_annunciation_status , ett_btatt_value , hfx_btatt_cgm_sensor_status_annunciation_status , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_bitmask ( sub_tree , tvb , offset , hf_btatt_cgm_sensor_status_annunciation_cal_temp , ett_btatt_value , hfx_btatt_cgm_sensor_status_annunciation_cal_temp , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_bitmask ( sub_tree , tvb , offset , hf_btatt_cgm_sensor_status_annunciation_warning , ett_btatt_value , hfx_btatt_cgm_sensor_status_annunciation_warning , ENC_NA ) ;
 offset += 1 ;
 if ( tvb_reported_length_remaining ( tvb , offset ) >= 2 ) {
 proto_tree_add_item ( tree , hf_btatt_cgm_e2e_crc , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 break ;
 case 0x2AAA : if ( service_uuid . bt_uuid == GATT_SERVICE_CONTINUOUS_GLUCOSE_MONITORING ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 sub_item = proto_tree_add_item ( tree , hf_btatt_cgm_session_start_time , tvb , offset , 7 , ENC_NA ) ;
 sub_tree = proto_item_add_subtree ( sub_item , ett_btatt_list ) ;
 proto_tree_add_item ( sub_tree , hf_btatt_year , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( sub_tree , hf_btatt_month , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( sub_tree , hf_btatt_day , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( sub_tree , hf_btatt_hours , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( sub_tree , hf_btatt_minutes , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( sub_tree , hf_btatt_seconds , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_timezone , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_dst_offset , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 if ( tvb_reported_length_remaining ( tvb , offset ) >= 2 ) {
 proto_tree_add_item ( tree , hf_btatt_cgm_e2e_crc , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 break ;
 case 0x2AAB : if ( service_uuid . bt_uuid == GATT_SERVICE_CONTINUOUS_GLUCOSE_MONITORING ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_cgm_session_run_time , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 if ( tvb_reported_length_remaining ( tvb , offset ) >= 2 ) {
 proto_tree_add_item ( tree , hf_btatt_cgm_e2e_crc , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 break ;
 case 0x2AAC : if ( service_uuid . bt_uuid == GATT_SERVICE_CONTINUOUS_GLUCOSE_MONITORING ) {
 if ( is_writeable_response ( att_data -> opcode ) || att_data -> opcode == ATT_OPCODE_HANDLE_VALUE_CONFIRMATION ) break ;
 if ( ! is_writeable_request ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_INDICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_cgm_specific_ops_control_point_opcode , tvb , offset , 1 , ENC_NA ) ;
 opcode = tvb_get_guint8 ( tvb , offset ) ;
 offset += 1 ;
 sub_item = proto_tree_add_item ( tree , hf_btatt_cgm_specific_ops_control_point_operand , tvb , offset , 0 , ENC_NA ) ;
 sub_tree = proto_item_add_subtree ( sub_item , ett_btatt_list ) ;
 operand_offset = offset ;
 switch ( opcode ) {
 case 1 : proto_tree_add_item ( sub_tree , hf_btatt_cgm_specific_ops_control_point_operand_communication_interval , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 2 : case 8 : case 11 : case 14 : case 17 : case 20 : case 23 : case 25 : case 26 : case 27 : break ;
 case 4 : proto_tree_add_item ( sub_tree , hf_btatt_cgm_specific_ops_control_point_calibration_glucose_concentration , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( sub_tree , hf_btatt_cgm_specific_ops_control_point_calibration_time , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_bitmask ( sub_tree , tvb , offset , hf_btatt_cgm_type_and_sample_location , ett_btatt_value , hfx_btatt_cgm_type_and_sample_location , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( sub_tree , hf_btatt_cgm_specific_ops_control_point_next_calibration_time , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( sub_tree , hf_btatt_cgm_specific_ops_control_point_calibration_data_record_number , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_bitmask ( sub_tree , tvb , offset , hf_btatt_cgm_specific_ops_control_point_calibration_status , ett_btatt_value , hfx_btatt_cgm_specific_ops_control_point_calibration_status , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 5 : proto_tree_add_item ( sub_tree , hf_btatt_cgm_specific_ops_control_point_operand_calibration_data_record_number , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 case 7 : case 10 : case 13 : case 16 : proto_tree_add_item ( sub_tree , hf_btatt_cgm_specific_ops_control_point_operand_alert_level , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 case 19 : case 22 : proto_tree_add_item ( sub_tree , hf_btatt_cgm_specific_ops_control_point_operand_alert_level_rate , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 case 3 : case 6 : case 9 : case 12 : case 15 : case 18 : case 21 : case 24 : expert_add_info ( pinfo , sub_item , & ei_btatt_opcode_invalid_request ) ;
 break ;
 case 28 : proto_tree_add_item ( sub_tree , hf_btatt_cgm_specific_ops_control_point_request_opcode , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( sub_tree , hf_btatt_cgm_specific_ops_control_point_response_code , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 switch ( tvb_get_guint8 ( tvb , offset - 2 ) ) {
 case 1 : case 2 : case 4 : case 5 : case 7 : case 8 : case 10 : case 11 : case 13 : case 14 : case 16 : case 17 : case 19 : case 20 : case 22 : case 23 : case 25 : case 26 : case 27 : expert_add_info ( pinfo , sub_item , & ei_btatt_opcode_invalid_response ) ;
 break ;
 case 3 : proto_tree_add_item ( sub_tree , hf_btatt_cgm_specific_ops_control_point_operand_communication_interval , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 6 : proto_tree_add_item ( sub_tree , hf_btatt_cgm_specific_ops_control_point_calibration_glucose_concentration , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( sub_tree , hf_btatt_cgm_specific_ops_control_point_calibration_time , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_bitmask ( sub_tree , tvb , offset , hf_btatt_cgm_type_and_sample_location , ett_btatt_value , hfx_btatt_cgm_type_and_sample_location , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( sub_tree , hf_btatt_cgm_specific_ops_control_point_next_calibration_time , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_item ( sub_tree , hf_btatt_cgm_specific_ops_control_point_calibration_data_record_number , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_bitmask ( sub_tree , tvb , offset , hf_btatt_cgm_specific_ops_control_point_calibration_status , ett_btatt_value , hfx_btatt_cgm_specific_ops_control_point_calibration_status , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 9 : case 12 : case 15 : case 18 : proto_tree_add_item ( sub_tree , hf_btatt_cgm_specific_ops_control_point_operand_alert_level , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 case 21 : case 24 : proto_tree_add_item ( sub_tree , hf_btatt_cgm_specific_ops_control_point_operand_alert_level_rate , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 }
 break ;
 }
 ;
 proto_item_set_len ( sub_item , offset - operand_offset ) ;
 if ( tvb_reported_length_remaining ( tvb , offset ) >= 2 ) {
 proto_tree_add_item ( tree , hf_btatt_cgm_e2e_crc , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 break ;
 case 0x2AAD : if ( service_uuid . bt_uuid == GATT_SERVICE_INDOOR_POSITIONING ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_WRITE_COMMAND ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_indoor_positioning_configuration , ett_btatt_value , hfx_btatt_indoor_positioning_configuration , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2AAE : if ( service_uuid . bt_uuid == GATT_SERVICE_INDOOR_POSITIONING ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_WRITE_COMMAND ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_latitude , tvb , offset , 4 , ENC_LITTLE_ENDIAN ) ;
 offset += 4 ;
 break ;
 case 0x2AAF : if ( service_uuid . bt_uuid == GATT_SERVICE_INDOOR_POSITIONING ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_WRITE_COMMAND ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_longitude , tvb , offset , 4 , ENC_LITTLE_ENDIAN ) ;
 offset += 4 ;
 break ;
 case 0x2AB0 : if ( service_uuid . bt_uuid == GATT_SERVICE_INDOOR_POSITIONING ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_WRITE_COMMAND ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_local_north_coordinate , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 case 0x2AB1 : if ( service_uuid . bt_uuid == GATT_SERVICE_INDOOR_POSITIONING ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_WRITE_COMMAND ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_local_east_coordinate , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 case 0x2AB2 : if ( service_uuid . bt_uuid == GATT_SERVICE_INDOOR_POSITIONING ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_WRITE_COMMAND ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_floor_number , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2AB3 : if ( service_uuid . bt_uuid == GATT_SERVICE_INDOOR_POSITIONING ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_WRITE_COMMAND ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_altitude , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 break ;
 case 0x2AB4 : if ( service_uuid . bt_uuid == GATT_SERVICE_INDOOR_POSITIONING ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_WRITE_COMMAND ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_uncertainty , ett_btatt_value , hfx_btatt_uncertainty , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2AB5 : if ( service_uuid . bt_uuid == GATT_SERVICE_INDOOR_POSITIONING ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_WRITE_COMMAND ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_location_name , tvb , offset , tvb_captured_length_remaining ( tvb , offset ) , ENC_NA | ENC_UTF_8 ) ;
 offset += tvb_captured_length_remaining ( tvb , offset ) ;
 break ;
 case 0x2AB6 : if ( service_uuid . bt_uuid == GATT_SERVICE_HTTP_PROXY ) {
 if ( is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_uri , tvb , offset , tvb_captured_length_remaining ( tvb , offset ) , ENC_NA | ENC_UTF_8 ) ;
 offset += tvb_captured_length_remaining ( tvb , offset ) ;
 break ;
 case 0x2AB7 : if ( service_uuid . bt_uuid == GATT_SERVICE_HTTP_PROXY ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 sub_item = proto_tree_add_item ( tree , hf_btatt_http_headers , tvb , offset , tvb_captured_length_remaining ( tvb , offset ) , ENC_NA | ENC_UTF_8 ) ;
 sub_tree = proto_item_add_subtree ( sub_item , ett_btatt_value ) ;
 call_dissector ( http_handle , tvb_new_subset_remaining ( tvb , offset ) , pinfo , sub_tree ) ;
 offset += tvb_captured_length_remaining ( tvb , offset ) ;
 break ;
 case 0x2AB8 : if ( service_uuid . bt_uuid == GATT_SERVICE_HTTP_PROXY ) {
 if ( att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_http_status_code , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_http_data_status , ett_btatt_value , hfx_btatt_http_data_status , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2AB9 : if ( service_uuid . bt_uuid == GATT_SERVICE_HTTP_PROXY ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_http_entity_body , tvb , offset , tvb_captured_length_remaining ( tvb , offset ) , ENC_NA | ENC_UTF_8 ) ;
 offset += tvb_captured_length_remaining ( tvb , offset ) ;
 break ;
 case 0x2ABA : if ( service_uuid . bt_uuid == GATT_SERVICE_HTTP_PROXY ) {
 if ( is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_http_control_point_opcode , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2ABB : if ( service_uuid . bt_uuid == GATT_SERVICE_HTTP_PROXY ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_https_security , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2ABC : if ( service_uuid . bt_uuid == GATT_SERVICE_TRANSPORT_DISCOVERY ) {
 if ( is_writeable_response ( att_data -> opcode ) || att_data -> opcode == ATT_OPCODE_HANDLE_VALUE_CONFIRMATION ) break ;
 if ( ! is_writeable_request ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_INDICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_tds_opcode , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 if ( att_data -> opcode == 0x1B || att_data -> opcode == 0x1D ) {
 proto_tree_add_item ( tree , hf_btatt_tds_result_code , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 }
 else {
 proto_tree_add_item ( tree , hf_btatt_tds_organization_id , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 }
 if ( tvb_reported_length_remaining ( tvb , offset ) > 0 ) {
 proto_tree_add_item ( tree , hf_btatt_tds_data , tvb , offset , tvb_reported_length_remaining ( tvb , offset ) , ENC_NA ) ;
 offset += tvb_reported_length_remaining ( tvb , offset ) ;
 }
 break ;
 case 0x2ABD : if ( service_uuid . bt_uuid == GATT_SERVICE_OBJECT_TRANSFER ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_ots_feature_oacp , ett_btatt_value , hfx_btatt_ots_feature_oacp , ENC_LITTLE_ENDIAN ) ;
 offset += 4 ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_ots_feature_olcp , ett_btatt_value , hfx_btatt_ots_feature_olcp , ENC_LITTLE_ENDIAN ) ;
 offset += 4 ;
 break ;
 case 0x2ABE : if ( service_uuid . bt_uuid == GATT_SERVICE_OBJECT_TRANSFER ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_ots_object_name , tvb , offset , tvb_captured_length_remaining ( tvb , offset ) , ENC_NA | ENC_UTF_8 ) ;
 offset += tvb_captured_length_remaining ( tvb , offset ) ;
 break ;
 case 0x2ABF : if ( service_uuid . bt_uuid == GATT_SERVICE_OBJECT_TRANSFER ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 offset = dissect_gatt_uuid ( tree , pinfo , tvb , offset ) ;
 break ;
 case 0x2AC0 : if ( service_uuid . bt_uuid == GATT_SERVICE_OBJECT_TRANSFER ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_ots_current_size , tvb , offset , 4 , ENC_LITTLE_ENDIAN ) ;
 offset += 4 ;
 proto_tree_add_item ( tree , hf_btatt_ots_allocated_size , tvb , offset , 4 , ENC_LITTLE_ENDIAN ) ;
 offset += 4 ;
 break ;
 case 0x2AC1 : if ( service_uuid . bt_uuid == GATT_SERVICE_OBJECT_TRANSFER ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 sub_item = proto_tree_add_item ( tree , hf_btatt_ots_object_first_created , tvb , offset , 7 , ENC_NA ) ;
 sub_tree = proto_item_add_subtree ( sub_item , ett_btatt_value ) ;
 call_dissector_with_data ( find_dissector ( "btgatt.uuid0x2a08" ) , tvb_new_subset_length_caplen ( tvb , offset , 7 , 7 ) , pinfo , sub_tree , att_data ) ;
 offset += 7 ;
 break ;
 case 0x2AC2 : if ( service_uuid . bt_uuid == GATT_SERVICE_OBJECT_TRANSFER ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 sub_item = proto_tree_add_item ( tree , hf_btatt_ots_object_last_modified , tvb , offset , 7 , ENC_NA ) ;
 sub_tree = proto_item_add_subtree ( sub_item , ett_btatt_value ) ;
 call_dissector_with_data ( find_dissector ( "btgatt.uuid0x2a08" ) , tvb_new_subset_length_caplen ( tvb , offset , 7 , 7 ) , pinfo , sub_tree , att_data ) ;
 offset += 7 ;
 break ;
 case 0x2AC3 : if ( service_uuid . bt_uuid == GATT_SERVICE_OBJECT_TRANSFER ) {
 if ( is_readable_request ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_ots_object_id , tvb , offset , 6 , ENC_LITTLE_ENDIAN ) ;
 offset += 6 ;
 break ;
 case 0x2AC4 : if ( service_uuid . bt_uuid == GATT_SERVICE_OBJECT_TRANSFER ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_ots_properties , ett_btatt_value , hfx_btatt_ots_properties , ENC_LITTLE_ENDIAN ) ;
 offset += 4 ;
 break ;
 case 0x2AC5 : if ( service_uuid . bt_uuid == GATT_SERVICE_OBJECT_TRANSFER ) {
 if ( is_writeable_response ( att_data -> opcode ) || att_data -> opcode == ATT_OPCODE_HANDLE_VALUE_CONFIRMATION ) break ;
 if ( ! is_writeable_request ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_INDICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_ots_action_opcode , tvb , offset , 1 , ENC_NA ) ;
 opcode = tvb_get_guint8 ( tvb , offset ) ;
 offset += 1 ;
 switch ( opcode ) {
 case 0x01 : proto_tree_add_item ( tree , hf_btatt_ots_size , tvb , offset , 4 , ENC_LITTLE_ENDIAN ) ;
 offset += 4 ;
 offset = dissect_gatt_uuid ( tree , pinfo , tvb , offset ) ;
 break ;
 case 0x02 : case 0x07 : break ;
 case 0x03 : case 0x05 : case 0x06 : proto_tree_add_item ( tree , hf_btatt_ots_offset , tvb , offset , 4 , ENC_LITTLE_ENDIAN ) ;
 offset += 4 ;
 proto_tree_add_item ( tree , hf_btatt_ots_length , tvb , offset , 4 , ENC_LITTLE_ENDIAN ) ;
 offset += 4 ;
 break ;
 case 0x04 : if ( tvb_reported_length_remaining ( tvb , offset ) > 0 ) {
 proto_tree_add_item ( tree , hf_btatt_ots_execute_data , tvb , offset , tvb_reported_length_remaining ( tvb , offset ) , ENC_NA | ENC_UTF_8 ) ;
 offset += tvb_reported_length_remaining ( tvb , offset ) ;
 }
 break ;
 case 0x60 : proto_tree_add_item ( tree , hf_btatt_ots_action_response_opcode , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_ots_action_result_code , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 switch ( tvb_get_guint8 ( tvb , offset ) ) {
 case 0x01 : case 0x02 : case 0x05 : case 0x06 : case 0x07 : case 0x60 : break ;
 case 0x03 : proto_tree_add_checksum ( tree , tvb , offset , hf_btatt_ots_checksum , - 1 , NULL , pinfo , 0 , ENC_BIG_ENDIAN , PROTO_CHECKSUM_NO_FLAGS ) ;
 offset += 4 ;
 break ;
 case 0x04 : if ( tvb_reported_length_remaining ( tvb , offset ) > 0 ) {
 proto_tree_add_item ( tree , hf_btatt_ots_execute_data , tvb , offset , tvb_reported_length_remaining ( tvb , offset ) , ENC_NA | ENC_UTF_8 ) ;
 offset += tvb_reported_length_remaining ( tvb , offset ) ;
 }
 break ;
 }
 }
 break ;
 case 0x2AC6 : if ( service_uuid . bt_uuid == GATT_SERVICE_OBJECT_TRANSFER ) {
 if ( is_writeable_response ( att_data -> opcode ) || att_data -> opcode == ATT_OPCODE_HANDLE_VALUE_CONFIRMATION ) break ;
 if ( ! is_writeable_request ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_INDICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_ots_list_opcode , tvb , offset , 1 , ENC_NA ) ;
 opcode = tvb_get_guint8 ( tvb , offset ) ;
 offset += 1 ;
 switch ( opcode ) {
 case 0x01 : case 0x02 : case 0x03 : case 0x04 : case 0x07 : case 0x08 : break ;
 case 0x05 : proto_tree_add_item ( tree , hf_btatt_ots_object_id , tvb , offset , 6 , ENC_LITTLE_ENDIAN ) ;
 offset += 6 ;
 break ;
 case 0x06 : proto_tree_add_item ( tree , hf_btatt_ots_list_order , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x70 : proto_tree_add_item ( tree , hf_btatt_ots_list_response_opcode , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_ots_list_result_code , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 switch ( tvb_get_guint8 ( tvb , offset - 2 ) ) {
 case 0x01 : case 0x02 : case 0x03 : case 0x04 : case 0x05 : case 0x06 : case 0x08 : case 0x70 : break ;
 case 0x07 : proto_tree_add_item ( tree , hf_btatt_ots_list_total_number_of_objects , tvb , offset , 4 , ENC_LITTLE_ENDIAN ) ;
 offset += 4 ;
 }
 }
 break ;
 case 0x2AC7 : if ( service_uuid . bt_uuid == GATT_SERVICE_OBJECT_TRANSFER ) {
 if ( is_readable_request ( att_data -> opcode ) || is_writeable_response ( att_data -> opcode ) ) break ;
 if ( ! is_readable_response ( att_data -> opcode ) && ! is_writeable_request ( att_data -> opcode ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_ots_filter , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 switch ( tvb_get_guint8 ( tvb , offset - 1 ) ) {
 case 0x00 : case 0x0A : break ;
 case 0x01 : case 0x02 : case 0x03 : case 0x04 : proto_tree_add_item ( tree , hf_btatt_ots_name_string , tvb , offset , tvb_reported_length_remaining ( tvb , offset ) , ENC_NA | ENC_UTF_8 ) ;
 offset += tvb_reported_length_remaining ( tvb , offset ) ;
 break ;
 case 0x05 : offset = dissect_gatt_uuid ( tree , pinfo , tvb , offset ) ;
 break ;
 case 0x06 : case 0x07 : call_dissector_with_data ( find_dissector ( "btgatt.uuid0x2a08" ) , tvb_new_subset_length_caplen ( tvb , offset , 7 , 7 ) , pinfo , tree , att_data ) ;
 offset += 7 ;
 call_dissector_with_data ( find_dissector ( "btgatt.uuid0x2a08" ) , tvb_new_subset_length_caplen ( tvb , offset , 7 , 7 ) , pinfo , tree , att_data ) ;
 offset += 7 ;
 break ;
 case 0x08 : case 0x09 : proto_tree_add_item ( tree , hf_btatt_ots_size_from , tvb , offset , 4 , ENC_LITTLE_ENDIAN ) ;
 offset += 4 ;
 proto_tree_add_item ( tree , hf_btatt_ots_size_to , tvb , offset , 4 , ENC_LITTLE_ENDIAN ) ;
 offset += 4 ;
 break ;
 }
 break ;
 case 0x2AC8 : if ( service_uuid . bt_uuid == GATT_SERVICE_OBJECT_TRANSFER ) {
 if ( att_data -> opcode == ATT_OPCODE_HANDLE_VALUE_CONFIRMATION ) break ;
 if ( att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_INDICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_bitmask ( tree , tvb , offset , hf_btatt_ots_flags , ett_btatt_value , hfx_btatt_ots_flags , ENC_NA ) ;
 offset += 1 ;
 proto_tree_add_item ( tree , hf_btatt_ots_object_id , tvb , offset , 6 , ENC_LITTLE_ENDIAN ) ;
 offset += 6 ;
 break ;
 case 0x2AC9 : if ( service_uuid . bt_uuid == GATT_SERVICE_GENERIC_ACCESS_PROFILE ) {
 if ( ! ( is_readable_request ( att_data -> opcode ) || is_readable_response ( att_data -> opcode ) ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_resolvable_private_address , tvb , offset , 1 , ENC_NA ) ;
 offset += 1 ;
 break ;
 case 0x2ACC : if ( service_uuid . bt_uuid == GATT_SERVICE_FITNESS_MACHINE ) {
 if ( ! ( is_readable_request ( att_data -> opcode ) || is_readable_response ( att_data -> opcode ) ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 sub_item = proto_tree_add_item ( tree , hf_btatt_value , tvb , offset , - 1 , ENC_NA ) ;
 expert_add_info ( pinfo , sub_item , & ei_btatt_undecoded ) ;
 offset = tvb_captured_length ( tvb ) ;
 break ;
 case 0x2ACD : if ( service_uuid . bt_uuid == GATT_SERVICE_FITNESS_MACHINE ) {
 if ( att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 sub_item = proto_tree_add_item ( tree , hf_btatt_value , tvb , offset , - 1 , ENC_NA ) ;
 expert_add_info ( pinfo , sub_item , & ei_btatt_undecoded ) ;
 offset = tvb_captured_length ( tvb ) ;
 break ;
 case 0x2ACE : if ( service_uuid . bt_uuid == GATT_SERVICE_FITNESS_MACHINE ) {
 if ( att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 sub_item = proto_tree_add_item ( tree , hf_btatt_value , tvb , offset , - 1 , ENC_NA ) ;
 expert_add_info ( pinfo , sub_item , & ei_btatt_undecoded ) ;
 offset = tvb_captured_length ( tvb ) ;
 break ;
 case 0x2ACF : if ( service_uuid . bt_uuid == GATT_SERVICE_FITNESS_MACHINE ) {
 if ( att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 sub_item = proto_tree_add_item ( tree , hf_btatt_value , tvb , offset , - 1 , ENC_NA ) ;
 expert_add_info ( pinfo , sub_item , & ei_btatt_undecoded ) ;
 offset = tvb_captured_length ( tvb ) ;
 break ;
 case 0x2AD0 : if ( service_uuid . bt_uuid == GATT_SERVICE_FITNESS_MACHINE ) {
 if ( att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 sub_item = proto_tree_add_item ( tree , hf_btatt_value , tvb , offset , - 1 , ENC_NA ) ;
 expert_add_info ( pinfo , sub_item , & ei_btatt_undecoded ) ;
 offset = tvb_captured_length ( tvb ) ;
 break ;
 case 0x2AD1 : if ( service_uuid . bt_uuid == GATT_SERVICE_FITNESS_MACHINE ) {
 if ( att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 sub_item = proto_tree_add_item ( tree , hf_btatt_value , tvb , offset , - 1 , ENC_NA ) ;
 expert_add_info ( pinfo , sub_item , & ei_btatt_undecoded ) ;
 offset = tvb_captured_length ( tvb ) ;
 break ;
 case 0x2AD2 : if ( service_uuid . bt_uuid == GATT_SERVICE_FITNESS_MACHINE ) {
 if ( att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 sub_item = proto_tree_add_item ( tree , hf_btatt_value , tvb , offset , - 1 , ENC_NA ) ;
 expert_add_info ( pinfo , sub_item , & ei_btatt_undecoded ) ;
 offset = tvb_captured_length ( tvb ) ;
 break ;
 case 0x2AD3 : if ( service_uuid . bt_uuid == GATT_SERVICE_FITNESS_MACHINE ) {
 if ( ! ( is_readable_request ( att_data -> opcode ) || is_readable_response ( att_data -> opcode ) || att_data -> opcode == ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 sub_item = proto_tree_add_item ( tree , hf_btatt_value , tvb , offset , - 1 , ENC_NA ) ;
 expert_add_info ( pinfo , sub_item , & ei_btatt_undecoded ) ;
 offset = tvb_captured_length ( tvb ) ;
 break ;
 case 0x2AD4 : if ( service_uuid . bt_uuid == GATT_SERVICE_FITNESS_MACHINE ) {
 if ( ! ( is_readable_request ( att_data -> opcode ) || is_readable_response ( att_data -> opcode ) ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 sub_item = proto_tree_add_item ( tree , hf_btatt_value , tvb , offset , - 1 , ENC_NA ) ;
 expert_add_info ( pinfo , sub_item , & ei_btatt_undecoded ) ;
 offset = tvb_captured_length ( tvb ) ;
 break ;
 case 0x2AD5 : if ( service_uuid . bt_uuid == GATT_SERVICE_FITNESS_MACHINE ) {
 if ( ! ( is_readable_request ( att_data -> opcode ) || is_readable_response ( att_data -> opcode ) ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 sub_item = proto_tree_add_item ( tree , hf_btatt_value , tvb , offset , - 1 , ENC_NA ) ;
 expert_add_info ( pinfo , sub_item , & ei_btatt_undecoded ) ;
 offset = tvb_captured_length ( tvb ) ;
 break ;
 case 0x2AD6 : if ( service_uuid . bt_uuid == GATT_SERVICE_FITNESS_MACHINE ) {
 if ( ! ( is_readable_request ( att_data -> opcode ) || is_readable_response ( att_data -> opcode ) ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 sub_item = proto_tree_add_item ( tree , hf_btatt_value , tvb , offset , - 1 , ENC_NA ) ;
 expert_add_info ( pinfo , sub_item , & ei_btatt_undecoded ) ;
 offset = tvb_captured_length ( tvb ) ;
 break ;
 case 0x2AD7 : if ( service_uuid . bt_uuid == GATT_SERVICE_FITNESS_MACHINE ) {
 if ( ! ( is_readable_request ( att_data -> opcode ) || is_readable_response ( att_data -> opcode ) ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 sub_item = proto_tree_add_item ( tree , hf_btatt_value , tvb , offset , - 1 , ENC_NA ) ;
 expert_add_info ( pinfo , sub_item , & ei_btatt_undecoded ) ;
 offset = tvb_captured_length ( tvb ) ;
 break ;
 case 0x2AD8 : if ( service_uuid . bt_uuid == GATT_SERVICE_FITNESS_MACHINE ) {
 if ( ! ( is_readable_request ( att_data -> opcode ) || is_readable_response ( att_data -> opcode ) ) ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 sub_item = proto_tree_add_item ( tree , hf_btatt_value , tvb , offset , - 1 , ENC_NA ) ;
 expert_add_info ( pinfo , sub_item , & ei_btatt_undecoded ) ;
 offset = tvb_captured_length ( tvb ) ;
 break ;
 case 0x2AD9 : if ( service_uuid . bt_uuid == GATT_SERVICE_FITNESS_MACHINE ) {
 if ( is_writeable_response ( att_data -> opcode ) || att_data -> opcode == ATT_OPCODE_HANDLE_VALUE_CONFIRMATION ) break ;
 if ( ! is_writeable_request ( att_data -> opcode ) && att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_INDICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 sub_item = proto_tree_add_item ( tree , hf_btatt_value , tvb , offset , - 1 , ENC_NA ) ;
 expert_add_info ( pinfo , sub_item , & ei_btatt_undecoded ) ;
 offset = tvb_captured_length ( tvb ) ;
 break ;
 case 0x2ADA : if ( service_uuid . bt_uuid == GATT_SERVICE_FITNESS_MACHINE ) {
 if ( att_data -> opcode != ATT_OPCODE_HANDLE_VALUE_NOTIFICATION ) expert_add_info ( pinfo , tree , & ei_btatt_invalid_usage ) ;
 }
 if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 sub_item = proto_tree_add_item ( tree , hf_btatt_value , tvb , offset , - 1 , ENC_NA ) ;
 expert_add_info ( pinfo , sub_item , & ei_btatt_undecoded ) ;
 offset = tvb_captured_length ( tvb ) ;
 break ;
 case 0x2A1F : case 0x2A20 : case 0x2A2F : case 0x2A30 : case 0x2A3A : case 0x2A3B : case 0x2A3C : case 0x2A3D : case 0x2A3E : case 0x2A57 : case 0x2A59 : case 0x2ADB : case 0x2ADC : case 0x2ADD : case 0x2ADE : case 0x2AE0 : case 0x2AE1 : case 0x2AE2 : case 0x2AE3 : case 0x2AE4 : case 0x2AE5 : case 0x2AE6 : case 0x2AE7 : case 0x2AE8 : case 0x2AE9 : case 0x2AEA : case 0x2AEB : case 0x2AEC : case 0x2AED : case 0x2AEE : case 0x2AEF : case 0x2AF0 : case 0x2AF1 : case 0x2AF2 : case 0x2AF3 : case 0x2AF4 : case 0x2AF5 : case 0x2AF6 : case 0x2AF7 : case 0x2AF8 : case 0x2AF9 : case 0x2AFA : case 0x2AFB : case 0x2AFC : case 0x2AFD : case 0x2AFE : case 0x2AFF : case 0x2B00 : case 0x2B01 : case 0x2B02 : case 0x2B03 : case 0x2B04 : case 0x2B05 : case 0x2B06 : case 0x2B07 : case 0x2B08 : case 0x2B09 : case 0x2B0A : case 0x2B0B : case 0x2B0C : case 0x2B0D : case 0x2B0E : case 0x2B0F : case 0x2B10 : case 0x2B11 : case 0x2B12 : case 0x2B13 : case 0x2B14 : case 0x2B15 : case 0x2B16 : case 0x2B17 : case 0x2B18 : case 0x2B19 : case 0x2B1A : case 0x2B1B : case 0x2B1C : case 0x2B1D : case 0x2B1E : case 0x2B1F : default : if ( bluetooth_gatt_has_no_parameter ( att_data -> opcode ) ) break ;
 proto_tree_add_item ( tree , hf_btatt_value , tvb , offset , - 1 , ENC_NA ) ;
 offset = tvb_captured_length ( tvb ) ;
 }
 return old_offset + offset ;
 }