static void fill_picture_parameters ( struct dxva_context * ctx , const H264Context * h , DXVA_PicParams_H264 * pp ) {
 const Picture * current_picture = h -> cur_pic_ptr ;
 int i , j ;
 memset ( pp , 0 , sizeof ( * pp ) ) ;
 fill_picture_entry ( & pp -> CurrPic , ff_dxva2_get_surface_index ( ctx , current_picture ) , h -> picture_structure == PICT_BOTTOM_FIELD ) ;
 pp -> UsedForReferenceFlags = 0 ;
 pp -> NonExistingFrameFlags = 0 ;
 for ( i = 0 , j = 0 ;
 i < FF_ARRAY_ELEMS ( pp -> RefFrameList ) ;
 i ++ ) {
 const Picture * r ;
 if ( j < h -> short_ref_count ) {
 r = h -> short_ref [ j ++ ] ;
 }
 else {
 r = NULL ;
 while ( ! r && j < h -> short_ref_count + 16 ) r = h -> long_ref [ j ++ - h -> short_ref_count ] ;
 }
 if ( r ) {
 fill_picture_entry ( & pp -> RefFrameList [ i ] , ff_dxva2_get_surface_index ( ctx , r ) , r -> long_ref != 0 ) ;
 if ( ( r -> f . reference & PICT_TOP_FIELD ) && r -> field_poc [ 0 ] != INT_MAX ) pp -> FieldOrderCntList [ i ] [ 0 ] = r -> field_poc [ 0 ] ;
 if ( ( r -> f . reference & PICT_BOTTOM_FIELD ) && r -> field_poc [ 1 ] != INT_MAX ) pp -> FieldOrderCntList [ i ] [ 1 ] = r -> field_poc [ 1 ] ;
 pp -> FrameNumList [ i ] = r -> long_ref ? r -> pic_id : r -> frame_num ;
 if ( r -> f . reference & PICT_TOP_FIELD ) pp -> UsedForReferenceFlags |= 1 << ( 2 * i + 0 ) ;
 if ( r -> f . reference & PICT_BOTTOM_FIELD ) pp -> UsedForReferenceFlags |= 1 << ( 2 * i + 1 ) ;
 }
 else {
 pp -> RefFrameList [ i ] . bPicEntry = 0xff ;
 pp -> FieldOrderCntList [ i ] [ 0 ] = 0 ;
 pp -> FieldOrderCntList [ i ] [ 1 ] = 0 ;
 pp -> FrameNumList [ i ] = 0 ;
 }
 }
 pp -> wFrameWidthInMbsMinus1 = h -> mb_width - 1 ;
 pp -> wFrameHeightInMbsMinus1 = h -> mb_height - 1 ;
 pp -> num_ref_frames = h -> sps . ref_frame_count ;
 pp -> wBitFields = ( ( h -> picture_structure != PICT_FRAME ) << 0 ) | ( ( h -> sps . mb_aff && ( h -> picture_structure == PICT_FRAME ) ) << 1 ) | ( h -> sps . residual_color_transform_flag << 2 ) | ( 0 << 3 ) | ( h -> sps . chroma_format_idc << 4 ) | ( ( h -> nal_ref_idc != 0 ) << 6 ) | ( h -> pps . constrained_intra_pred << 7 ) | ( h -> pps . weighted_pred << 8 ) | ( h -> pps . weighted_bipred_idc << 9 ) | ( 1 << 11 ) | ( h -> sps . frame_mbs_only_flag << 12 ) | ( h -> pps . transform_8x8_mode << 13 ) | ( ( h -> sps . level_idc >= 31 ) << 14 ) | ( 1 << 15 ) ;
 pp -> bit_depth_luma_minus8 = h -> sps . bit_depth_luma - 8 ;
 pp -> bit_depth_chroma_minus8 = h -> sps . bit_depth_chroma - 8 ;
 if ( ctx -> workaround & FF_DXVA2_WORKAROUND_SCALING_LIST_ZIGZAG ) pp -> Reserved16Bits = 0 ;
 else pp -> Reserved16Bits = 3 ;
 pp -> StatusReportFeedbackNumber = 1 + ctx -> report_id ++ ;
 pp -> CurrFieldOrderCnt [ 0 ] = 0 ;
 if ( ( h -> picture_structure & PICT_TOP_FIELD ) && current_picture -> field_poc [ 0 ] != INT_MAX ) pp -> CurrFieldOrderCnt [ 0 ] = current_picture -> field_poc [ 0 ] ;
 pp -> CurrFieldOrderCnt [ 1 ] = 0 ;
 if ( ( h -> picture_structure & PICT_BOTTOM_FIELD ) && current_picture -> field_poc [ 1 ] != INT_MAX ) pp -> CurrFieldOrderCnt [ 1 ] = current_picture -> field_poc [ 1 ] ;
 pp -> pic_init_qs_minus26 = h -> pps . init_qs - 26 ;
 pp -> chroma_qp_index_offset = h -> pps . chroma_qp_index_offset [ 0 ] ;
 pp -> second_chroma_qp_index_offset = h -> pps . chroma_qp_index_offset [ 1 ] ;
 pp -> ContinuationFlag = 1 ;
 pp -> pic_init_qp_minus26 = h -> pps . init_qp - 26 ;
 pp -> num_ref_idx_l0_active_minus1 = h -> pps . ref_count [ 0 ] - 1 ;
 pp -> num_ref_idx_l1_active_minus1 = h -> pps . ref_count [ 1 ] - 1 ;
 pp -> Reserved8BitsA = 0 ;
 pp -> frame_num = h -> frame_num ;
 pp -> log2_max_frame_num_minus4 = h -> sps . log2_max_frame_num - 4 ;
 pp -> pic_order_cnt_type = h -> sps . poc_type ;
 if ( h -> sps . poc_type == 0 ) pp -> log2_max_pic_order_cnt_lsb_minus4 = h -> sps . log2_max_poc_lsb - 4 ;
 else if ( h -> sps . poc_type == 1 ) pp -> delta_pic_order_always_zero_flag = h -> sps . delta_pic_order_always_zero_flag ;
 pp -> direct_8x8_inference_flag = h -> sps . direct_8x8_inference_flag ;
 pp -> entropy_coding_mode_flag = h -> pps . cabac ;
 pp -> pic_order_present_flag = h -> pps . pic_order_present ;
 pp -> num_slice_groups_minus1 = h -> pps . slice_group_count - 1 ;
 pp -> slice_group_map_type = h -> pps . mb_slice_group_map_type ;
 pp -> deblocking_filter_control_present_flag = h -> pps . deblocking_filter_parameters_present ;
 pp -> redundant_pic_cnt_present_flag = h -> pps . redundant_pic_cnt_present ;
 pp -> Reserved8BitsB = 0 ;
 pp -> slice_group_change_rate_minus1 = 0 ;
 }