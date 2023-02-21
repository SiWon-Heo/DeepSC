static int slice_end ( AVCodecContext * avctx , AVFrame * pict ) {
 Mpeg1Context * s1 = avctx -> priv_data ;
 MpegEncContext * s = & s1 -> mpeg_enc_ctx ;
 if ( ! s1 -> mpeg_enc_ctx_allocated || ! s -> current_picture_ptr ) return 0 ;
 if ( s -> avctx -> hwaccel ) {
 if ( s -> avctx -> hwaccel -> end_frame ( s -> avctx ) < 0 ) av_log ( avctx , AV_LOG_ERROR , "hardware accelerator failed to decode picture\n" ) ;
 }
 if ( CONFIG_MPEG_XVMC_DECODER && s -> avctx -> xvmc_acceleration ) ff_xvmc_field_end ( s ) ;
 if ( ! s -> first_field ) {
 s -> current_picture_ptr -> f . qscale_type = FF_QSCALE_TYPE_MPEG2 ;
 ff_er_frame_end ( & s -> er ) ;
 ff_MPV_frame_end ( s ) ;
 if ( s -> pict_type == AV_PICTURE_TYPE_B || s -> low_delay ) {
 * pict = s -> current_picture_ptr -> f ;
 ff_print_debug_info ( s , pict ) ;
 }
 else {
 if ( avctx -> active_thread_type & FF_THREAD_FRAME ) s -> picture_number ++ ;
 if ( s -> last_picture_ptr != NULL ) {
 * pict = s -> last_picture_ptr -> f ;
 ff_print_debug_info ( s , pict ) ;
 }
 }
 return 1 ;
 }
 else {
 return 0 ;
 }
 }