static int truespeech_decode_frame ( AVCodecContext * avctx , void * data , int * got_frame_ptr , AVPacket * avpkt ) {
 AVFrame * frame = data ;
 const uint8_t * buf = avpkt -> data ;
 int buf_size = avpkt -> size ;
 TSContext * c = avctx -> priv_data ;
 int i , j ;
 int16_t * samples ;
 int iterations , ret ;
 iterations = buf_size / 32 ;
 if ( ! iterations ) {
 av_log ( avctx , AV_LOG_ERROR , "Too small input buffer (%d bytes), need at least 32 bytes\n" , buf_size ) ;
 return - 1 ;
 }
 frame -> nb_samples = iterations * 240 ;
 if ( ( ret = ff_get_buffer ( avctx , frame ) ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "get_buffer() failed\n" ) ;
 return ret ;
 }
 samples = ( int16_t * ) frame -> data [ 0 ] ;
 memset ( samples , 0 , iterations * 240 * sizeof ( * samples ) ) ;
 for ( j = 0 ;
 j < iterations ;
 j ++ ) {
 truespeech_read_frame ( c , buf ) ;
 buf += 32 ;
 truespeech_correlate_filter ( c ) ;
 truespeech_filters_merge ( c ) ;
 for ( i = 0 ;
 i < 4 ;
 i ++ ) {
 truespeech_apply_twopoint_filter ( c , i ) ;
 truespeech_place_pulses ( c , samples , i ) ;
 truespeech_update_filters ( c , samples , i ) ;
 truespeech_synth ( c , samples , i ) ;
 samples += 60 ;
 }
 truespeech_save_prevvec ( c ) ;
 }
 * got_frame_ptr = 1 ;
 return buf_size ;
 }