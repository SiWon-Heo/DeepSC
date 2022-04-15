void ff_vdpau_h264_set_reference_frames ( H264Context * h ) {
 struct vdpau_render_state * render , * render_ref ;
 VdpReferenceFrameH264 * rf , * rf2 ;
 Picture * pic ;
 int i , list , pic_frame_idx ;
 render = ( struct vdpau_render_state * ) h -> cur_pic_ptr -> f . data [ 0 ] ;
 assert ( render ) ;
 rf = & render -> info . h264 . referenceFrames [ 0 ] ;
 # define H264_RF_COUNT FF_ARRAY_ELEMS ( render -> info . h264 . referenceFrames ) for ( list = 0 ;
 list < 2 ;
 ++ list ) {
 Picture * * lp = list ? h -> long_ref : h -> short_ref ;
 int ls = list ? 16 : h -> short_ref_count ;
 for ( i = 0 ;
 i < ls ;
 ++ i ) {
 pic = lp [ i ] ;
 if ( ! pic || ! pic -> f . reference ) continue ;
 pic_frame_idx = pic -> long_ref ? pic -> pic_id : pic -> frame_num ;
 render_ref = ( struct vdpau_render_state * ) pic -> f . data [ 0 ] ;
 assert ( render_ref ) ;
 rf2 = & render -> info . h264 . referenceFrames [ 0 ] ;
 while ( rf2 != rf ) {
 if ( ( rf2 -> surface == render_ref -> surface ) && ( rf2 -> is_long_term == pic -> long_ref ) && ( rf2 -> frame_idx == pic_frame_idx ) ) break ;
 ++ rf2 ;
 }
 if ( rf2 != rf ) {
 rf2 -> top_is_reference |= ( pic -> f . reference & PICT_TOP_FIELD ) ? VDP_TRUE : VDP_FALSE ;
 rf2 -> bottom_is_reference |= ( pic -> f . reference & PICT_BOTTOM_FIELD ) ? VDP_TRUE : VDP_FALSE ;
 continue ;
 }
 if ( rf >= & render -> info . h264 . referenceFrames [ H264_RF_COUNT ] ) continue ;
 rf -> surface = render_ref -> surface ;
 rf -> is_long_term = pic -> long_ref ;
 rf -> top_is_reference = ( pic -> f . reference & PICT_TOP_FIELD ) ? VDP_TRUE : VDP_FALSE ;
 rf -> bottom_is_reference = ( pic -> f . reference & PICT_BOTTOM_FIELD ) ? VDP_TRUE : VDP_FALSE ;
 rf -> field_order_cnt [ 0 ] = pic -> field_poc [ 0 ] ;
 rf -> field_order_cnt [ 1 ] = pic -> field_poc [ 1 ] ;
 rf -> frame_idx = pic_frame_idx ;
 ++ rf ;
 }
 }
 for ( ;
 rf < & render -> info . h264 . referenceFrames [ H264_RF_COUNT ] ;
 ++ rf ) {
 rf -> surface = VDP_INVALID_HANDLE ;
 rf -> is_long_term = 0 ;
 rf -> top_is_reference = 0 ;
 rf -> bottom_is_reference = 0 ;
 rf -> field_order_cnt [ 0 ] = 0 ;
 rf -> field_order_cnt [ 1 ] = 0 ;
 rf -> frame_idx = 0 ;
 }
 }