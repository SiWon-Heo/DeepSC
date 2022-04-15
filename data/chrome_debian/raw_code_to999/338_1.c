static void vdpau_h264_set_rf ( VdpReferenceFrameH264 * rf , Picture * pic , int pic_structure ) {
 VdpVideoSurface surface = ff_vdpau_get_surface_id ( pic ) ;
 if ( pic_structure == 0 ) pic_structure = pic -> f . reference ;
 rf -> surface = surface ;
 rf -> is_long_term = pic -> f . reference && pic -> long_ref ;
 rf -> top_is_reference = ( pic_structure & PICT_TOP_FIELD ) != 0 ;
 rf -> bottom_is_reference = ( pic_structure & PICT_BOTTOM_FIELD ) != 0 ;
 rf -> field_order_cnt [ 0 ] = h264_foc ( pic -> field_poc [ 0 ] ) ;
 rf -> field_order_cnt [ 1 ] = h264_foc ( pic -> field_poc [ 1 ] ) ;
 rf -> frame_idx = pic -> long_ref ? pic -> pic_id : pic -> frame_num ;
 }