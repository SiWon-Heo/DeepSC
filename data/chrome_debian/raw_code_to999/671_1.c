static int RTP_packet ( void * ptr _U_ , packet_info * pinfo , epan_dissect_t * edt _U_ , void const * RTPinfo ) {
 voip_rtp_tapinfo_t * tapinfo = & the_tapinfo_rtp_struct ;
 voip_rtp_stream_info_t * tmp_listinfo ;
 voip_rtp_stream_info_t * strinfo = NULL ;
 GList * list ;
 struct _rtp_conversation_info * p_conv_data = NULL ;
 const struct _rtp_info * pi = ( const struct _rtp_info * ) RTPinfo ;
 if ( pi -> info_setup_frame_num == 0 ) {
 return 0 ;
 }
 # ifdef HAVE_LIBPORTAUDIO add_rtp_packet ( pi , pinfo ) ;
 # endif list = g_list_first ( tapinfo -> list ) ;
 while ( list ) {
 tmp_listinfo = ( voip_rtp_stream_info_t * ) list -> data ;
 if ( ( tmp_listinfo -> setup_frame_number == pi -> info_setup_frame_num ) && ( tmp_listinfo -> ssrc == pi -> info_sync_src ) && ( tmp_listinfo -> end_stream == FALSE ) ) {
 if ( tmp_listinfo -> pt != pi -> info_payload_type ) {
 tmp_listinfo -> end_stream = TRUE ;
 }
 else {
 strinfo = ( voip_rtp_stream_info_t * ) ( list -> data ) ;
 break ;
 }
 }
 list = g_list_next ( list ) ;
 }
 if ( ( rtp_evt_frame_num == pinfo -> fd -> num ) && ! strinfo && ( rtp_evt_end == TRUE ) ) {
 return 0 ;
 }
 if ( strinfo == NULL ) {
 strinfo = ( voip_rtp_stream_info_t * ) g_malloc ( sizeof ( voip_rtp_stream_info_t ) ) ;
 COPY_ADDRESS ( & ( strinfo -> src_addr ) , & ( pinfo -> src ) ) ;
 strinfo -> src_port = pinfo -> srcport ;
 COPY_ADDRESS ( & ( strinfo -> dest_addr ) , & ( pinfo -> dst ) ) ;
 strinfo -> dest_port = pinfo -> destport ;
 strinfo -> ssrc = pi -> info_sync_src ;
 strinfo -> end_stream = FALSE ;
 strinfo -> pt = pi -> info_payload_type ;
 strinfo -> pt_str = NULL ;
 strinfo -> is_srtp = pi -> info_is_srtp ;
 if ( ( strinfo -> pt >= PT_UNDF_96 ) && ( strinfo -> pt <= PT_UNDF_127 ) ) {
 p_conv_data = ( struct _rtp_conversation_info * ) p_get_proto_data ( wmem_file_scope ( ) , pinfo , proto_get_id_by_filter_name ( "rtp" ) , 0 ) ;
 if ( p_conv_data && p_conv_data -> rtp_dyn_payload ) {
 encoding_name_and_rate_t * encoding_name_and_rate_pt = NULL ;
 encoding_name_and_rate_pt = ( encoding_name_and_rate_t * ) g_hash_table_lookup ( p_conv_data -> rtp_dyn_payload , & strinfo -> pt ) ;
 if ( encoding_name_and_rate_pt ) {
 strinfo -> pt_str = g_strdup ( encoding_name_and_rate_pt -> encoding_name ) ;
 }
 }
 }
 if ( ! strinfo -> pt_str ) strinfo -> pt_str = g_strdup ( val_to_str_ext ( strinfo -> pt , & rtp_payload_type_short_vals_ext , "%u" ) ) ;
 strinfo -> npackets = 0 ;
 strinfo -> start_fd = pinfo -> fd ;
 strinfo -> start_rel_ts = pinfo -> rel_ts ;
 strinfo -> setup_frame_number = pi -> info_setup_frame_num ;
 strinfo -> rtp_event = - 1 ;
 tapinfo -> list = g_list_prepend ( tapinfo -> list , strinfo ) ;
 }
 strinfo -> npackets ++ ;
 strinfo -> stop_fd = pinfo -> fd ;
 strinfo -> stop_rel_ts = pinfo -> rel_ts ;
 if ( rtp_evt_frame_num == pinfo -> fd -> num ) {
 strinfo -> rtp_event = rtp_evt ;
 if ( rtp_evt_end == TRUE ) {
 strinfo -> end_stream = TRUE ;
 }
 }
 the_tapinfo_struct . redraw = TRUE ;
 return 1 ;
 }