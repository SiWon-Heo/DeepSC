static int decode_frame ( AVCodecContext * avctx , void * data , int * got_frame , AVPacket * avpkt ) {
 Indeo3DecodeContext * ctx = avctx -> priv_data ;
 const uint8_t * buf = avpkt -> data ;
 int buf_size = avpkt -> size ;
 int res ;
 res = decode_frame_headers ( ctx , avctx , buf , buf_size ) ;
 if ( res < 0 ) return res ;
 if ( res ) {
 * got_frame = 0 ;
 return buf_size ;
 }
 if ( ctx -> frame_flags & BS_NONREF && ( avctx -> skip_frame >= AVDISCARD_NONREF ) ) return 0 ;
 if ( ! ( ctx -> frame_flags & BS_KEYFRAME ) && avctx -> skip_frame >= AVDISCARD_NONKEY ) return 0 ;
 ctx -> buf_sel = ( ctx -> frame_flags >> BS_BUFFER ) & 1 ;
 if ( ( res = decode_plane ( ctx , avctx , ctx -> planes , ctx -> y_data_ptr , ctx -> y_data_size , 40 ) ) ) return res ;
 if ( ( res = decode_plane ( ctx , avctx , & ctx -> planes [ 1 ] , ctx -> u_data_ptr , ctx -> u_data_size , 10 ) ) ) return res ;
 if ( ( res = decode_plane ( ctx , avctx , & ctx -> planes [ 2 ] , ctx -> v_data_ptr , ctx -> v_data_size , 10 ) ) ) return res ;
 if ( ctx -> frame . data [ 0 ] ) avctx -> release_buffer ( avctx , & ctx -> frame ) ;
 ctx -> frame . reference = 0 ;
 if ( ( res = ff_get_buffer ( avctx , & ctx -> frame ) ) < 0 ) {
 av_log ( ctx -> avctx , AV_LOG_ERROR , "get_buffer() failed\n" ) ;
 return res ;
 }
 output_plane ( & ctx -> planes [ 0 ] , ctx -> buf_sel , ctx -> frame . data [ 0 ] , ctx -> frame . linesize [ 0 ] , avctx -> height ) ;
 output_plane ( & ctx -> planes [ 1 ] , ctx -> buf_sel , ctx -> frame . data [ 1 ] , ctx -> frame . linesize [ 1 ] , ( avctx -> height + 3 ) >> 2 ) ;
 output_plane ( & ctx -> planes [ 2 ] , ctx -> buf_sel , ctx -> frame . data [ 2 ] , ctx -> frame . linesize [ 2 ] , ( avctx -> height + 3 ) >> 2 ) ;
 * got_frame = 1 ;
 * ( AVFrame * ) data = ctx -> frame ;
 return buf_size ;
 }