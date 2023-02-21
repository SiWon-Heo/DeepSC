static void free_frame_buffer ( MpegEncContext * s , Picture * pic ) {
 if ( s -> codec_id != AV_CODEC_ID_WMV3IMAGE && s -> codec_id != AV_CODEC_ID_VC1IMAGE && s -> codec_id != AV_CODEC_ID_MSS2 ) ff_thread_release_buffer ( s -> avctx , & pic -> f ) ;
 else avcodec_default_release_buffer ( s -> avctx , & pic -> f ) ;
 av_freep ( & pic -> hwaccel_picture_private ) ;
 }