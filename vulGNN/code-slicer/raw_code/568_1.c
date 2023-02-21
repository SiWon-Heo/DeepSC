static int roq_encode_end ( AVCodecContext * avctx ) {
 RoqContext * enc = avctx -> priv_data ;
 avctx -> release_buffer ( avctx , enc -> last_frame ) ;
 avctx -> release_buffer ( avctx , enc -> current_frame ) ;
 av_free ( enc -> tmpData ) ;
 av_free ( enc -> this_motion4 ) ;
 av_free ( enc -> last_motion4 ) ;
 av_free ( enc -> this_motion8 ) ;
 av_free ( enc -> last_motion8 ) ;
 return 0 ;
 }