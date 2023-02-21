static gint dissect_rtp ( tvbuff_t * tvb , packet_info * pinfo , proto_tree * tree , void * data _U_ ) {
 proto_item * ti = NULL ;
 proto_tree * volatile rtp_tree = NULL ;
 proto_tree * rtp_csrc_tree = NULL ;
 proto_tree * rtp_hext_tree = NULL ;
 guint8 octet1 , octet2 ;
 unsigned int version ;
 gboolean padding_set ;
 gboolean extension_set ;
 unsigned int csrc_count ;
 gboolean marker_set ;
 unsigned int payload_type ;
 gchar * payload_type_str = NULL ;
 gboolean is_srtp = FALSE ;
 unsigned int i = 0 ;
 unsigned int hdr_extension_len = 0 ;
 unsigned int hdr_extension_id = 0 ;
 volatile unsigned int padding_count ;
 gint length , reported_length ;
 int data_len ;
 volatile unsigned int offset = 0 ;
 guint16 seq_num ;
 guint32 timestamp ;
 guint32 sync_src ;
 guint32 csrc_item ;
 struct _rtp_conversation_info * p_conv_data = NULL ;
 unsigned int hdrext_offset = 0 ;
 tvbuff_t * newtvb = NULL ;
 const char * pt = NULL ;
 static struct _rtp_info rtp_info_arr [ 4 ] ;
 static int rtp_info_current = 0 ;
 struct _rtp_info * rtp_info ;
 rtp_info_current ++ ;
 if ( rtp_info_current == 4 ) {
 rtp_info_current = 0 ;
 }
 rtp_info = & rtp_info_arr [ rtp_info_current ] ;
 octet1 = tvb_get_guint8 ( tvb , offset ) ;
 version = RTP_VERSION ( octet1 ) ;
 if ( version == 0 ) {
 switch ( global_rtp_version0_type ) {
 case RTP0_STUN : call_dissector ( stun_handle , tvb , pinfo , tree ) ;
 return tvb_length ( tvb ) ;
 case RTP0_CLASSICSTUN : call_dissector ( classicstun_handle , tvb , pinfo , tree ) ;
 return tvb_length ( tvb ) ;
 case RTP0_T38 : call_dissector ( t38_handle , tvb , pinfo , tree ) ;
 return tvb_length ( tvb ) ;
 case RTP0_SPRT : call_dissector ( sprt_handle , tvb , pinfo , tree ) ;
 return tvb_length ( tvb ) ;
 case RTP0_INVALID : if ( ! ( tvb_memeql ( tvb , 4 , "ZRTP" , 4 ) ) ) {
 call_dissector ( zrtp_handle , tvb , pinfo , tree ) ;
 return tvb_length ( tvb ) ;
 }
 default : ;
 }
 }
 rtp_info -> info_version = version ;
 if ( version != 2 ) {
 col_set_str ( pinfo -> cinfo , COL_PROTOCOL , "RTP" ) ;
 col_add_fstr ( pinfo -> cinfo , COL_INFO , "Unknown RTP version %u" , version ) ;
 if ( tree ) {
 ti = proto_tree_add_item ( tree , proto_rtp , tvb , offset , - 1 , ENC_NA ) ;
 rtp_tree = proto_item_add_subtree ( ti , ett_rtp ) ;
 proto_tree_add_uint ( rtp_tree , hf_rtp_version , tvb , offset , 1 , octet1 ) ;
 }
 return offset ;
 }
 padding_set = RTP_PADDING ( octet1 ) ;
 extension_set = RTP_EXTENSION ( octet1 ) ;
 csrc_count = RTP_CSRC_COUNT ( octet1 ) ;
 octet2 = tvb_get_guint8 ( tvb , offset + 1 ) ;
 marker_set = RTP_MARKER ( octet2 ) ;
 payload_type = RTP_PAYLOAD_TYPE ( octet2 ) ;
 if ( marker_set && payload_type >= FIRST_RTCP_CONFLICT_PAYLOAD_TYPE && payload_type <= LAST_RTCP_CONFLICT_PAYLOAD_TYPE ) {
 call_dissector ( rtcp_handle , tvb , pinfo , tree ) ;
 return tvb_length ( tvb ) ;
 }
 seq_num = tvb_get_ntohs ( tvb , offset + 2 ) ;
 timestamp = tvb_get_ntohl ( tvb , offset + 4 ) ;
 sync_src = tvb_get_ntohl ( tvb , offset + 8 ) ;
 rtp_info -> info_padding_set = padding_set ;
 rtp_info -> info_padding_count = 0 ;
 rtp_info -> info_marker_set = marker_set ;
 rtp_info -> info_is_video = FALSE ;
 rtp_info -> info_payload_type = payload_type ;
 rtp_info -> info_seq_num = seq_num ;
 rtp_info -> info_timestamp = timestamp ;
 rtp_info -> info_sync_src = sync_src ;
 rtp_info -> info_is_srtp = FALSE ;
 rtp_info -> info_setup_frame_num = 0 ;
 rtp_info -> info_payload_type_str = NULL ;
 rtp_info -> info_payload_rate = 0 ;
 length = tvb_length_remaining ( tvb , offset ) ;
 reported_length = tvb_reported_length_remaining ( tvb , offset ) ;
 if ( reported_length >= 0 && length >= reported_length ) {
 rtp_info -> info_all_data_present = TRUE ;
 rtp_info -> info_data_len = reported_length ;
 rtp_info -> info_data = tvb_get_ptr ( tvb , 0 , - 1 ) ;
 }
 else {
 rtp_info -> info_all_data_present = FALSE ;
 rtp_info -> info_data_len = 0 ;
 rtp_info -> info_data = NULL ;
 }
 get_conv_info ( pinfo , rtp_info ) ;
 p_conv_data = ( struct _rtp_conversation_info * ) p_get_proto_data ( wmem_file_scope ( ) , pinfo , proto_rtp , 0 ) ;
 if ( p_conv_data ) rtp_info -> info_is_video = p_conv_data -> is_video ;
 if ( p_conv_data && p_conv_data -> srtp_info ) is_srtp = TRUE ;
 rtp_info -> info_is_srtp = is_srtp ;
 col_set_str ( pinfo -> cinfo , COL_PROTOCOL , ( is_srtp ) ? "SRTP" : "RTP" ) ;
 p_conv_data = ( struct _rtp_conversation_info * ) p_get_proto_data ( wmem_file_scope ( ) , pinfo , proto_rtp , 0 ) ;
 # if 0 if ( p_conv_data && p_conv_data -> srtp_info ) {
 srtp_info = p_conv_data -> srtp_info ;
 if ( rtp_info -> info_all_data_present ) {
 srtp_offset = rtp_info -> info_data_len - srtp_info -> mki_len - srtp_info -> auth_tag_len ;
 }
 }
 # endif if ( p_conv_data && p_conv_data -> bta2dp_info && p_conv_data -> bta2dp_info -> codec_dissector ) {
 rtp_info -> info_payload_type_str = ( const char * ) dissector_handle_get_short_name ( p_conv_data -> bta2dp_info -> codec_dissector ) ;
 }
 else if ( p_conv_data && p_conv_data -> btvdp_info && p_conv_data -> btvdp_info -> codec_dissector ) {
 rtp_info -> info_payload_type_str = ( const char * ) dissector_handle_get_short_name ( p_conv_data -> btvdp_info -> codec_dissector ) ;
 }
 if ( ( payload_type > 95 ) && ( payload_type < 128 ) ) {
 if ( p_conv_data && p_conv_data -> rtp_dyn_payload ) {
 encoding_name_and_rate_t * encoding_name_and_rate_pt = NULL ;
 # ifdef DEBUG_CONVERSATION rtp_dump_dyn_payload ( p_conv_data -> rtp_dyn_payload ) ;
 # endif DPRINT ( ( "looking up conversation data for dyn_pt=%d" , payload_type ) ) ;
 encoding_name_and_rate_pt = ( encoding_name_and_rate_t * ) g_hash_table_lookup ( p_conv_data -> rtp_dyn_payload , & payload_type ) ;
 DPRINT ( ( "did %sfind conversation data for dyn_pt=%d" , encoding_name_and_rate_pt ? "" : "not " , payload_type ) ) ;
 if ( encoding_name_and_rate_pt ) {
 DPRINT ( ( "found conversation data for dyn_pt=%d, enc_name=%s" , payload_type , encoding_name_and_rate_pt -> encoding_name ) ) ;
 rtp_info -> info_payload_type_str = payload_type_str = encoding_name_and_rate_pt -> encoding_name ;
 rtp_info -> info_payload_rate = encoding_name_and_rate_pt -> sample_rate ;
 }
 }
 }
 if ( p_conv_data && p_conv_data -> bta2dp_info ) {
 pt = ( p_conv_data -> bta2dp_info -> codec_dissector ) ? dissector_handle_get_short_name ( p_conv_data -> bta2dp_info -> codec_dissector ) : "Unknown" ;
 }
 else if ( p_conv_data && p_conv_data -> btvdp_info ) {
 pt = ( p_conv_data -> btvdp_info -> codec_dissector ) ? dissector_handle_get_short_name ( p_conv_data -> btvdp_info -> codec_dissector ) : "Unknown" ;
 }
 else {
 pt = ( payload_type_str ? payload_type_str : val_to_str_ext ( payload_type , & rtp_payload_type_vals_ext , "Unknown (%u)" ) ) ;
 }
 col_add_fstr ( pinfo -> cinfo , COL_INFO , "PT=%s, SSRC=0x%X, Seq=%u, Time=%u%s" , pt , sync_src , seq_num , timestamp , marker_set ? ", Mark" : "" ) ;
 if ( tree ) {
 proto_tree * item ;
 ti = proto_tree_add_item ( tree , proto_rtp , tvb , offset , - 1 , ENC_NA ) ;
 rtp_tree = proto_item_add_subtree ( ti , ett_rtp ) ;
 if ( global_rtp_show_setup_info ) {
 show_setup_info ( tvb , pinfo , rtp_tree ) ;
 }
 proto_tree_add_uint ( rtp_tree , hf_rtp_version , tvb , offset , 1 , octet1 ) ;
 proto_tree_add_boolean ( rtp_tree , hf_rtp_padding , tvb , offset , 1 , octet1 ) ;
 proto_tree_add_boolean ( rtp_tree , hf_rtp_extension , tvb , offset , 1 , octet1 ) ;
 proto_tree_add_uint ( rtp_tree , hf_rtp_csrc_count , tvb , offset , 1 , octet1 ) ;
 offset ++ ;
 proto_tree_add_boolean ( rtp_tree , hf_rtp_marker , tvb , offset , 1 , octet2 ) ;
 proto_tree_add_uint_format ( rtp_tree , hf_rtp_payload_type , tvb , offset , 1 , octet2 , "Payload type: %s (%u)" , pt , payload_type ) ;
 offset ++ ;
 proto_tree_add_uint ( rtp_tree , hf_rtp_seq_nr , tvb , offset , 2 , seq_num ) ;
 if ( p_conv_data != NULL ) {
 item = proto_tree_add_uint ( rtp_tree , hf_rtp_ext_seq_nr , tvb , offset , 2 , p_conv_data -> extended_seqno ) ;
 PROTO_ITEM_SET_GENERATED ( item ) ;
 }
 offset += 2 ;
 proto_tree_add_uint ( rtp_tree , hf_rtp_timestamp , tvb , offset , 4 , timestamp ) ;
 offset += 4 ;
 proto_tree_add_uint ( rtp_tree , hf_rtp_ssrc , tvb , offset , 4 , sync_src ) ;
 offset += 4 ;
 }
 else {
 offset += 12 ;
 }
 if ( csrc_count > 0 ) {
 ti = proto_tree_add_item ( rtp_tree , hf_rtp_csrc_items , tvb , offset , csrc_count * 4 , ENC_NA ) ;
 proto_item_append_text ( ti , " (%u items)" , csrc_count ) ;
 rtp_csrc_tree = proto_item_add_subtree ( ti , ett_csrc_list ) ;
 for ( i = 0 ;
 i < csrc_count ;
 i ++ ) {
 csrc_item = tvb_get_ntohl ( tvb , offset ) ;
 proto_tree_add_uint_format ( rtp_csrc_tree , hf_rtp_csrc_item , tvb , offset , 4 , csrc_item , "CSRC item %d: 0x%X" , i , csrc_item ) ;
 offset += 4 ;
 }
 }
 if ( extension_set ) {
 hdr_extension_id = tvb_get_ntohs ( tvb , offset ) ;
 if ( tree ) proto_tree_add_uint ( rtp_tree , hf_rtp_prof_define , tvb , offset , 2 , hdr_extension_id ) ;
 offset += 2 ;
 hdr_extension_len = tvb_get_ntohs ( tvb , offset ) ;
 if ( tree ) proto_tree_add_uint ( rtp_tree , hf_rtp_length , tvb , offset , 2 , hdr_extension_len ) ;
 offset += 2 ;
 if ( hdr_extension_len > 0 ) {
 if ( tree ) {
 ti = proto_tree_add_item ( rtp_tree , hf_rtp_hdr_exts , tvb , offset , hdr_extension_len * 4 , ENC_NA ) ;
 rtp_hext_tree = proto_item_add_subtree ( ti , ett_hdr_ext ) ;
 }
 newtvb = tvb_new_subset ( tvb , offset , hdr_extension_len * 4 , hdr_extension_len * 4 ) ;
 if ( hdr_extension_id == RTP_RFC5215_ONE_BYTE_SIG ) {
 dissect_rtp_hext_rfc5215_onebyte ( newtvb , pinfo , rtp_hext_tree ) ;
 }
 else if ( ( hdr_extension_id & RTP_RFC5215_TWO_BYTE_MASK ) == RTP_RFC5215_TWO_BYTE_SIG ) {
 dissect_rtp_hext_rfc5215_twobytes ( tvb , offset - 4 , hdr_extension_id , newtvb , pinfo , rtp_hext_tree ) ;
 }
 else {
 if ( ! ( dissector_try_uint ( rtp_hdr_ext_dissector_table , hdr_extension_id , newtvb , pinfo , rtp_hext_tree ) ) ) {
 hdrext_offset = offset ;
 for ( i = 0 ;
 i < hdr_extension_len ;
 i ++ ) {
 if ( tree ) proto_tree_add_uint ( rtp_hext_tree , hf_rtp_hdr_ext , tvb , hdrext_offset , 4 , tvb_get_ntohl ( tvb , hdrext_offset ) ) ;
 hdrext_offset += 4 ;
 }
 }
 }
 }
 offset += hdr_extension_len * 4 ;
 }
 if ( padding_set ) {
 if ( tvb_length ( tvb ) < tvb_reported_length ( tvb ) ) {
 if ( tree ) proto_tree_add_text ( rtp_tree , tvb , 0 , 0 , "Frame has padding, but not all the frame data was captured" ) ;
 call_dissector ( data_handle , tvb_new_subset_remaining ( tvb , offset ) , pinfo , rtp_tree ) ;
 return tvb_length ( tvb ) ;
 }
 padding_count = tvb_get_guint8 ( tvb , tvb_reported_length ( tvb ) - 1 ) ;
 data_len = tvb_reported_length_remaining ( tvb , offset ) - padding_count ;
 rtp_info -> info_payload_offset = offset ;
 rtp_info -> info_payload_len = tvb_length_remaining ( tvb , offset ) ;
 rtp_info -> info_padding_count = padding_count ;
 if ( p_conv_data && p_conv_data -> bta2dp_info ) {
 if ( p_conv_data -> bta2dp_info -> codec_dissector == sbc_handle ) {
 rtp_info -> info_payload_offset += 1 ;
 rtp_info -> info_payload_len -= 1 ;
 }
 if ( p_conv_data -> bta2dp_info -> content_protection_type == BTAVDTP_CONTENT_PROTECTION_TYPE_SCMS_T ) {
 rtp_info -> info_payload_offset += 1 ;
 rtp_info -> info_payload_len -= 1 ;
 }
 }
 if ( p_conv_data && p_conv_data -> btvdp_info && p_conv_data -> bta2dp_info -> content_protection_type == BTAVDTP_CONTENT_PROTECTION_TYPE_SCMS_T ) {
 rtp_info -> info_payload_offset += 1 ;
 rtp_info -> info_payload_len -= 1 ;
 }
 if ( data_len > 0 ) {
 TRY {
 dissect_rtp_data ( tvb , pinfo , tree , rtp_tree , offset , data_len , data_len , payload_type ) ;
 }
 CATCH_ALL {
 if ( ! pinfo -> flags . in_error_pkt ) tap_queue_packet ( rtp_tap , pinfo , rtp_info ) ;
 RETHROW ;
 }
 ENDTRY ;
 offset += data_len ;
 }
 else if ( data_len < 0 ) {
 padding_count = tvb_reported_length_remaining ( tvb , offset ) ;
 }
 if ( padding_count > 1 ) {
 if ( tree ) proto_tree_add_item ( rtp_tree , hf_rtp_padding_data , tvb , offset , padding_count - 1 , ENC_NA ) ;
 offset += padding_count - 1 ;
 }
 if ( tree ) proto_tree_add_item ( rtp_tree , hf_rtp_padding_count , tvb , offset , 1 , ENC_BIG_ENDIAN ) ;
 }
 else {
 rtp_info -> info_payload_offset = offset ;
 rtp_info -> info_payload_len = tvb_length_remaining ( tvb , offset ) ;
 if ( p_conv_data && p_conv_data -> bta2dp_info ) {
 if ( p_conv_data -> bta2dp_info -> codec_dissector == sbc_handle ) {
 rtp_info -> info_payload_offset += 1 ;
 rtp_info -> info_payload_len -= 1 ;
 }
 if ( p_conv_data -> bta2dp_info -> content_protection_type == BTAVDTP_CONTENT_PROTECTION_TYPE_SCMS_T ) {
 rtp_info -> info_payload_offset += 1 ;
 rtp_info -> info_payload_len -= 1 ;
 }
 }
 if ( p_conv_data && p_conv_data -> btvdp_info && p_conv_data -> bta2dp_info -> content_protection_type == BTAVDTP_CONTENT_PROTECTION_TYPE_SCMS_T ) {
 rtp_info -> info_payload_offset += 1 ;
 rtp_info -> info_payload_len -= 1 ;
 }
 if ( tvb_reported_length_remaining ( tvb , offset ) > 0 ) {
 TRY {
 dissect_rtp_data ( tvb , pinfo , tree , rtp_tree , offset , tvb_length_remaining ( tvb , offset ) , tvb_reported_length_remaining ( tvb , offset ) , payload_type ) ;
 }
 CATCH_ALL {
 if ( ! pinfo -> flags . in_error_pkt ) tap_queue_packet ( rtp_tap , pinfo , rtp_info ) ;
 RETHROW ;
 }
 ENDTRY ;
 }
 }
 if ( ! pinfo -> flags . in_error_pkt ) tap_queue_packet ( rtp_tap , pinfo , rtp_info ) ;
 return offset ;
 }