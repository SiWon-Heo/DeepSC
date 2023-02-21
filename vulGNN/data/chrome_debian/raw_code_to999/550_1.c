static void vp3_decode_flush ( AVCodecContext * avctx ) {
 Vp3DecodeContext * s = avctx -> priv_data ;
 if ( s -> golden_frame . data [ 0 ] ) {
 if ( s -> golden_frame . data [ 0 ] == s -> last_frame . data [ 0 ] ) memset ( & s -> last_frame , 0 , sizeof ( AVFrame ) ) ;
 if ( s -> current_frame . data [ 0 ] == s -> golden_frame . data [ 0 ] ) memset ( & s -> current_frame , 0 , sizeof ( AVFrame ) ) ;
 ff_thread_release_buffer ( avctx , & s -> golden_frame ) ;
 }
 if ( s -> last_frame . data [ 0 ] ) {
 if ( s -> current_frame . data [ 0 ] == s -> last_frame . data [ 0 ] ) memset ( & s -> current_frame , 0 , sizeof ( AVFrame ) ) ;
 ff_thread_release_buffer ( avctx , & s -> last_frame ) ;
 }
 if ( s -> current_frame . data [ 0 ] ) ff_thread_release_buffer ( avctx , & s -> current_frame ) ;
 }