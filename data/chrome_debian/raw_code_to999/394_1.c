static int mpeg_field_start ( MpegEncContext * s , const uint8_t * buf , int buf_size ) {
 AVCodecContext * avctx = s -> avctx ;
 Mpeg1Context * s1 = ( Mpeg1Context * ) s ;
 if ( s -> first_field || s -> picture_structure == PICT_FRAME ) {
 if ( ff_MPV_frame_start ( s , avctx ) < 0 ) return - 1 ;
 ff_mpeg_er_frame_start ( s ) ;
 s -> current_picture_ptr -> f . repeat_pict = 0 ;
 if ( s -> repeat_first_field ) {
 if ( s -> progressive_sequence ) {
 if ( s -> top_field_first ) s -> current_picture_ptr -> f . repeat_pict = 4 ;
 else s -> current_picture_ptr -> f . repeat_pict = 2 ;
 }
 else if ( s -> progressive_frame ) {
 s -> current_picture_ptr -> f . repeat_pict = 1 ;
 }
 }
 * s -> current_picture_ptr -> f . pan_scan = s1 -> pan_scan ;
 if ( HAVE_THREADS && ( avctx -> active_thread_type & FF_THREAD_FRAME ) ) ff_thread_finish_setup ( avctx ) ;
 }
 else {
 int i ;
 if ( ! s -> current_picture_ptr ) {
 av_log ( s -> avctx , AV_LOG_ERROR , "first field missing\n" ) ;
 return - 1 ;
 }
 if ( s -> avctx -> hwaccel && ( s -> avctx -> slice_flags & SLICE_FLAG_ALLOW_FIELD ) ) {
 if ( s -> avctx -> hwaccel -> end_frame ( s -> avctx ) < 0 ) av_log ( avctx , AV_LOG_ERROR , "hardware accelerator failed to decode first field\n" ) ;
 }
 for ( i = 0 ;
 i < 4 ;
 i ++ ) {
 s -> current_picture . f . data [ i ] = s -> current_picture_ptr -> f . data [ i ] ;
 if ( s -> picture_structure == PICT_BOTTOM_FIELD ) {
 s -> current_picture . f . data [ i ] += s -> current_picture_ptr -> f . linesize [ i ] ;
 }
 }
 }
 if ( avctx -> hwaccel ) {
 if ( avctx -> hwaccel -> start_frame ( avctx , buf , buf_size ) < 0 ) return - 1 ;
 }
 if ( CONFIG_MPEG_XVMC_DECODER && s -> avctx -> xvmc_acceleration ) if ( ff_xvmc_field_start ( s , avctx ) < 0 ) return - 1 ;
 return 0 ;
 }