static int twin_decode_frame ( AVCodecContext * avctx , void * data , int * got_frame_ptr , AVPacket * avpkt ) {
 AVFrame * frame = data ;
 const uint8_t * buf = avpkt -> data ;
 int buf_size = avpkt -> size ;
 TwinContext * tctx = avctx -> priv_data ;
 GetBitContext gb ;
 const ModeTab * mtab = tctx -> mtab ;
 float * * out = NULL ;
 enum FrameType ftype ;
 int window_type , ret ;
 static const enum FrameType wtype_to_ftype_table [ ] = {
 FT_LONG , FT_LONG , FT_SHORT , FT_LONG , FT_MEDIUM , FT_LONG , FT_LONG , FT_MEDIUM , FT_MEDIUM }
 ;
 if ( buf_size * 8 < avctx -> bit_rate * mtab -> size / avctx -> sample_rate + 8 ) {
 av_log ( avctx , AV_LOG_ERROR , "Frame too small (%d bytes). Truncated file?\n" , buf_size ) ;
 return AVERROR ( EINVAL ) ;
 }
 if ( tctx -> discarded_packets >= 2 ) {
 frame -> nb_samples = mtab -> size ;
 if ( ( ret = ff_get_buffer ( avctx , frame ) ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "get_buffer() failed\n" ) ;
 return ret ;
 }
 out = ( float * * ) frame -> extended_data ;
 }
 init_get_bits ( & gb , buf , buf_size * 8 ) ;
 skip_bits ( & gb , get_bits ( & gb , 8 ) ) ;
 window_type = get_bits ( & gb , WINDOW_TYPE_BITS ) ;
 if ( window_type > 8 ) {
 av_log ( avctx , AV_LOG_ERROR , "Invalid window type, broken sample?\n" ) ;
 return - 1 ;
 }
 ftype = wtype_to_ftype_table [ window_type ] ;
 read_and_decode_spectrum ( tctx , & gb , tctx -> spectrum , ftype ) ;
 imdct_output ( tctx , ftype , window_type , out ) ;
 FFSWAP ( float * , tctx -> curr_frame , tctx -> prev_frame ) ;
 if ( tctx -> discarded_packets < 2 ) {
 tctx -> discarded_packets ++ ;
 * got_frame_ptr = 0 ;
 return buf_size ;
 }
 * got_frame_ptr = 1 ;
 return buf_size ;
 }