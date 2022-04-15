static int decode_tag ( AVCodecContext * avctx , void * data , int * got_frame_ptr , AVPacket * avpkt ) {
 AVFrame * frame = data ;
 const uint8_t * buf = avpkt -> data ;
 int buf_size = avpkt -> size ;
 NellyMoserDecodeContext * s = avctx -> priv_data ;
 int blocks , i , ret ;
 float * samples_flt ;
 blocks = buf_size / NELLY_BLOCK_LEN ;
 if ( blocks <= 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "Packet is too small\n" ) ;
 return AVERROR_INVALIDDATA ;
 }
 if ( buf_size % NELLY_BLOCK_LEN ) {
 av_log ( avctx , AV_LOG_WARNING , "Leftover bytes: %d.\n" , buf_size % NELLY_BLOCK_LEN ) ;
 }
 frame -> nb_samples = NELLY_SAMPLES * blocks ;
 if ( ( ret = ff_get_buffer ( avctx , frame ) ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "get_buffer() failed\n" ) ;
 return ret ;
 }
 samples_flt = ( float * ) frame -> data [ 0 ] ;
 for ( i = 0 ;
 i < blocks ;
 i ++ ) {
 nelly_decode_block ( s , buf , samples_flt ) ;
 samples_flt += NELLY_SAMPLES ;
 buf += NELLY_BLOCK_LEN ;
 }
 * got_frame_ptr = 1 ;
 return buf_size ;
 }