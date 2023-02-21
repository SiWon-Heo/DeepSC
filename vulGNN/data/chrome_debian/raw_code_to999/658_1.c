static int cinaudio_decode_frame ( AVCodecContext * avctx , void * data , int * got_frame_ptr , AVPacket * avpkt ) {
 AVFrame * frame = data ;
 const uint8_t * buf = avpkt -> data ;
 CinAudioContext * cin = avctx -> priv_data ;
 const uint8_t * buf_end = buf + avpkt -> size ;
 int16_t * samples ;
 int delta , ret ;
 frame -> nb_samples = avpkt -> size - cin -> initial_decode_frame ;
 if ( ( ret = ff_get_buffer ( avctx , frame ) ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "get_buffer() failed\n" ) ;
 return ret ;
 }
 samples = ( int16_t * ) frame -> data [ 0 ] ;
 delta = cin -> delta ;
 if ( cin -> initial_decode_frame ) {
 cin -> initial_decode_frame = 0 ;
 delta = sign_extend ( AV_RL16 ( buf ) , 16 ) ;
 buf += 2 ;
 * samples ++ = delta ;
 }
 while ( buf < buf_end ) {
 delta += cinaudio_delta16_table [ * buf ++ ] ;
 delta = av_clip_int16 ( delta ) ;
 * samples ++ = delta ;
 }
 cin -> delta = delta ;
 * got_frame_ptr = 1 ;
 return avpkt -> size ;
 }