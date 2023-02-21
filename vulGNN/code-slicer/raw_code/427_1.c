static int sipr_decode_frame ( AVCodecContext * avctx , void * data , int * got_frame_ptr , AVPacket * avpkt ) {
 SiprContext * ctx = avctx -> priv_data ;
 AVFrame * frame = data ;
 const uint8_t * buf = avpkt -> data ;
 SiprParameters parm ;
 const SiprModeParam * mode_par = & modes [ ctx -> mode ] ;
 GetBitContext gb ;
 float * samples ;
 int subframe_size = ctx -> mode == MODE_16k ? L_SUBFR_16k : SUBFR_SIZE ;
 int i , ret ;
 ctx -> avctx = avctx ;
 if ( avpkt -> size < ( mode_par -> bits_per_frame >> 3 ) ) {
 av_log ( avctx , AV_LOG_ERROR , "Error processing packet: packet size (%d) too small\n" , avpkt -> size ) ;
 return - 1 ;
 }
 frame -> nb_samples = mode_par -> frames_per_packet * subframe_size * mode_par -> subframe_count ;
 if ( ( ret = ff_get_buffer ( avctx , frame ) ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "get_buffer() failed\n" ) ;
 return ret ;
 }
 samples = ( float * ) frame -> data [ 0 ] ;
 init_get_bits ( & gb , buf , mode_par -> bits_per_frame ) ;
 for ( i = 0 ;
 i < mode_par -> frames_per_packet ;
 i ++ ) {
 decode_parameters ( & parm , & gb , mode_par ) ;
 ctx -> decode_frame ( ctx , & parm , samples ) ;
 samples += subframe_size * mode_par -> subframe_count ;
 }
 * got_frame_ptr = 1 ;
 return mode_par -> bits_per_frame >> 3 ;
 }