static void vdpau_h264_set_reference_frames ( AVCodecContext * avctx ) {
 H264Context * const h = avctx -> priv_data ;
 AVVDPAUContext * hwctx = avctx -> hwaccel_context ;
 VdpPictureInfoH264 * info = & hwctx -> info . h264 ;
 int list ;
 VdpReferenceFrameH264 * rf = & info -> referenceFrames [ 0 ] ;
 # define H264_RF_COUNT FF_ARRAY_ELEMS ( info -> referenceFrames ) for ( list = 0 ;
 list < 2 ;
 ++ list ) {
 Picture * * lp = list ? h -> long_ref : h -> short_ref ;
 int i , ls = list ? 16 : h -> short_ref_count ;
 for ( i = 0 ;
 i < ls ;
 ++ i ) {
 Picture * pic = lp [ i ] ;
 VdpReferenceFrameH264 * rf2 ;
 VdpVideoSurface surface_ref ;
 int pic_frame_idx ;
 if ( ! pic || ! pic -> f . reference ) continue ;
 pic_frame_idx = pic -> long_ref ? pic -> pic_id : pic -> frame_num ;
 surface_ref = ff_vdpau_get_surface_id ( pic ) ;
 rf2 = & info -> referenceFrames [ 0 ] ;
 while ( rf2 != rf ) {
 if ( ( rf2 -> surface == surface_ref ) && ( rf2 -> is_long_term == pic -> long_ref ) && ( rf2 -> frame_idx == pic_frame_idx ) ) break ;
 ++ rf2 ;
 }
 if ( rf2 != rf ) {
 rf2 -> top_is_reference |= ( pic -> f . reference & PICT_TOP_FIELD ) ? VDP_TRUE : VDP_FALSE ;
 rf2 -> bottom_is_reference |= ( pic -> f . reference & PICT_BOTTOM_FIELD ) ? VDP_TRUE : VDP_FALSE ;
 continue ;
 }
 if ( rf >= & info -> referenceFrames [ H264_RF_COUNT ] ) continue ;
 vdpau_h264_set_rf ( rf , pic , pic -> f . reference ) ;
 ++ rf ;
 }
 }
 for ( ;
 rf < & info -> referenceFrames [ H264_RF_COUNT ] ;
 ++ rf ) vdpau_h264_clear_rf ( rf ) ;
 }