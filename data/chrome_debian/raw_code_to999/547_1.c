static void update_frames ( AVCodecContext * avctx ) {
 Vp3DecodeContext * s = avctx -> priv_data ;
 if ( s -> last_frame . data [ 0 ] && s -> last_frame . type != FF_BUFFER_TYPE_COPY ) ff_thread_release_buffer ( avctx , & s -> last_frame ) ;
 s -> last_frame = s -> current_frame ;
 if ( s -> keyframe ) {
 if ( s -> golden_frame . data [ 0 ] ) ff_thread_release_buffer ( avctx , & s -> golden_frame ) ;
 s -> golden_frame = s -> current_frame ;
 s -> last_frame . type = FF_BUFFER_TYPE_COPY ;
 }
 s -> current_frame . data [ 0 ] = NULL ;
 }