static int bmv_aud_decode_frame ( AVCodecContext * avctx , void * data , int * got_frame_ptr , AVPacket * avpkt ) {
 AVFrame * frame = data ;
 const uint8_t * buf = avpkt -> data ;
 int buf_size = avpkt -> size ;
 int blocks = 0 , total_blocks , i ;
 int ret ;
 int16_t * output_samples ;
 int scale [ 2 ] ;
 total_blocks = * buf ++ ;
 if ( buf_size < total_blocks * 65 + 1 ) {
 av_log ( avctx , AV_LOG_ERROR , "expected %d bytes, got %d\n" , total_blocks * 65 + 1 , buf_size ) ;
 return AVERROR_INVALIDDATA ;
 }
 frame -> nb_samples = total_blocks * 32 ;
 if ( ( ret = ff_get_buffer ( avctx , frame ) ) < 0 ) {
 av_log ( avctx , AV_LOG_ERROR , "get_buffer() failed\n" ) ;
 return ret ;
 }
 output_samples = ( int16_t * ) frame -> data [ 0 ] ;
 for ( blocks = 0 ;
 blocks < total_blocks ;
 blocks ++ ) {
 uint8_t code = * buf ++ ;
 code = ( code >> 1 ) | ( code << 7 ) ;
 scale [ 0 ] = bmv_aud_mults [ code & 0xF ] ;
 scale [ 1 ] = bmv_aud_mults [ code >> 4 ] ;
 for ( i = 0 ;
 i < 32 ;
 i ++ ) {
 * output_samples ++ = av_clip_int16 ( ( scale [ 0 ] * ( int8_t ) * buf ++ ) >> 5 ) ;
 * output_samples ++ = av_clip_int16 ( ( scale [ 1 ] * ( int8_t ) * buf ++ ) >> 5 ) ;
 }
 }
 * got_frame_ptr = 1 ;
 return buf_size ;
 }