static int libgsm_decode_frame ( AVCodecContext * avctx , void * data , int * got_frame_ptr , AVPacket * avpkt ) {
 int i , ret ;
 LibGSMDecodeContext * s = avctx -> priv_data ;
 AVFrame * frame = data ;
 uint8_t * buf = avpkt -> data ;
 int buf_size = avpkt -> size ;
 int16_t * samples ;
 if ( buf_size < avctx -> block_align ) {
 av_log ( avctx , AV_LOG_ERROR , "Packet is too small\n" ) ;
 return AVERROR_INVALIDDATA ;
 }
 frame -> nb_samples = avctx -> frame_size ;
 if ( ( ret = ff_get_buffer ( avctx , frame ) ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "get_buffer() failed\n" ) ;
 return ret ;
 }
 samples = ( int16_t * ) frame -> data [ 0 ] ;
 for ( i = 0 ;
 i < avctx -> frame_size / GSM_FRAME_SIZE ;
 i ++ ) {
 if ( ( ret = gsm_decode ( s -> state , buf , samples ) ) < 0 ) return - 1 ;
 buf += GSM_BLOCK_SIZE ;
 samples += GSM_FRAME_SIZE ;
 }
 * got_frame_ptr = 1 ;
 return avctx -> block_align ;
 }